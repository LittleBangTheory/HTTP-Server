all: server.exe

server.exe: main.o syntaxe.o answer.o
	gcc $^ -o $@ -L. -lrequest -lparser
	echo "Pensez à faire : export LD_LIBRARY_PATH=. !"

syntaxe.o: syntaxe.c
	gcc -Wall -g -o $@ -c $<

answer.o: answer.c
	gcc -Wall -g -o $@ -c $<

api.o: api.c
	gcc -Wall -g -o $@ -c $<

main.o: main.c
	gcc -Wall -g -o $@ -c $<

clean:
	rm *.o