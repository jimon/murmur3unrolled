gcc -O3 -g --std=c11 -o benchmark.exe benchmark.c
objdump.exe -S -M intel -D benchmark.exe > benchmark.asm
./benchmark.exe