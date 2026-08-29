gcc -fsanitize=address,undefined -g -Wall -Wextra -Werror CustomerTest.c Customer.c -o CustomerTest 1> CustomerTest.log 2>&1
if [ -e CustomerTest ]; then
	./CustomerTest
	rm -f CustomerTest
	rm -f CustomerTest.log
else
	cat CustomerTest.log >&2
fi
