shell:
	call configcli
	
.loop:
	call readkey
	call writechar
	jp .loop
	
include "kernel.inc"
