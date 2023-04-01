all: main.o api.o utility.o abnf.o
	gcc $^ -o ourParser
	rm *.o

archive: 
	tar cvzf group5_parser.tar.gz src/* headers/* Makefile alltests/*.zip README.MD test_requests.sh

main.o: ./src/main.c
	gcc -Wall -Wextra -g -c $< -o $@

api.o: ./src/api.c ./headers/api.h
	gcc -Wall -Wextra -g -c $< -o $@

utility.o: ./src/utility.c ./headers/utility.h
	gcc -Wall -Wextra -g -c $< -o $@

abnf.o: ./src/abnf.c ./headers/abnf.h
	gcc -Wall -Wextra -g -c $< -o $@