::gcc -std=c99 -c src/assembler.c -o obj/assembler.o
::ar rcs obj/libassembler.a obj/assembler.o
::gcc -std=c99 src/wr80lang.c obj/libassembler.a -o bin/wrx.exe

gcc -std=c99 -c src/assembler.c -o obj/assembler.o
gcc -std=c99 -DBUILD_ASSEMBLER_DLL -shared -o lib/assembler.dll src/assembler.c -Wl,--out-implib,lib/libassembler.a
install.bat

