include config.mk

BINARY?=saneopt

SRC+=src/saneopt.c

OBJS+=src/saneopt.o

CFLAGS+=-g -Wall -Iinclude

ifdef VERSION
	CFLAGS+=-DSANEOPT_VERSION="\"$(VERSION)\""
endif

all: binary library

clean:
	rm -rf src/*.o

cleanall: clean
	rm -rf $(BINARY)
	rm -rf *.a
	rm -rf *.dSYM

binary: $(SRC)
	gcc $(CFLAGS) $< src/main.c -o $(BINARY)

library: libsaneopt.a clean

libsaneopt.a: $(OBJS)
	ar rcs $@ $^

src/%.o: src/%.c
	gcc $(CFLAGS) -c $< -o $@

.PHONY: all clean cleanall binary library
