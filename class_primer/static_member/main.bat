echo off
gcc -g -Wall -Wextra -Wconversion -Wshadow -Wformat=2 -Wundef -Wstrict-prototypes -Wmissing-prototypes -Werror -std=c99 main.c -o main 1>main.txt 2>&1
if exist main.exe (
	for /l %%i in (0,1,9) do (
		.\main 1>nul
	)
	del main.exe
	del main.txt
) else (
	type main.txt
)
pause