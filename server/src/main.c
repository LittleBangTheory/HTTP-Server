//GLOBAL INCLUDE SECTION
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

//INCLUDE SECTION FOR LIBREQUEST
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../headers/request.h"

//INCLUDE SECTION FOR LIBPARSER

//DEFINITIONS
#define REPONSE "HTTP/1.0 200 OK\r\n"//reponse
#define REPONSE2 "server: nginx\r\ndate: Tue, 02 May 2023 17:14:16 GMT\r\ncontent-type: text/html; charset=UTF-8\r\ncontent-length: 1\r\ncache-control: no-cache, private\r\nvary: Accept-Encoding\r\n"//headers
#define REPONSE3 "\r\n<html><head><title>Bonjour</title></head><body><h1>Yop</h1></body></html>\r\n\r\n"//body

//CODE SECTION
int main(int argc, char const *argv[])
{
    message* request;
    while (1)
    {

        if ((request=getRequest(8080)) == NULL ) return -1;
		// Affichage de debug 
		printf("#########################################\nDemande recue depuis le client %d\n",request->clientId); 
		printf("Client [%d] [%s:%d]\n",request->clientId,inet_ntoa(request->clientAddress->sin_addr),htons(request->clientAddress->sin_port));
		printf("Contenu de la demande :\n%.*s\n\n",request->len,request->buf);
        // Passage au parser
        /* parser(char* request,int len,int id_client);
        Les sections devront répondre au fur et à mesure*/
		writeDirectClient(request->clientId,REPONSE,strlen(REPONSE));  // Exemple d'une réponse en plusieurs partie, il faut juste transmettre le clientId, le char* et sa taille
        writeDirectClient(request->clientId,REPONSE2,strlen(REPONSE2));
        writeDirectClient(request->clientId,REPONSE3,strlen(REPONSE3));

		endWriteDirectClient(request->clientId); 
		requestShutdownSocket(request->clientId); 
	    freeRequest(request); 
    }
    
    return 0;
}
