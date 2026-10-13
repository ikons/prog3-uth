SUPPORT_DIR = /home/whatever
CC       = gcc
CFALGS   = -g -Wall -DDEBUG
CPPFLAGS = -I $(SUPPORT_DIR)
LDFLAGS  = -L $(SUPPORT_DIR)
LDLIBS   =

all: main1 main2

main1: main1.o reverse.o
	$(CC) $(CFALGS) $(CPPFLAGS) main1.o reverse.o -o main1 $(LDLIBS)
main1.o: main1.c reverse.h
	$(CC) $(CFALGS) $(CPPFLAGS) -c main1.c
reverse.o: reverse.c reverse.h
	$(CC) $(CFALGS) $(CPPFLAGS) -c reverse.c
main2: main2.o reverse.o palindrome.o
	$(CC) $(CFALGS) $(CPPFLAGS) main2.o reverse.o palindrome.o -o main2
main2.o: palindrome.h
reverse.o: reverse.h
palindrome.o: palindrome.h reverse.h
lint:
	lint $(CPPFLAGS) *.c
clean:
	rm palindrome reverse *.o
