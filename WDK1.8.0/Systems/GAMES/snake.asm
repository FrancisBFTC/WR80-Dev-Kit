Start:
	;st TableInterrupt::8
	;out p0
	;st TableInterrupt::4
	;shl 4
	;st TableInterrupt::0
	;out p1
	;ei
	
	clr
	clr
	st SnakeSteps::8
	out p0
	
	cdr
	st 7
	shl 4
	st 3
	ld r4
	
	st 6
	shl 4
	st 1
	ld r5
	
	st 6
	shl 4
	st 4
	ld r6
	
	st 7
	shl 4
	st 7
	ld r7
	
	cdr
	st 1
	ld r2
	shl 4
	st $F
	ld r3
	
	cdr
	st 15
	out p4
	out p5
	
	st 1
	out p6
	
	in p5
	out p2
	in p1
	add r2
	out p1
	in p4
	out p2
	
	in p1
	add r2
	out p1
	
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
		
		cdr
		or r1
		out p1
		
		in p2
		out p5
		
		in p1
		add r2
		out p1
		
		in p2
		out p4
		
		in p1
		add r2
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
		
		in p1
		add r2
		out p1
		
		popd
		out p4
		out p2
		
		in p1
		add r2
		out p1
	
		in p6
		bt r0
		jz SkipEatFoodL
		bt r3
		jz EatFoodL
		st $0A
		ld r1
		jp GameOver
		
	SkipEatFoodL:
		cdr
		st 1
		out p6
		
		in p3
		bt r4
		jz SnakeMoveD
		bt r6
		jz SnakeMoveR
		bt r7
		jz SnakeMoveU
		jp SnakeMoveL

EatFoodL:
	cdr
	st 1
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
		
		cdr
		or r1
		out p1
		
		in p2
		out p5
		
		in p1
		add r2
		out p1
		
		in p2
		out p4
		
		in p1
		add r2
		ld r1
		
		cdr
		out p6
		
	IncSnakeD:
		popd
		out p1
		
		popd
		out p5
		out p2
		
		in p1
		add r2
		out p1
		
		popd
		add r2
		and r3
		out p4
		out p2
		
		in p1
		add r2
		out p1
		
		in p6
		bt r0
		jz SkipEatFoodD
		bt r3
		jz EatFoodD
		st $0A
		ld r1
		jp GameOver
		
	SkipEatFoodD:
		cdr
		st 1
		out p6
		
		in p3
		bt r5
		jz SnakeMoveL
		bt r6
		jz SnakeMoveR
		bt r7
		jz SnakeMoveU
		jp SnakeMoveD
		
EatFoodD:
	cdr
	st 1
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
		
		cdr
		or r1
		out p1
		
		in p2
		out p5
		
		in p1
		add r2
		out p1
		
		in p2
		out p4
		
		in p1
		add r2
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
		
		in p1
		add r2
		out p1
		
		popd
		out p4
		out p2
		
		in p1
		add r2
		out p1
		
		in p6
		bt r0
		jz SkipEatFoodR
		bt r3
		jz EatFoodR
		st $0A
		ld r1
		jp GameOver
		
	SkipEatFoodR:
		cdr
		st 1
		out p6
		
		in p3
		bt r4
		jz SnakeMoveD
		bt r5
		jz SnakeMoveL
		bt r7
		jz SnakeMoveU
		jp SnakeMoveR
		
EatFoodR:
	cdr
	st 1
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
		
		cdr
		or r1
		out p1
		
		in p2
		out p5
		
		in p1
		add r2
		out p1
		
		in p2
		out p4
		
		in p1
		add r2
		ld r1
		
		cdr
		out p6
		
	IncSnakeU:
		popd
		out p1
		
		popd
		out p5
		out p2
		
		in p1
		add r2
		out p1
		
		popd
		sub r2
		and r3
		out p4
		out p2
		
		in p1
		add r2
		out p1
		
		in p6
		bt r0
		jz SkipEatFoodU
		bt r3
		jz EatFoodU
		st $0A
		ld r1
		jp GameOver
		
	SkipEatFoodU:
		cdr
		st 1
		out p6
		
		in p3
		bt r4
		jz SnakeMoveD
		bt r5
		jz SnakeMoveL
		bt r6
		jz SnakeMoveR
		jp SnakeMoveU
	
EatFoodU:
	cdr
	st 1
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
	bt r1
	jz Start
	jp GameOver

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
	
	cdr
	or r3
	out p6
	
	cdr
	ld r0
ret
	
org 0x300
SnakeSteps:
	
	
; Implementação Futura	
;TableInterrupt:
;	dw Keyboard, 0,0,0
	
;Keyboard:
;	di
;	pushd	
;	popd
;	ei
;iret


