main1: main1.o reverse.o
	cc main1.o reverse.o -o main1

main1.o: main1.c reverse.h
	cc -c main1.c

reverse.o: reverse.c reverse.h
	cc -c reverse.c
