main2: main2.o reverse.o palindrome.o
	cc main2.o reverse.o palindrome.o -o main2

main2.o: main2.c palindrome.h
	cc -c main2.c

reverse.o: reverse.c reverse.h
	cc -c reverse.c

palindrome.o: palindrome.c palindrome.h reverse.h
	cc -c palindrome.c
