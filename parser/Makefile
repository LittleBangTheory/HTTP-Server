all: main.o api.o utility.o abnf.o
	gcc $^ -o httpparser
	rm *.o

tar: 
	tar cvzf parser_chognot_dacosta_arnaud.tar.gz src/*.c headers/* Makefile alltests/*.zip README.md test_requests.sh

main.o: ./src/main.c
	gcc -Wall -Wextra -g -c $< -o $@

api.o: ./src/api.c ./headers/api.h
	gcc -Wall -Wextra -g -c $< -o $@

utility.o: ./src/utility.c ./headers/utility.h
	gcc -Wall -Wextra -g -c $< -o $@

abnf.o: ./src/abnf.c ./headers/abnf.h
	gcc -Wall -Wextra -g -c $< -o $@