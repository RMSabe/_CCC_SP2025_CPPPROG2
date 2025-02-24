#!/bin/bash

g++ globldef.c -c -o globldef.o
g++ delay.c -c -o delay.o
g++ cstrdef.c -c -o cstrdef.o
g++ strdef.cpp -c -o strdef.o
g++ main.cpp -c -o main.o
g++ AlObj.cpp -c -o AlObj.o

g++ main.o globldef.o delay.o cstrdef.o strdef.o AlObj.o -o algorithm.elf

rm globldef.o
rm delay.o
rm cstrdef.o
rm strdef.o
rm main.o
rm AlObj.o

