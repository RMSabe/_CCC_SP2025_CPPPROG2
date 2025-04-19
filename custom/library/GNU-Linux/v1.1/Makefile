globldef.o: globldef.c
	g++ globldef.c -c -o globldef.o

cstrdef.o: cstrdef.c
	g++ cstrdef.c -c -o cstrdef.o

strdef.o: strdef.cpp
	g++ strdef.cpp -c -o strdef.o

lib_res: globldef.o cstrdef.o strdef.o

LibraryManager.o: LibraryManager.cpp
	g++ LibraryManager.cpp -c -o LibraryManager.o

main.o: main.cpp
	g++ main.cpp -c -o main.o

libmgr.elf: main.o LibraryManager.o lib_res
	g++ main.o globldef.o cstrdef.o strdef.o LibraryManager.o -o libmgr.elf

cleanup:
	rm globldef.o
	rm cstrdef.o
	rm strdef.o
	rm LibraryManager.o
	rm main.o

all: libmgr.elf cleanup


