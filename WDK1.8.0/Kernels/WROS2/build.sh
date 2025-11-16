#!/bin/bash

echo off
clear

echo Montando os binarios...
cd src
wr80asm -m boot.asm -o ../bin/boot.bin -b -a
wr80asm -m shell.asm -o ../bin/shell.bin -b
wr80asm -m xprog.asm -o ../bin/xprog.bin -b

echo
echo Gerando a imagem de disco... 
cd ../rom
wr80img --create wros.img -l 4095

echo
echo Formatando a imagem em WROFS...
cd ../
wr80img --format -s bin -o rom/wros.img -b boot.bin

echo
echo Executando Emulador...
wr80emu -e rom/wros.img -b