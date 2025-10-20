IntTable:
	dw Keyboard
	dw Mouse
	dw Timer
	dw Unknown

ProcIndex:
	db 0
	
ProcTable:
	db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	dw PID_END,  PID_END
	db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	dw PID_END,  PID_END
	db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	dw PID_END,  PID_END
	db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	dw PID_END,   PID_END
	db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	dw PID_END,   PID_END
	db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	dw PID_END,   PID_END