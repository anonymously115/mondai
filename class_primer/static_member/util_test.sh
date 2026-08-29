gcc -fsanitize=address,undefined -g -Wall -Wextra -Werror util_test.c util.c -o util_test 1> util_test.log 2>&1
if [ -e util_test ]; then
	./util_test
	rm -f util_test
	rm -f util_test.log
else
	cat util_test.log >&2
fi
