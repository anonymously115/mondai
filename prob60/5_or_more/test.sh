gcc -fsanitize=address,undefined -g -Wall -Wextra -Werror -DNDEBUG main.c parser.c util.c -o main 1> main.log 2>&1
if [ -e main ]; then
	gcc -fsanitize=address,undefined -g -Wall -Wextra -Werror test.c util.c -o test 1> test.log 2>&1
	if [ -e test ]; then
		./test
		rm -f test
		rm -f test.log
	else
		cat test.log
	fi
	rm -f main
	rm -f main.log
else
	cat main.log
fi
