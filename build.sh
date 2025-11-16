#!/bin/bash

git restore *
git pull origin wdk1.8.0-compat
cd WDK1.8.0/Emulators/WR80DBG
make install
cd ../../Kernels/WROS
./build.sh
cd ../../../
