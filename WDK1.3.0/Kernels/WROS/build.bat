echo off
cls

echo Montando os binarios...
cd src
wr80asm -m boot.asm -o ../bin/boot.bin -b -a
wr80asm -m shell.asm -o ../bin/shell.bin -b
wr80asm -m prog.asm -o ../bin/prog.bin -b

echo.
echo Gerando a imagem de disco... 
cd ../rom
wr80img --create wros.img

echo.
echo Formatando a imagem em WROFS...
cd ../
wr80img --format -s bin -o rom/wros.img -b boot.bin

echo.
echo Executando depurador...
::wr80dbg -d rom/wros.img -b
wr80emu -e rom/wros.img -b