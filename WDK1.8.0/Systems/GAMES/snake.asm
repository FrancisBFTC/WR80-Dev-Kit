include "../../Libraries/WR80ASM/wr80x.asm"

define KEY_W $77
define KEY_A $61
define KEY_S $73
define KEY_D $64

define SNAKE_STEP $01
define SNAKE_FOOD $1F
define COORD_X_Y  $0F

define _P1 $09 

jp Start

SnakeSteps:
	.times 256, 0
	
Start:
	; Limpa os dispositivos e configura memória de passos
	clr
	clr
	
	; Armazenamento de Teclas WASD
	std KEY_W
	ld r7
	
	std KEY_A
	ld r5
	
	std KEY_S
	ld r4
	
	std KEY_D
	ld r6
	
	; Valores de Incremento e Comida da cobrinha
	std SNAKE_STEP
	ld r2
	std SNAKE_FOOD
	ld r3
	
	; Coordenada inicial da cobrinha
	std COORD_X_Y
	out p4
	out p5
	st SNAKE_STEP
	out p6
	
	; Escreve o primeiro passo da coordenada inicial
	std _P1
	idc
	
	in p5
	out p2
	incr
	
	in p4
	out p2
	incr
	
InitFood:
	in p4
	pushd
	in p5
	pushd
	
	call CreateFood
	
	popd
	out p5
	popd
	out p4
	
	SnakeMoveL:
		in p4
		pushd
		in p5
		pushd
		in p1
		pushd
		
		stl r1
		out p1
		
		in p2
		out p5
		incr
		
		in p2
		out p4
		incr
		
		in p1
		ld r1
		
		cdr
		out p6
		
	IncSnakeL:
		popd
		out p1
		
		popd
		add r2
		and r3
		out p5
		out p2
		
		incr
		
		popd
		out p4
		out p2
		
		incr
		
		in p6
		jz SkipEatFoodL
		bt r3
		jz EatFoodL
		jp GameOver
		
	SkipEatFoodL:
		std SNAKE_STEP
		out p6
		
		in p3
		jz SnakeMoveL
		bt r4
		jz SnakeMoveD
		bt r6
		jz SnakeMoveR
		bt r7
		jz SnakeMoveU
		jp SnakeMoveL

EatFoodL:
	std SNAKE_STEP
	out p6
		
	in p4
	pushd
	in p5
	pushd
	in p1
	pushd
	
	call CreateFood
	out p7
	jp IncSnakeL
	
	SnakeMoveD:
		in p4
		pushd
		in p5
		pushd
		in p1
		pushd
		
		stl r1
		out p1
		
		in p2
		out p5
		incr
		
		in p2
		out p4
		incr
		
		in p1
		ld r1
		
		cdr
		out p6
		
	IncSnakeD:
		popd
		out p1
		
		popd
		out p5
		out p2
		
		incr
		
		popd
		add r2
		and r3
		out p4
		out p2
		
		incr
		
		in p6
		jz SkipEatFoodD
		bt r3
		jz EatFoodD
		jp GameOver
		
	SkipEatFoodD:
		std SNAKE_STEP
		out p6
		
		in p3
		jz SnakeMoveD
		bt r5
		jz SnakeMoveL
		bt r6
		jz SnakeMoveR
		bt r7
		jz SnakeMoveU
		jp SnakeMoveD
		
EatFoodD:
	std SNAKE_STEP
	out p6
	
	in p4
	pushd
	in p5
	pushd
	in p1
	pushd
	
	call CreateFood
	out p7
	jp IncSnakeD
		
	SnakeMoveR:
		in p4
		pushd
		in p5
		pushd
		in p1
		pushd
		
		stl r1
		out p1
		
		in p2
		out p5
		incr
		
		in p2
		out p4
		incr
		
		in p1
		ld r1
		
		cdr
		out p6
		
	IncSnakeR:
		popd
		out p1
		
		popd
		sub r2
		and r3
		out p5
		out p2
		
		incr
		
		popd
		out p4
		out p2
		
		incr
		
		in p6
		jz SkipEatFoodR
		bt r3
		jz EatFoodR
		jp GameOver
		
	SkipEatFoodR:
		std SNAKE_STEP
		out p6
		
		in p3
		jz SnakeMoveR
		bt r4
		jz SnakeMoveD
		bt r5
		jz SnakeMoveL
		bt r7
		jz SnakeMoveU
		jp SnakeMoveR
		
EatFoodR:
	std SNAKE_STEP
	out p6
	
	in p4
	pushd
	in p5
	pushd
	in p1
	pushd
	
	call CreateFood
	out p7
	jp IncSnakeR

	SnakeMoveU:
		in p4
		pushd
		in p5
		pushd
		in p1
		pushd
		
		stl r1
		out p1
		
		in p2
		out p5
		incr
		
		in p2
		out p4
		incr
		
		in p1
		ld r1
		
		cdr
		out p6
		
	IncSnakeU:
		popd
		out p1
		
		popd
		out p5
		out p2
		
		incr
		
		popd
		sub r2
		and r3
		out p4
		out p2
		
		incr
		
		in p6
		jz SkipEatFoodU
		bt r3
		jz EatFoodU
		jp GameOver
		
	SkipEatFoodU:
		std SNAKE_STEP
		out p6
		
		in p3
		jz SnakeMoveU
		bt r4
		jz SnakeMoveD
		bt r5
		jz SnakeMoveL
		bt r6
		jz SnakeMoveR
		jp SnakeMoveU
	
EatFoodU:
	std SNAKE_STEP
	out p6
	
	in p4
	pushd
	in p5
	pushd
	in p1
	pushd
	
	call CreateFood
	out p7
	jp IncSnakeU

GameOver:
	not r0
	ld r0
	out p6
	in p3
	jz GameOver
	jp Start

CreateFood:
	in p4
	pushd
	shl 1
	ld r0
	in p1
	sub r0
	ld r0
	in p4
	shr 4
	add r0
	add r2
	and r3
	out p4
	
	popd
	ld r0
	in p5
	shl 1
	add r0
	ld r0
	in p1
	add r0
	ld r0
	in p5
	shr 3
	add r0
	and r3
	out p5
	
	in p6
	bt r2
	jz CreateFood
	
	stl r3
	out p6
	
	cdr
	ld r0
ret
