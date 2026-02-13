#include "astlib.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	char code[] =
		"if(3 > 2)"
		"	x = 1;"
		"y = 2;"
		"z = 3;";
    compile(code);
    
	return 0;
}
