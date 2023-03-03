simplegrammar.exe: abnf.o main.o
	@echo Compiling...
	gcc -g $^ -o $@
	@echo Deleting object files...
	rm $^

abnf.o: ./src/abnf.c ./headers/abnf.h
	gcc -Wall -c -g $< -o $@

main.o: ./src/main.c ./headers/abnf.h
	gcc -Wall -c -g $< -o $@