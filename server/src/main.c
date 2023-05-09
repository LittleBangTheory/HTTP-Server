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
    // Declaration of the structure destinated to store the requests
    message* request;
    while (1)
    {
        // If we fail to open the connection of the port 7777, exit
        if ((request=getRequest(7777)) == NULL ) return -1;

        // Debug information -> print the client informations and the request 
		printf("[REQUEST FROM CLIENT %d -> %s:%d]\n",request->clientId,inet_ntoa(request->clientAddress->sin_addr),htons(request->clientAddress->sin_port)); 
		printf("[REQUEST BODY :]\n%.*s\n",request->len,request->buf);

        // Aanalyze the request and send the answer
        int result_analyze = analyze(request->buf,request->clientId);

        /* If we sent an error code, conclude the answer with a CRLF, because :
        * There is no body to send*
        * The functions that send the headers end with one CRLF
        * The end of the header part is supposed to be marked with two CRLF
        */
        if (result_analyze!=1)
        {
            writeDirectClient(request->clientId,"\r\n",2);
        }
        
        // Close the connection
		endWriteDirectClient(request->clientId); 
		requestShutdownSocket(request->clientId); 
	    freeRequest(request); 
    }
    
    return 0;
}
