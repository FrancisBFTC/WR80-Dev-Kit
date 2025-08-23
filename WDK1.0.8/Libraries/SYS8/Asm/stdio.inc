define PARAM1 4
define PARAM2 5
define PARAM3 6

; Printf - Print the String in the TTY output
; In:
;	BP + 4 = High part string address
;	BP + 5 = Low part string address
; 	BP + 6 = number from 0 to 9
printf:
	pushb
	pushs
	popb

	clr
	st 1
	ld %r1
	st PARAM1
	abp
	out %p0
	cdr
	st PARAM2
	abp
	out %p1


.print:
	in %p2
	out %p3
	bt %r0
	jz .done

	in %p1
	add %r1
	out %p1
	jp .print

.done:
	cdr
	st 3
	shl 4
	ld %r1
	cdr
	st PARAM3
	abp
	add %r1
	out %p3

	popb
ret

; Puts - Print the String with line break
; In:
;	BP + 4 = High part string address
;	BP + 5 = Low part string address	
puts:
	pushb
	pushs
	popb
	
	clr
	st 1
	ld %r1
	st PARAM1
	abp
	out %p0
	cdr
	st PARAM2
	abp
	out %p1

.puts_loop:
	in %p2
	out %p3
	bt %r0
	jz .puts_done

	in %p1
	add %r1
	out %p1
	jp .puts_loop

.puts_done:
	cdr
	st $0D
	out %p3
	
	popb
ret

; Getchar - get the first char from keyboard waiting Enter
; In: None
; Out:
;	DR = ASCII Character
getchar:
	pushb
	pushs
	popb
	
	cdr
	st $0D
	ld %r0
	cdr
	ld %r1
	cdr
	st 1
	ld %r3
	
.getc_loop:
	in %p3
	bt %r0
	jz .getc_done
	bt %r1
	jz .getc_loop
	
	pushd
	cdr
	bt %r3
	popd
	jz .getc_loop
	
	ld %r2
	cdr
	ld %r3
	jp .getc_loop
	
.getc_done:
	cdr
	or %r2
	
	popb
ret

; Putchar - Print the char in the TTY output
; In:
;	BP + 4 = Char to print
putchar:
	pushb
	pushs
	popb
	
	cdr
	st PARAM1
	abp
	out %p3
	
	popb
ret


