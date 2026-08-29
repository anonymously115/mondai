gcc -fsanitize=address,undefined -g -Wall -Wextra -Werror AdultTest.c Adult.c Customer.c -o AdultTest 1> AdultTest.log 2>&1
if [ -e AdultTest ]; then
	./AdultTest
	rm -f AdultTest
	rm -f AdultTest.log
else
	cat AdultTest.log >&2
fi
