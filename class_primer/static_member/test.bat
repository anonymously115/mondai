echo off
gcc -Wall -Wextra -Werror -DNDEBUG main.c Pub.c Adult.c Customer.c parser.c util.c -o main 1>main.txt 2>&1
if exist main.exe (
	gcc -g -Wall -Wextra -Werror test.c util.c -o test 1>test.txt 2>&1
	if exist test.exe (
		.\test
		echo %ERRORLEVEL%
		del test.exe
		del test.txt
	) else (
		type test.txt
	)
	del main.exe
	del main.txt
) else (
	type main.txt
)
pause