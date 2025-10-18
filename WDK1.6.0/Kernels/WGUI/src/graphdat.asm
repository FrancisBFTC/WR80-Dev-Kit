; Bits de mapeamento para incremento
define P0_P1 0x01
define P4_P5 0x25
define R1_R2 $4A
define R0_R1 0x41
define _P4   0x24
define _R0	 0x40
define _R1   0x41
define _R2	 0x52
define _R3	 $5B
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
define DARK_GRAY $F6
define DARK_GRAY2 $E6

; Limites e coordenadas de janelas
define RES_LOW	$40
define MAX_SIZE	$FA
define WIDTH	100
define HEIGHT	50
define POSX 	0
define POSY		0
define TOP_SIZE 10
define FONT_COLOR 0
define BACKCOLOR DARKGREEN

; Dados textuais
_string:
	db "Running WR80X Virtual Machine (WGUI)...",13,10,0
_taskerror:
	db "Task Error: Maximum table limit exceeded!",13,10,0
	
_titlew_1:
	db "win 1",0
_titlew_2:
	db "win 2",0
_titlew_3:
	db "win 3",0

_strwin:
	db "Uhulll",0
	
pos_x:
	db 0
pos_y:
	db 0
	