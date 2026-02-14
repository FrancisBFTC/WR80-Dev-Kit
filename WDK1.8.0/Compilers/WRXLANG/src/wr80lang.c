#include "astlib.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	char code[] =	"byte i;"
					"while (i){ i = i + 1;}";
					/*
					"word A = 0;"
					"byte B = 0;"
					"A = A + B;"
					*/
    compile(code);
    
	return 0;
}
