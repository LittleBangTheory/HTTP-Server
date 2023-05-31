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

char* send_php(char* filename, char* query_string, char* post_body, char* path, char* host, char* protocol){
	// Open file
	FILE* file = fopen(filename, "r");

	// Get file descriptor, source port and source port as string
    int fd;
	int source_port;
	char str_source_port[5];
	fd=createSocket(9000, &source_port); 
	//sendGetValue(fd); 
	sendBeginRequest(fd,10,FCGI_RESPONDER,0);
	sprintf(str_source_port,"%d",str_source_port);

	FCGI_Header h,c;
	addNameValuePair(&h, "HTTP_HOST", host);
	addNameValuePair(&h, "DOCUMENT_ROOT", path);
	addNameValuePair(&h, "REQUEST_URI", filename);
	addNameValuePair(&h, "REMOTE_ADDR", "127.0.0.1");
	addNameValuePair(&h, "REQUEST_METHOD", "GET");
	addNameValuePair(&h, "SCRIPT_NAME", filename);
	addNameValuePair(&h, "SERVER_PROTOCOL", protocol);
	addNameValuePair(&h, "SERVER_ADDR", "127.0.0.1");
	addNameValuePair(&h, "SERVER_NAME", "Projet HTTP");
	addNameValuePair(&h, "REMOTE_PORT", str_source_port);
	addNameValuePair(&h, "SERVER_PORT", str_source_port);
	addNameValuePair(&h, "GATEWAY_INTERFACE", "CGI/1.1");
	addNameValuePair(&h, "SCRIPT_FILENAME", "proxy:fcgi://127.0.0.1:9000//var/www/html/info.php");
	sendWebData2(&h,fd,FCGI_PARAMS,10,NULL,0);
	sendWebData(fd,FCGI_PARAMS,10,NULL,0); 
	
	char* answer_data;
	// Get answer

	sendStdin(fd,10,NULL,0);
	//sendData(fd,10,argv[1],strlen(argv[1])); 

	return answer_data;

}