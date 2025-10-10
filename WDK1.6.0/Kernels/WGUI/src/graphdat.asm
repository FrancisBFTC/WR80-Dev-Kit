; Bits de mapeamento para incremento
define P0_P1 0x01
define P4_P5 0x25
define _P4   0x24
define _R0	 0x40
define _R1   0x41
define _R2	 0x52
define _R4	 0x64
define _R5 	 $6D
define _DR   0x80
define _P1   0x01
define _R6   0x76

; Cores da memória de vídeo
define GREEN 	$1E
define BLUE		$2E
define WHITE	$FF
define DARKGREEN 0x30

; Limites e coordenadas de janelas
define RES_LOW	$40
define MAX_SIZE	$FA
define WIDTH	130
define HEIGHT	50
define POSX 	0
define POSY		0
define TOP_SIZE 10
define FONT_COLOR BLUE

; Dados textuais
_string:
	db "Executando Maquina Virtual...",0
	
_titlew_1:
	db "win thread 1",0
_titlew_2:
	db "win thread 2",0
_titlew_3:
	db "win thread 3",0
	