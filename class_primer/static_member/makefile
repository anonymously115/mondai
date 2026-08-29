CC = gcc
CFLAGS = -Wall -Wextra -Werror
TARGET = main
# SRCS = $(filter-out %test.c, $(wildcard *.c))
SRCS = main.c
SRCS += parser.c
SRCS += util.c
SRCS += Adult.c
SRCS += Customer.c
SRCS += Pub.c
OBJS = $(SRCS:.c=.o)

all: release

check:
	mkdir -p cppcheck
	cppcheck \
	--enable=all \
	--check-level=exhaustive \
	--inconclusive \
	--force \
	--max-ctu-depth=4 \
	--platform=unix64 \
	--library=std.cfg \
	--std=c11 \
	--cppcheck-build-dir=cppcheck \
	-j $$(nproc) \
	-I . \
	-DNDEBUG \
	--inline-suppr \
	--suppress=missingIncludeSystem \
	--suppress=checkersReport \
	$(SRCS) $(SRCS:.c=.h)

debug: CFLAGS = -fsanitize=address,undefined -g -Wall -Wextra -Werror
debug: $(TARGET)

release: CFLAGS = -Wall -Wextra -Werror -DNDEBUG
release: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

