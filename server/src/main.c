//GLOBAL INCLUDE SECTION
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

//INCLUDE SECTION FOR LIBREQUEST
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "request.h"

//INCLUDE SECTION FOR LIBPARSER

//CODE SECTION
int main(int argc, char const *argv[])
{
    while (1)
    {
        message* request;
        message* response;
        if ((request=getRequest(8080)) == NULL ) return -1;
		// Affichage de debug 
		printf("#########################################\nDemande recue depuis le client %d\n",requete->clientId); 
		printf("Client [%d] [%s:%d]\n",requete->clientId,inet_ntoa(requete->clientAddress->sin_addr),htons(requete->clientAddress->sin_port));
		printf("Contenu de la demande %.*s\n\n",requete->len,requete->buf);
        // Passage au parser
        /* parser(char* request,int len,int id_client);

        Les sections devront répondre au fur et à mesure
		writeDirectClient(requete->clientId,REPONSE1,strlen(REPONSE1)); 
		writeDirectClient(requete->clientId,REPONSE2,strlen(REPONSE2)); */

		endWriteDirectClient(requete->clientId); 
		requestShutdownSocket(requete->clientId); 
	    freeRequest(request); 
    }
    
    return 0;
}
