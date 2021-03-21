OBJS += src/saneopt.o

TESTS += test/test-saneopt

CFLAGS=-g -Wall -Iinclude

all: libsaneopt.a

libsaneopt.a: $(OBJS)
	$(AR) rcs $@ $^

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

test/%: test/%.c libsaneopt.a
	$(CC) $(CFLAGS) -o $@ $?

test: libsaneopt.a $(TESTS)
	MallocScribble=1 test/test-saneopt

clean:
	rm -f libsaneopt.a
	rm -f $(OBJS)
	rm -f $(TESTS)

.PHONY: all test clean
