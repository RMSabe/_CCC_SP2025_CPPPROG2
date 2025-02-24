globldef.o: globldef.c
	g++ globldef.c -c -o globldef.o

delay.o: delay.c
	g++ delay.c -c -o delay.o

cstrdef.o: cstrdef.c
	g++ cstrdef.c -c -o cstrdef.o

strdef.o: strdef.cpp
	g++ strdef.cpp -c -o strdef.o

lib_res: globldef.o delay.o cstrdef.o strdef.o

AlObj.o: AlObj.cpp
	g++ AlObj.cpp -c -o AlObj.o

main.o: main.cpp
	g++ main.cpp -c -o main.o

algorithm.elf: main.o AlObj.o lib_res
	g++ main.o globldef.o delay.o cstrdef.o strdef.o AlObj.o -o algorithm.elf

cleanup:
	rm globldef.o
	rm delay.o
	rm cstrdef.o
	rm strdef.o
	rm AlObj.o
	rm main.o

all: algorithm.elf cleanup


