DrawBackGround:
	; Configura memória de vídeo
	std 0x00
	out p4
	std 0x00
	out p5
	
	; Define tamanho máximo (64000 pixels)
	std MAX_SIZE
	ld r1
	
	; Configura mapeamento de incremento
	std P4_P5
	idc
	
.Loop:
	; Escreve cor verde em P4:P5 e incrementa
	std BLUE
	out p6
	incr
	
	; Cessa loop se atingiu o tamanho máximo
	in p4
	bt r1
	jz .End
	jp .Loop
	
	; Finaliza loop
.End:
	ret

SetIncP5:
	std P4_P5
	idc
ret

SetIncP4:
	std _P4
	idc
ret

SetIncR0:
	std _R0
	idc
ret

SetIncP1:
	std _P1
	idc
ret

SetVideoAddr:
	; Simulação 0x140 x 6 = 0x780 + 5 = 0x785
	std $01
	mul r5
	pushd		; DR = 1 x 6 (y) = 6
	
	std $40
	mul r5
	ld r5		; R5 = 0x40 x 6 = 0x80 -> R0 = 1 
	
	popd
	add r0
	ld r0		; R0 = 6 + 1 = 7
	
	stl r5		; DR = 0x80
	add r4		; DR = 0x80 + 5 (x) = 0x85
	ld r4
	jc .SumHigh
	jp .ConfigVideo
.SumHigh:
	call SetIncR0
	incr
	
.ConfigVideo:
	stl r0
	out p4
	stl r4
	out p5
	
	cdr
	ld r0
	std RES_LOW
	ld r3
ret

DrawSquare:
	push r1
	push r2
	push r3
	push r4
	push r5
	push r6
	
	call SetVideoAddr
	
	.Loop.UpLine:
		call SetIncP5
		stl r6
		out p6
		incr
		call SetIncR0
		incr
		stl r0
		bt r1
		jz .NextRight
		jp .Loop.UpLine
		
.NextRight:
	cdr
	ld r0
	
	.Loop.RightLine:
		stl r6
		out p6
		call SetIncP4
		incr
		in p5
		add r3
		out p5
		jc .IncP4
		jp .CheckLimitR
	.IncP4:
		incr
	.CheckLimitR:
		call SetIncR0
		incr
		stl r0
		bt r2
		jz .NextDown
		jp .Loop.RightLine
		
.NextDown:
	cdr
	ld r0
	
	.Loop.DownLine:
		call SetIncP5
		stl r6
		out p6
		decr
		call SetIncR0
		incr
		stl r0
		bt r1
		jz .NextLeft
		jp .Loop.DownLine
		
.NextLeft:
	cdr
	ld r0
	
	.Loop.LeftLine:
		stl r6
		out p6
		call SetIncP4
		decr
		in p5
		sub r3
		out p5
		jc .CheckLimitL
		decr
	.CheckLimitL:
		call SetIncR0
		incr
		stl r0
		bt r2
		jz .Done.Draw
		jp .Loop.LeftLine
	
.Done.Draw:
	pop r6
	pop r5
	pop r4
	pop r3
	pop r2
	pop r1
	
	ret
	
DrawSolidSquare:
	push r1
	push r2
	push r3
	push r4
	push r5
	push r6
	
	call SetVideoAddr
	
	push r0
	in p4
	pushd
	in p5
	pushd
	
	.Loop.Line:
		call SetIncP5
		stl r6
		out p6
		incr
		call SetIncR0
		incr
		stl r0
		bt r1
		jz .NextLine
		jp .Loop.Line
	.NextLine:
		popd
		out p5
		popd
		out p4
		pop r0
		call SetIncR0
		incr
		stl r0
		bt r2
		jz .Done.Solid
		push r0
		cdr
		ld r0
		
		call SetIncP4
		incr
		in p5
		add r3
		out p5
		jc .IncP4_1
		jp .BackLoop
	.IncP4_1:
		incr
		
	.BackLoop:
		in p4
		pushd
		in p5
		pushd
		jp .Loop.Line
		
.Done.Solid:
	pop r6
	pop r5
	pop r4
	pop r3
	pop r2
	pop r1

	ret


DrawWindow:
	push r4
	push r5
	
	call DrawSquare
	call PaintTop
	call CalcSubWindow
	call DrawSquare
	call PaintCenter
	
	pop r5
	pop r4
	
	std _R4
	idc
	incr
	
	std _R5
	idc
	incr
	
	call PrintString
ret

CalcSubWindow:
	std 2
	ld r0
	stl r1
	sub r0
	ld r1
	
	std TOP_SIZE
	pushd
	ld r0
	stl r2
	sub r0
	ld r2
	
	std _R4
	idc
	incr
	
	std _DR
	idc
	popd
	decr
	
	ld r0
	stl r5
	add r0
	ld r5
ret

PaintTop:
	push r1
	push r2
	push r3
	push r4
	push r5
	push r6
	
	std _R1
	idc
	decr
	
	std 2
	ld r0
	std TOP_SIZE
	sub r0
	ld r2
	
	std _R4
	idc
	incr
	
	std _R5
	idc
	incr
	
	std 0xF2
	ld r6
	call DrawSolidSquare
	
	pop r6
	pop r5
	pop r4
	pop r3
	pop r2
	pop r1
ret

PaintCenter:
	push r1
	push r2
	push r3
	push r4
	push r5
	push r6
	
	std _R1
	idc
	decr
	
	std _R2
	idc
	decr
	
	std _R4
	idc
	incr
	
	std _R5
	idc
	incr
	
	std 0xF6
	ld r6
	call DrawSolidSquare
	
	pop r6
	pop r5
	pop r4
	pop r3
	pop r2
	pop r1
ret

PrintChar:
	push r4
	push r5
	pushd
	call SetVideoAddr
	
	std 0x61
	ld r0
	popd
	bt r0
	pushd
	jc SetTextLowers
	
	std 0x41
	ld r0
	popd
	bt r0
	pushd
	jc SetTextUppers
	
	std 0x30
	ld r0
	popd
	bt r0
	pushd
	jc SetTextNumbers
	
	std 0x20
	ld r0
	popd
	bt r0
	pushd
	jz SetCharSpace
	
	std 0x2C
	ld r0
	popd
	bt r0
	pushd
	jz SetCharComma
	
	std 0x2E
	ld r0
	popd
	bt r0
	pushd
	jz SetCharDot
	jp .Invalid
	
SetCharSpace:
	std CharSpace::8
	out p0
	std CharSpace::0
	out p1
	popd
	jp .BeginRead2
SetCharComma:
	std CharComma::8
	out p0
	std CharComma::0
	out p1
	popd
	jp .BeginRead2
SetCharDot:
	std CharDot::8
	out p0
	std CharDot::0
	out p1
	popd
	jp .BeginRead2
SetTextNumbers:
	std TextNumbers::8
	out p0
	std TextNumbers::0
	out p1
	std 0x30
	jp .BeginRead
SetTextUppers:
	std TextUppers::8
	out p0
	std TextUppers::0
	out p1
	std 0x41
	jp .BeginRead
SetTextLowers:
	std TextLowers::8
	out p0
	std TextLowers::0
	out p1
	std 0x61
	
.BeginRead:
	ld r0
	popd
	sub r0
	ld r3
	std FONT_SIZE
	mul r3
	ld r1
	
	in p0
	add r0
	out p0
	
	in p1
	add r1
	out p1
	jc .IncP0
	jp .BeginRead2
.IncP0:
	std 0x00
	idc
	incr
	
	
.BeginRead2:
	std RES_LOW
	ld r3
	std FONT_SIZE
	ld r0
.BeginRead3:
	std 1
	shl BIT_SHIFT
	ld r2
	
	in p4
	pushd
	in p5
	pushd
	
	.ReadBitMap:
		in p2
		and r2
		jz .ShiftR2
		std FONT_COLOR
		out p6
	.ShiftR2:
		call SetIncP5
		incr
		stl r2
		shr 1
		ld r2
		cdr
		bt r2
		jz .NextByte
		jp .ReadBitMap
		
	.NextByte:
		call SetIncP1
		incr
		popd
		out p5
		popd
		out p4
		call SetIncR0
		decr
		jz .Done.PrintChar
		call SetIncP4
		incr
		in p5
		add r3
		out p5
		jc .IncP4_2
		jp .BeginRead3
	.IncP4_2:
		incr
		jp .BeginRead3
		
.Invalid:
	pop r5
	pop r4
	popd
	ret
	
.Done.PrintChar:
	pop r5
	pop r4
	ret
	
PrintString:
	in p2
	call CheckNull
	jz .done.PrtStr
	
	in p0
	pushd
	in p1
	pushd
		
	in p2
	call PrintChar
		
	popd
	out p1
	popd
	out p0
	
	call IncConf
	incr
	
	call MoveCursor
	
	jp PrintString
.done.PrtStr:
	ret
	
MoveCursor:
	std FONT_SIZE
	add r4
	ld r4
ret
	