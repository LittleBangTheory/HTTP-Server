all: server.exe

server.exe: main.o syntaxe.o answer.o
	gcc $^ -L. -lrequest -lparser -lmagic -o $@
	@echo "Pensez à faire : export LD_LIBRARY_PATH=. !"

syntaxe.o: syntaxe.c
	gcc -Wall -g -c $< -lmagic -o $@ 

answer.o: answer.c
	gcc -Wall -Wextra -g -c $< -lmagic -o $@

api.o: api.c
	gcc -Wall -Wextra -g -c $< -lmagic -o $@

main.o: main.c
	gcc -Wall -Wextra -g -c $< -lmagic -o $@ 

clean:
	@rm *.o
	@echo "Done"

hosts:
	@sudo echo "127.0.0.2	hidden-site" >> /etc/hosts
	@sudo echo "127.0.0.3	master-site" >> /etc/hosts