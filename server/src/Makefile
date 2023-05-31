all: server.exe

server.exe: main.o syntaxe.o answer.o fastcgi.o
	gcc $^ -L. -lrequest -lparser -lmagic -o $@
	@echo "Pensez à faire : export LD_LIBRARY_PATH=. !"

syntaxe.o: syntaxe.c
	gcc -Wall -g -c $< -lmagic -o $@ 

answer.o: answer.c
	gcc -Wall -Wextra -g -c $< -lmagic -o $@

api.o: api.c
	gcc -Wall -Wextra -g -c $< -lmagic -o $@

fastcgi.o: fastcgi.c
	gcc -Wall -Wextra -g -c $< -o $@

main.o: main.c
	gcc -Wall -Wextra -g -c $< -lmagic -o $@ 

clean:
	@rm *.o
	@echo "Done"

hosts:
	@sudo echo "127.0.0.2	hidden-site" >> /etc/hosts
	@sudo echo "127.0.0.2	master-site" >> /etc/hosts
	@sudo echo "127.0.0.2	www.toto.com" >> /etc/hosts
	@sudo echo "127.0.0.2	www.fake.com" >> /etc/hosts

debug: main.o syntaxe.o answer.o
	gcc $^ -L. -lrequest -lparser -lmagic -fsanitize=address -o $@
	@echo "Pensez à faire : export LD_LIBRARY_PATH=. !"