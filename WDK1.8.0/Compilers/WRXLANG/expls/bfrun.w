/*
	WRX BrainFuck Program
	BASE = 0xE00, DATA_MEM = 0x200 (512 bytes)
*/

include "../../../Libraries/SYS8/WRX/iosys.w"
include "brainfuck.w"

word code = "[-]+++++>[-]++<[->[->.+<]++<]";
byte fucked = !BrainFuck(&code);

if(fucked)
	print("\nError: Invalid Program => Give up!");
else
	print("\nCongrats! You are fucked the same way! :)");