gcc -fsanitize=address,undefined -g -Wall -Wextra -Wconversion -Wshadow -Wformat=2 -Wundef -Wstrict-prototypes -Wmissing-prototypes -Werror PubTest.c Pub.c CustomerStub.c -o PubTest 1> PubTest.log 2>&1
if [ -e PubTest ]; then
	./PubTest
	rm -f PubTest
	rm -f PubTest.log
else
	cat PubTest.log >&2
fi
