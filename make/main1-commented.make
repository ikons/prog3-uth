# κανόνας 1
main1: main1.o reverse.o
	cc main1.o reverse.o -o main1
# κανόνας 2
main1.o: main1.c reverse.h
	cc -c main1.c
# κανόνας 3
reverse.o: reverse.c reverse.h
	cc -c reverse.c
