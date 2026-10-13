CC       = gcc
CFLAGS   = -g -Wall -Wextra
CPPFLAGS = -DDEBUG
PROGS    = main1 main2

all: $(PROGS)

main1: main1.o reverse.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@
main2: main2.o reverse.o palindrome.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

main1.o: main1.c reverse.h
main2.o: main2.c palindrome.h
reverse.o: reverse.c reverse.h
palindrome.o: palindrome.c palindrome.h reverse.h

clean:
	rm -f $(PROGS) *.o
.PHONY: all clean
