echo off
gcc -Wall -Wextra -Werror -std=c99 parser_test.c parser.c util.c -o parser_test 1>parser_test.txt 2>&1
if exist parser_test.exe (
	.\parser_test
	echo %ERRORLEVEL%
	del parser_test.exe
	del parser_test.txt
) else (
	type parser_test.txt
)
pause