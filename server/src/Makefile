all: server.exe

server.exe: main.c
	gcc -Wall -g -o $@ $< -L. -lrequest
	echo "Pensez à faire : export LD_LIBRARY_PATH=. !"