echo off
gcc -g -Wall -Wextra -Werror -std=c99 util_test.c util.c -o util_test 1>util_test.txt 2>&1
if exist util_test.exe (
	.\util_test
	echo %ERRORLEVEL%
	del util_test.exe
	del util_test.txt
) else (
	type util_test.txt
)
pause