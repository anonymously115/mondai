echo off
gcc -g -Wall -Wextra -Wconversion -Wshadow -Wformat=2 -Wundef -Wstrict-prototypes -Wmissing-prototypes -Werror -std=c99 AdultTest.c Adult.c Customer.c -o AdultTest 1>AdultTest.txt 2>&1
if exist AdultTest.exe (
	.\AdultTest
	echo %ERRORLEVEL%
	del AdultTest.exe
	del AdultTest.txt
) else (
	type AdultTest.txt
)
pause