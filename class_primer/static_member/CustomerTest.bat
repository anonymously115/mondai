echo off
gcc -g -Wall -Wextra -Werror -std=c99 CustomerTest.c Customer.c -o CustomerTest 1>CustomerTest.txt 2>&1
if exist CustomerTest.exe (
	.\CustomerTest
	echo %ERRORLEVEL%
	del CustomerTest.exe
	del CustomerTest.txt
) else (
	type CustomerTest.txt
)
pause