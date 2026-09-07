gcc -fsanitize=address,undefined -g -Wall -Wextra -Werror ArrayTest.c Array.c -o ArrayTest 1> ArrayTest.log 2>&1
if [ -e ArrayTest ]; then
	./ArrayTest
	rm -f ArrayTest
	rm -f ArrayTest.log
else
	cat ArrayTest.log >&2
fi
