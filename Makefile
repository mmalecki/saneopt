OBJS += src/saneopt.o

TEST_OBJS += test/test-saneopt

CFLAGS=-g -Wall -Iinclude

all: libsaneopt.a

libsaneopt.a: $(OBJS)
	ar rcs $@ $^

src/%.o: src/%.c
	gcc $(CFLAGS) -c $< -o $@

test/%: test/%.c
	gcc -L. -lsaneopt $(CFLAGS) $< -o $@

test: libsaneopt.a $(OBJS) $(TEST_OBJS)
	test/test-saneopt

clean:
	rm -f libsaneopt.a
	rm -f $(OBJS)
