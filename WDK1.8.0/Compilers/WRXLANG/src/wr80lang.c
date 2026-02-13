#include "astlib.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	bool state;
	int rx = 0;
	
	AST *tree = parse("A = 2 <= 3");
	int result = gen(tree, &state, rx);

	return 0;
}
