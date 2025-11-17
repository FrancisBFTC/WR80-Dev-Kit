#!/bin/bash

echo Installing Assembler...
cd WDK1.8.0/Assemblers/WR80ASM
make install

echo
echo Installing Image Builder...
cd ../WR80IMG
make install

echo
echo Installing Emulator...
cd ../../Emulators/WR80EMU
make install

echo
echo Installing Debugger...
cd ../WR80DBG
make install

echo
echo Installing Virtual Machine...
cd ../WR80VM
make install

echo
echo All tools installed!
