@echo off
color a
echo Compilando..
windres icono\resource.rc icono\resource.o
gcc IMEIGen.c icono\resource.o -o IMEIgen_win32.exe -m32
echo Fin :)
pause