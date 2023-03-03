simplegrammar.exe: abnf.o main.o utility.o
	@echo Compiling...
	gcc -g $^ -o $@
	@echo Deleting object files...
	rm $^
	@echo COMPILED

abnf.o: ./src/abnf.c ./headers/abnf.h
	gcc -Wall -c -g $< -o $@

main.o: ./src/main.c ./headers/abnf.h ./headers/utility.h
	gcc -Wall -c -g $< -o $@

utility.o: ./src/utility.c ./headers/utility.h
	gcc -Wall -c -g $< -o $@