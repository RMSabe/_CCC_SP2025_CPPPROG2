"C:\MinGW64\bin\g++.exe" globldef.c -std=c++11 -c -o globldef.o
"C:\MinGW64\bin\g++.exe" cstrdef.c -std=c++11 -c -o cstrdef.o
"C:\MinGW64\bin\g++.exe" console.c -std=c++11 -c -o console.o
"C:\MinGW64\bin\g++.exe" strdef.cpp -std=c++11 -c -o strdef.o
"C:\MinGW64\bin\g++.exe" main.cpp -std=c++11 -c -o main.o
"C:\MinGW64\bin\g++.exe" AlObj.cpp -std=c++11 -c -o AlObj.o

"C:\MinGW64\bin\g++.exe" main.o globldef.o cstrdef.o console.o strdef.o AlObj.o -o algorithm.exe

del globldef.o
del cstrdef.o
del console.o
del strdef.o
del main.o
del AlObj.o
