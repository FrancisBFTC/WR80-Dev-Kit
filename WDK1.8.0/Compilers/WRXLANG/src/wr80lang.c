#include "astlib.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	char code[] =
		"if(3 > 2) {"
		"	x = 1;"
		"	if(5 == 3)"
		"		y = 2;"
		"	else{"
		"		z = 3;"
		"	}"
		"}"
		"w = 4;";
    compile(code);
    
	return 0;
}
