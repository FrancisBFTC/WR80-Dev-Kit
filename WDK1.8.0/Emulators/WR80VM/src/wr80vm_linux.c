// arquivo: wr80vm_linux.c
// Compile no Linux com:
//   gcc wr80vm_linux.c -o wr80vm `sdl2-config --cflags --libs` -lpthread
//
// Observação: requer SDL2 instalado (pkg-config or sdl2-config).
// Execute: sudo apt install libsdl2-dev

#include <SDL2/SDL.h>

#include "../wr80vm_private.h"
#include "../../../Libraries/WR80EMU/wr80emu.h"

#define WIDTH  320
#define HEIGHT 200
#define PIXELS (WIDTH * HEIGHT)

static uint8_t *pixel_buffer = NULL;
static uint32_t *framebuffer = NULL; // 32-bit RGB framebuffer used by SDL texture
static int dib_stride = 0;

static volatile bool running = false;

static pthread_t scan_thread_id;
static pthread_t emu_thread_id;
//static pthread_mutex_t cs = PTHREAD_MUTEX_INITIALIZER;

static SDL_Window *g_window = NULL;
static SDL_Renderer *g_renderer = NULL;
static SDL_Texture *g_texture = NULL;

// palette in 0x00RRGGBB format stored as uint32_t
static uint32_t palette[256];

//EmuArgs args; // assumo que EmuArgs é declarado em wr80vm_private.h (você usava args no WinMain)

void init_palette(void)
{
    // igual à construção VGA do código original: 6x6x6 = 216 cores + grayscale
    int idx = 0;
    for (int r=0; r<6; r++) {
        for (int g=0; g<6; g++) {
            for (int b=0; b<6; b++) {
                uint8_t rr = r * 51;
                uint8_t gg = g * 51;
                uint8_t bb = b * 51;
                palette[idx++] = (rr << 16) | (gg << 8) | (bb);
            }
        }
    }
    for (; idx < 256; idx++) {
        int v = (idx - 216) * (255 / (256 - 216));
        if (v < 0) v = 0;
        if (v > 255) v = 255;
        palette[idx] = (v << 16) | (v << 8) | v;
    }
}

// wrapper que chama emulate_buffer
static void *emulate_thread(void *param)
{
    EmuArgs* a = (EmuArgs*)param;
    unsigned char *memory = NULL;
    char* binary = a->filename;

    int size = load_bin(binary, &memory);

    if (size != -1) {
        load_config("config.dat");

        if (devs.controller) {
            ctrl_run = true;
            CreateThread(conThread, controller);
        }

        emulate_buffer(memory, size, false);
        free(memory);
        memory = NULL;

        if (devs.romf[0]) {
            write_bin(devs.romf, rom, size);
            free(rom);
            rom = NULL;
        }

        if (devs.controller) {
            ctrl_run = false;
			CloseThread(conThread, 1000);
        }
    }

    running = false;
    // mande um evento SDL para acordar loop principal e sair
    SDL_Event ev;
    ev.type = SDL_QUIT;
    SDL_PushEvent(&ev);

    return NULL;
}

static void *scan_thread_func(void *arg)
{
    (void)arg;
    while (running) {
        // copiar linha a linha do pixel_buffer (8-bit indices) -> framebuffer (32-bit RGB)
        for (int y = 0; y < HEIGHT; ++y) {
			pthread_mutex_lock(&cs);
            uint8_t *src = pixel_buffer + y * WIDTH;
            uint32_t *dst = framebuffer + y * WIDTH;
            for (int x = 0; x < WIDTH; ++x) {
                uint8_t idx = src[x];
                dst[x] = palette[idx];
            }
			pthread_mutex_unlock(&cs);
        }

        // notifica o loop principal para repaint via evento custom
        SDL_Event ev;
        ev.type = SDL_USEREVENT;
        ev.user.code = 0;
        ev.user.data1 = NULL;
        ev.user.data2 = NULL;
        SDL_PushEvent(&ev);

        // ~60 FPS
        Sleep(16);
    }
    return NULL;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        const char* description = FILE_DESCRIPTION;
		const char* author = COMPANY_NAME;
		const char* version = VER_STRING;
		printf("\n********************************************************************************\n");
		printf("%s v%s\n", description, version);
		printf("Created by %s\n\n", author);
		printf("********************************************************************************\n");
        printf("Usage:\n");
        printf("wr80vm <file.bin> [-c | --console]\n");
        return 1;
    }

    // prepara args.filename (assumo que EmuArgs tem campo filename como char[])
    memset(&args, 0, sizeof(args));
    strncpy(args.filename, argv[1], sizeof(args.filename) - 1);

    // se tiver flag console, apenas informa (no Linux terminal já existe)
    if (argc >= 3) {
        if ((strcmp(argv[2], "-c") == 0) || (strcmp(argv[2], "--console") == 0)) {
            printf("Console ativado (modo texto) — saída visível no terminal.\n");
        }
    }

    // inicializa SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        fprintf(stderr, "SDL_Init erro: %s\n", SDL_GetError());
        return 1;
    }

    // cria janela redimensionável
    g_window = SDL_CreateWindow("WR80 Virtual Machine - Res: 320x200x256",
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                WIDTH * 2, HEIGHT * 2,
                                SDL_WINDOW_RESIZABLE);
    if (!g_window) {
        fprintf(stderr, "SDL_CreateWindow erro: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!g_renderer) {
        // fallback sem aceleracao
        g_renderer = SDL_CreateRenderer(g_window, -1, 0);
        if (!g_renderer) {
            fprintf(stderr, "SDL_CreateRenderer erro: %s\n", SDL_GetError());
            SDL_DestroyWindow(g_window);
            SDL_Quit();
            return 1;
        }
    }

    // textura 320x200 RGB888
    g_texture = SDL_CreateTexture(g_renderer,
                                  SDL_PIXELFORMAT_RGB888,
                                  SDL_TEXTUREACCESS_STREAMING,
                                  WIDTH, HEIGHT);
    if (!g_texture) {
        fprintf(stderr, "SDL_CreateTexture erro: %s\n", SDL_GetError());
        SDL_DestroyRenderer(g_renderer);
        SDL_DestroyWindow(g_window);
        SDL_Quit();
        return 1;
    }

    // aloca buffers
    pixel_buffer = (uint8_t*)malloc(PIXELS);
    if (!pixel_buffer) {
        fprintf(stderr, "malloc pixel_buffer falhou\n");
        SDL_DestroyTexture(g_texture);
        SDL_DestroyRenderer(g_renderer);
        SDL_DestroyWindow(g_window);
        SDL_Quit();
        return 1;
    }
    memset(pixel_buffer, 0, PIXELS);

    framebuffer = (uint32_t*)malloc(PIXELS * sizeof(uint32_t));
    if (!framebuffer) {
        fprintf(stderr, "malloc framebuffer falhou\n");
        free(pixel_buffer);
        SDL_DestroyTexture(g_texture);
        SDL_DestroyRenderer(g_renderer);
        SDL_DestroyWindow(g_window);
        SDL_Quit();
        return 1;
    }
    memset(framebuffer, 0, PIXELS * sizeof(uint32_t));

    dib_stride = WIDTH * 4;

    init_palette();

    // inicializa mutex
    pthread_mutex_init(&cs, NULL);

    // marca args para emulação (assim como no seu Windows original)
    args.buf = pixel_buffer;
    args.size = PIXELS;
    args.flag = true;

    running = true;

    // cria scan thread
    if (pthread_create(&scan_thread_id, NULL, scan_thread_func, NULL) != 0) {
        fprintf(stderr, "pthread_create scan_thread falhou\n");
        running = false;
    }

    // cria emulate thread
    if (pthread_create(&emu_thread_id, NULL, emulate_thread, &args) != 0) {
        fprintf(stderr, "pthread_create emulate_thread falhou\n");
        running = false;
    }

    // loop principal de eventos / render
    bool quit = false;
    while (!quit && running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
                running = false;
            } else if (e.type == SDL_USEREVENT) {
                // pedido de repaint vindo do scan thread: atualiza a textura com framebuffer
                // trava mutex só para segurança caso scan thread ainda esteja copiando
                pthread_mutex_lock(&cs);
                SDL_UpdateTexture(g_texture, NULL, framebuffer, WIDTH * sizeof(uint32_t));
                pthread_mutex_unlock(&cs);

                // pega tamanho da janela para escalar
                int win_w, win_h;
                SDL_GetWindowSize(g_window, &win_w, &win_h);

                SDL_Rect dst = {0, 0, win_w, win_h};

                SDL_RenderClear(g_renderer);
                SDL_RenderCopy(g_renderer, g_texture, NULL, &dst);
                SDL_RenderPresent(g_renderer);
            } else if (e.type == SDL_WINDOWEVENT) {
                if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    // na próxima USEREVENT a renderização usará o novo tamanho
                }
            }
            // outros eventos (teclado/mouse) podem ser tratados aqui
        }

        // pequena espera para não busy-loop quando nenhum evento
        SDL_Delay(5);
    }

    // shutdown
    running = false;

    // espera threads terminarem
    pthread_join(scan_thread_id, NULL);
    pthread_join(emu_thread_id, NULL);

    pthread_mutex_destroy(&cs);

    if (g_texture) SDL_DestroyTexture(g_texture);
    if (g_renderer) SDL_DestroyRenderer(g_renderer);
    if (g_window) SDL_DestroyWindow(g_window);

    if (framebuffer) free(framebuffer);
    if (pixel_buffer) free(pixel_buffer);

    SDL_Quit();

    return 0;
}
