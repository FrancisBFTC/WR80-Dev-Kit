// arquivo: vga13h_window.c
// Compile no Dev-C++ com: gcc vga13h_window.c -lgdi32 -o vga13h_window.exe


#include <process.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "../wr80vm_private.h"
#include "../../wr80emu/src/wr80emu.h"

#define WIDTH  320
#define HEIGHT 200
#define PIXELS (WIDTH * HEIGHT)

static uint8_t *pixel_buffer = NULL; 
static HANDLE hThread = NULL;
static volatile bool running = false;

static HWND g_hwnd = NULL;
static HBITMAP hDib = NULL;
static uint8_t *dib_bits = NULL;
static int dib_stride = 0;

void init_console(void) {
        AllocConsole();

	system("cls");
    // Redireciona streams padrão
    FILE* f;
    f = freopen("CONOUT$", "w", stdout);
    f = freopen("CONOUT$", "w", stderr);
    //f = freopen("CONIN$",  "r", stdin);

    // Opcional: muda título
    SetConsoleTitle("WR80VM Console");
}

// wrapper que chama emulate_buffer
unsigned __stdcall emulate_thread(void* param) {
	EmuArgs* args = (EmuArgs*)param;
	
	unsigned char *memory = NULL;
	char* binary = args->filename;
	
    int size = load_bin(binary, &memory);
    
    if(size != -1){
    	load_config("config.dat");

		if(devs.controller){
			unsigned tid;
			ctrl_run = true;
    		conThread = (HANDLE)_beginthreadex(NULL, 0, controller, NULL, 0, &tid);
		}
		
    	emulate_buffer(memory, size, false);
    	free(memory); 			// liberar memória alocada
    	memory = NULL;
    	
    	if(devs.romf[0]){
			write_bin(devs.romf, rom, size);
			free(rom);
			rom = NULL;
		}

		if(devs.controller){
			ctrl_run = false;
			WaitForSingleObject(conThread, 1000);
    		CloseHandle(conThread);
		}
		
	}
    
    PostQuitMessage(0);
    running = false;
    return 0;
}

unsigned __stdcall scan_thread(void *arg)
{
    while (running) {
        for (int y = 0; y < HEIGHT; ++y) {
            EnterCriticalSection(&cs);
            uint8_t *src = pixel_buffer + y * WIDTH;
            uint8_t *dst = dib_bits + (y * dib_stride);
            memcpy(dst, src, WIDTH);
            LeaveCriticalSection(&cs);
        }
        // pede repintura da janela
        InvalidateRect(g_hwnd, NULL, FALSE);
        Sleep(16); // ~60 FPS
    }
    return 0;
}


void set_pixel(int x, int y, uint8_t color_index) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
    EnterCriticalSection(&cs);
    pixel_buffer[y * WIDTH + x] = color_index;
    LeaveCriticalSection(&cs);
}

// Thread exemplo que mexe pixels
unsigned __stdcall demo_thread(void *arg)
{
    int t = 0;
    while (running) {
        for (int x = 0; x < WIDTH; x += 5) {
            int y = (t + x/5) % HEIGHT;
            uint8_t color = (uint8_t)((x + t) & 255);
            set_pixel(x, y, color);
        }
        t++;
        Sleep(50);
    }
    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_PAINT:
			{
			    PAINTSTRUCT ps;
			    HDC hdc = BeginPaint(hWnd, &ps);
			
			    RECT rc;
			    GetClientRect(hWnd, &rc);
			    int win_w = rc.right - rc.left;
			    int win_h = rc.bottom - rc.top;
			
			    HDC memDC = CreateCompatibleDC(hdc);
			    SelectObject(memDC, hDib);
			
			    // Escala a imagem de 320x200 para preencher o cliente
			    SetStretchBltMode(hdc, COLORONCOLOR); // evita interpolação
			    StretchBlt(hdc, 0, 0, win_w, win_h,
			               memDC, 0, 0, WIDTH, HEIGHT,
			               SRCCOPY);
			
			    DeleteDC(memDC);
			    EndPaint(hWnd, &ps);
			    return 0;
			}

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nShow)
{
	int argc;
    LPWSTR *argv = CommandLineToArgvW(GetCommandLineW(), &argc);

    if (argv == NULL) {
        MessageBox(NULL, "Erro ao processar argumentos!", "Erro", MB_ICONERROR);
        return 1;
    }

    // Checa se tem pelo menos 2 argumentos (argv[0] é o wr80vm.exe)
    if (argc < 2) {
        MessageBox(NULL, "Uso: wr80vm.exe <arquivo>", "Erro", MB_ICONERROR);
        return 1;
    }
    
    // Converte wchar_t* para char*
    char filename[MAX_PATH];
    WideCharToMultiByte(CP_ACP, 0, argv[1], -1, filename, MAX_PATH, NULL, NULL);
    
    char console[20];
    WideCharToMultiByte(CP_ACP, 0, argv[2], -1, console, 20, NULL, NULL);
    
    strcpy(args.filename, filename);
    
    // Cria um console novo
    if(argv[2] != NULL){
    	if(strcmp(console, "-c") == 0 || strcmp(console, "--console") == 0){
    		init_console();
		}	
	}
    
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "VGA13hWindow";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);

    RECT rc = {0,0, WIDTH*2, HEIGHT*2}; // 3x maior que 320x200
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	int win_w = rc.right - rc.left;
	int win_h = rc.bottom - rc.top;
	
	HWND hwnd = CreateWindow(
	    wc.lpszClassName,
	    "WR80 Virtual Machine - Res: 320x200x256",
	    WS_OVERLAPPEDWINDOW,
	    CW_USEDEFAULT, CW_USEDEFAULT,
	    win_w, win_h,
	    NULL, NULL, hInstance, NULL);

    g_hwnd = hwnd;

    pixel_buffer = (uint8_t*)malloc(PIXELS);
    memset(pixel_buffer, 0, PIXELS);

    InitializeCriticalSection(&cs);

    // Cria paleta 256 cores (igual VGA básica: 6x6x6 + extras)
    BITMAPINFO *bmi = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));
    ZeroMemory(bmi, sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));
    bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi->bmiHeader.biWidth = WIDTH;
    bmi->bmiHeader.biHeight = -HEIGHT; 
    bmi->bmiHeader.biPlanes = 1;
    bmi->bmiHeader.biBitCount = 8;
    bmi->bmiHeader.biCompression = BI_RGB;

    // Paleta estilo VGA: 6x6x6 = 216 cores + gradações de cinza
    int idx = 0;
    for (int r=0; r<6; r++) {
        for (int g=0; g<6; g++) {
            for (int b=0; b<6; b++) {
                bmi->bmiColors[idx].rgbRed   = r * 51; 
                bmi->bmiColors[idx].rgbGreen = g * 51;
                bmi->bmiColors[idx].rgbBlue  = b * 51;
                idx++;
            }
        }
    }
    // completa o resto com escala de cinza
    for (; idx < 256; idx++) {
        int v = (idx-216) * (255 / (256-216));
        bmi->bmiColors[idx].rgbRed   = v;
        bmi->bmiColors[idx].rgbGreen = v;
        bmi->bmiColors[idx].rgbBlue  = v;
    }

    void *bits = NULL;
    hDib = CreateDIBSection(NULL, bmi, DIB_RGB_COLORS, &bits, NULL, 0);
    dib_bits = (uint8_t*)bits;
    dib_stride = ((WIDTH * 8 + 31) / 32) * 4;

    free(bmi);

    ShowWindow(hwnd, nShow);
    UpdateWindow(hwnd);

    running = true;
    unsigned tid;
    hThread = (HANDLE)_beginthreadex(NULL, 0, scan_thread, NULL, 0, &tid);
    //HANDLE hDemo = (HANDLE)_beginthreadex(NULL, 0, demo_thread, NULL, 0, &tid);
    
    // antes do loop de mensagens
	args.buf = pixel_buffer;
	args.size = PIXELS;        // 320*200
	args.flag = true;
    HANDLE hEmu = (HANDLE)_beginthreadex(NULL, 0, emulate_thread, &args, 0, &tid);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) && running) {
	    TranslateMessage(&msg);
	    DispatchMessage(&msg);
    }

    running = false;
    WaitForSingleObject(hThread, 1000);
    CloseHandle(hThread);
    //WaitForSingleObject(hDemo, 1000);
    //CloseHandle(hDemo);
    WaitForSingleObject(hEmu, 1000);
    CloseHandle(hEmu);

    DeleteCriticalSection(&cs);
    if (hDib) DeleteObject(hDib);
    if (pixel_buffer) free(pixel_buffer);

    return 0;
}


