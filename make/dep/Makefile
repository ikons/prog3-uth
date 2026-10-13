CC       = gcc
CFLAGS   = -g -Wall -Wextra
DEPFLAGS = -MMD -MP
PROGS    = main1 main2
OBJS     = main1.o main2.o reverse.o palindrome.o

all: $(PROGS)

main1: main1.o reverse.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@
main2: main2.o reverse.o palindrome.o
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

%.o: %.c
	$(CC) $(DEPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(PROGS) $(OBJS) $(OBJS:.o=.d)
.PHONY: all clean
-include $(OBJS:.o=.d)
