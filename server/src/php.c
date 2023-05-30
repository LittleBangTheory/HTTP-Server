#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../headers/fastcgi.h"

#define sendStdin(fd,id,stdin,len) sendWebData(fd,FCGI_STDIN,id,stdin,len)
#define sendData(fd,id,data,len) sendWebData(fd,FCGI_DATA,id,data,len)

/**
 * @brief Create a Socket object
 * 
 * @param port 
 * @return int 
 */
static int createSocket(int port)
{
	int fd;
	struct sockaddr_in serv_addr;
	int enable = 1;

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket creation failed\n");
		return (-1);
	}

	bzero(&serv_addr, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	inet_aton("127.0.0.1",&serv_addr.sin_addr);
	serv_addr.sin_port = htons(port);

	if (connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("connect failed\n");
		return (-1);
	}

	return fd;
}

/**
 * @brief Send a Begin Request object
 * 
 * @param fd 
 */
void sendGetValue(int fd) 
{
    FCGI_Header h; 

	h.version=FCGI_VERSION_1; 
	h.type=FCGI_GET_VALUES; 
	h.requestId=htons(FCGI_NULL_REQUEST_ID); 
	h.contentLength=0; 
	h.paddingLength=0; 
	addNameValuePair(&h,FCGI_MAX_CONNS,NULL); 
	addNameValuePair(&h,FCGI_MAX_REQS,NULL); 
	addNameValuePair(&h,FCGI_MPXS_CONNS,NULL); 
	writeSocket(fd,&h,FCGI_HEADER_SIZE+(h.contentLength)+(h.paddingLength)); 
}

/**
 * @brief Send a begin request object
 * 
 * @param fd 
 * @param requestId 
 * @param role 
 * @param flags 
 */
void sendBeginRequest(int fd,unsigned short requestId,unsigned short role,unsigned char flags) 
{
FCGI_Header h; 
FCGI_BeginRequestBody *begin; 

	h.version=FCGI_VERSION_1; 
	h.type=FCGI_BEGIN_REQUEST; 
	h.requestId=htons(requestId); 
	h.contentLength=sizeof(FCGI_BeginRequestBody); 
	h.paddingLength=0; 
	begin=(FCGI_BeginRequestBody *)&(h.contentData); 
	begin->role=htons(role); 
	begin->flags=flags; 
	writeSocket(fd,&h,FCGI_HEADER_SIZE+(h.contentLength)+(h.paddingLength)); 
}

/**
 * @brief Send the data
 * 
 * @param fd 
 * @param type 
 * @param requestId 
 * @param data 
 * @param len 
 */
void sendWebData(int fd,unsigned char type,unsigned short requestId,char *data,unsigned int len) 
{
    FCGI_Header h; 

	if (len > FASTCGILENGTH) return ; 
	
	h.version=FCGI_VERSION_1; 
	h.type=type; 
	h.requestId=htons(requestId); 
	h.contentLength=len; 
	h.paddingLength=0;
	memcpy(h.contentData,data,len); 
	writeSocket(fd,&h,FCGI_HEADER_SIZE+(h.contentLength)+(h.paddingLength));  
}

char* send_php(char* filename){
    FILE* file = fopen(filename, "r");
    // Get file length
    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Read file in buffer
    char* buffer = malloc(length + 1);
    fread(buffer, length, 1, file);
    fclose(file);

    // Key for the socket
    int fd; 
    // Create the socket
	fd=createSocket(9000);
    // Send a Begin Request object
	sendGetValue(fd); 
	sendBeginRequest(fd,10,FCGI_RESPONDER,FCGI_KEEP_CONN); 
    // Send the data
	sendStdin(fd,10,buffer,length); 
	sendData(fd,10,buffer,length); 

}