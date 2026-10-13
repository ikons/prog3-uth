main2: main2.o reverse.o palindrome.o
	cc main2.o reverse.o palindrome.o -o main2

main2.o: palindrome.h
reverse.o: reverse.h
palindrome.o: palindrome.h reverse.h
