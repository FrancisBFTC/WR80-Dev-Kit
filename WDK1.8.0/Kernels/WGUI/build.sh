#!/bin/bash

echo off
cls

echo Montando o Graph...
cd src
wr80asm -m graph.asm -o ../bin/graph.bin -b -a

echo
echo Gerando a Imagem WGUI... 
cd ../img
wr80img -c wgui.img -l 4095
cd ../
wr80img -s bin/graph.bin -o img/wgui.img

echo
echo Executando WGUI...
wr80vm img/wgui.img -c