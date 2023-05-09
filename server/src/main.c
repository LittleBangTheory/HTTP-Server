//GLOBAL INCLUDE SECTION
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "../headers/syntaxe.h"
#include "../headers/answer.h"

//INCLUDE SECTION FOR LIBREQUEST
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../headers/request.h"

//INCLUDE SECTION FOR LIBPARSER
#ifndef _API_
#define _API_
#include "../libparser/api.h"
#endif

//CODE SECTION
int main(int argc, char const *argv[])
{
    message* request;
    while (1)
    {

        if ((request=getRequest(7777)) == NULL ) return -1;

		printf("[REQUEST FROM CLIENT %d -> %s:%d]\n",request->clientId,inet_ntoa(request->clientAddress->sin_addr),htons(request->clientAddress->sin_port)); 
		printf("[REQUEST BODY :]\n%.*s\n",request->len,request->buf);

        int result_analyze = analyze(request->buf,request->clientId);
        if (result_analyze!=1)
        {
            writeDirectClient(request->clientId,"\r\n",2);
        }
        

		endWriteDirectClient(request->clientId); 
		requestShutdownSocket(request->clientId); 
	    freeRequest(request); 
    }
    
    return 0;
}
