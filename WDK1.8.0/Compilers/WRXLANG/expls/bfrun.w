include "../../../Libraries/SYS8/WRX/iosys.w"
include "brainfuck.w"

//word code = "+++++[>.>.>.<<<]";
word code1 = "[-]+.A";
byte success = bf_run(&code1);

if(!success)
	print("\nError: invalid symbol!");