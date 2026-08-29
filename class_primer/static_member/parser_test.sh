gcc -fsanitize=address,undefined -g -Wall -Wextra -Werror parser_test.c parser.c util.c -o parser_test 1> parser_test.log 2>&1
if [ -e parser_test ]; then
	./parser_test
	rm -f parser_test
	rm -f parser_test.log
else
	cat parser_test.log >&2
fi
