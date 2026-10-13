main2: main2.o reverse.o palindrome.o removevowel.o
	cc main2.o reverse.o palindrome.o removevowel.o -o main2

main2.o: main2.c palindrome.h removevowel.h
	cc -c main2.c

reverse.o: reverse.c reverse.h
	cc -c reverse.c

palindrome.o: palindrome.c palindrome.h reverse.h
	cc -c palindrome.c

removevowel.o: removevowel.c removevowel.h
	cc -c removevowel.c

.PHONY: clean
clean:
	rm -f main2 *.o
