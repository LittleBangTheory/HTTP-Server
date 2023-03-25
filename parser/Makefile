all: main.o api.o utility.o abnf.o
	gcc $^ -o ourParser
	rm *.o

main.o: ./src/main.c
	gcc -Wall -g -c $< -o $@

api.o: ./src/api.c ./headers/api.h
	gcc -Wall -g -c $< -o $@

utility.o: ./src/utility.c ./headers/utility.h
	gcc -Wall -g -c $< -o $@

abnf.o: ./src/abnf.c ./headers/abnf.h
	gcc -Wall -g -c $< -o $@