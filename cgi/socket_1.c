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

#include "fastcgi.h" 

// =========================================================================================================== // 
void writeSocket(int fd,FCGI_Header *h,unsigned int len)
{
	int w;

	h->contentLength=htons(h->contentLength); 
	h->paddingLength=htons(h->paddingLength); 
 

	while (len) {
	// try to write 	
		do {
			w = write(fd, h, len);
		} while (w == -1 && errno == EINTR);
	len-=w; 
	}
} 

void readSocket(int fd, FCGI_Header *h, unsigned int len){
	int r;
	while (len) {
	// try to read 	
		do {
			r = read(fd, h->contentData, len);
		} while (r == -1 && errno == EINTR);
	len-=r; 
	}
	h->contentLength=ntohs(h->contentLength); 
	h->paddingLength=ntohs(h->paddingLength); 
}

// =========================================================================================================== // 
void writeLen(int len, char **p) {
	if (len > 0x7F ) { 
		*((*p)++)=(len>>24)&0x7F; 
		*((*p)++)=(len>>16)&0xFF; 
		*((*p)++)=(len>>8)&0xFF; 
		*((*p)++)=(len)&0xFF; 
	} else *((*p)++)=(len)&0x7F;
}
	
// =========================================================================================================== // 
int addNameValuePair(FCGI_Header *h,char *name,char *value)
{
	char *p; 
	unsigned int nameLen=0,valueLen=0;

	if (name) nameLen=strlen(name); 
	if (value) valueLen=strlen(value);

	if ((valueLen > FASTCGIMAXNVPAIR) || (valueLen > FASTCGIMAXNVPAIR) ) return -1; 
	if ((h->contentLength+((nameLen>0x7F)?4:1)+((valueLen>0x7F)?4:1)) > FASTCGILENGTH ) return -1; 
	
	p=(h->contentData)+h->contentLength; 
	writeLen(nameLen,&p); 
	writeLen(valueLen,&p);
	strncpy(p,name,nameLen); 
	p+=nameLen; 
	if (value) strncpy(p,value,valueLen); 
	h->contentLength+=nameLen+((nameLen>0x7F)?4:1);
	h->contentLength+=valueLen+((valueLen>0x7F)?4:1);
}	 
// =========================================================================================================== // 		
	
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
// =========================================================================================================== // 
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
// =========================================================================================================== // 
void sendAbortRequest(int fd,unsigned short requestId) 
{
FCGI_Header h; 

	h.version=FCGI_VERSION_1; 
	h.type=htons(FCGI_ABORT_REQUEST); 
	h.requestId=requestId; 
	h.contentLength=0; 
	h.paddingLength=0; 
	writeSocket(fd,&h,FCGI_HEADER_SIZE+(h.contentLength)+(h.paddingLength)); 
}
#define sendStdin(fd,id,stdin,len) sendWebData(fd,FCGI_STDIN,id,stdin,len)
#define sendStdout(fd,id,stdout,len) sendWebData(fd,FCGI_STDOUT,id,stdout,len)
#define sendData(fd,id,data,len) sendWebData(fd,FCGI_DATA,id,data,len)
//============================================================================================================ // 

void sendWebData(int fd,unsigned char type,unsigned short requestId,char *data,unsigned int len) {
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

//============================================================================================================ // 

void getWebData(FCGI_Header* h, int fd){
	readSocket(fd, h, FCGI_HEADER_SIZE);
	char* data = calloc(h->contentLength, sizeof(char));
	memcpy(data, h->contentData, h->contentLength);
	printf("%s\n",data);
	readSocket(fd, h, h->contentLength);

}

//============================================================================================================ // 

void sendWebData2(FCGI_Header* h,int fd,unsigned char type,unsigned short requestId,char *data,unsigned int len) 
{

	if (len > FASTCGILENGTH) return ; 
	
	h->version=FCGI_VERSION_1; 
	h->type=type; 
	h->requestId=htons(requestId); 
	//h->contentLength=len; 
	h->paddingLength=0;
	//memcpy(h->contentData,data,len); 
	writeSocket(fd,h,FCGI_HEADER_SIZE+(h->contentLength)+(h->paddingLength));  
}

// =========================================================================================================== // 
static int createSocket(int port_dest, int* port_source)
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
	serv_addr.sin_port = htons(port_dest);

	if (connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("connect failed\n");
		return (-1);
	}

	struct sockaddr_in local_addr;
	socklen_t addr_len = sizeof(local_addr);
	if(getsockname(fd, (struct sockaddr *)&local_addr, &addr_len) < 0){
		perror("getsockname failed\n");
		return -1;
	}
	*port_source = ntohs(local_addr.sin_port);
	printf("Local port : %d\n", *port_source);

	return fd;
}
// =========================================================================================================== // 
int main(int argc,char *argv[])
{
	int fd;
	int port_source;
	char sport_source[5];
	fd=createSocket(9000, &port_source); 
	//sendGetValue(fd); 
	sendBeginRequest(fd,10,FCGI_RESPONDER,0);
	sprintf(sport_source,"%d",port_source);

	FCGI_Header h,c;
/*
	h.type=FCGI_PARAMS;
	h.
	writeSocket(fd,&h,FCGI_HEADER_SIZE+(h.contentLength)+(h.paddingLength));
*/
	addNameValuePair(&h, "HTTP_HOST", "127.0.0.1");
	addNameValuePair(&h, "DOCUMENT_ROOT", "/var/www/html");
	addNameValuePair(&h, "REQUEST_URI", "/info.php");
	addNameValuePair(&h, "REMOTE_ADDR", "127.0.0.1");
	addNameValuePair(&h, "REQUEST_METHOD", "GET");
	addNameValuePair(&h, "SCRIPT_NAME", "/info.php");
	addNameValuePair(&h, "SERVER_PROTOCOL", "HTTP/1.1");
	addNameValuePair(&h, "SERVER_ADDR", "127.0.0.1");
	addNameValuePair(&h, "SERVER_NAME", "127.0.0.1");
	addNameValuePair(&h, "REMOTE_PORT", sport_source);
	addNameValuePair(&h, "SERVER_PORT", sport_source);
	addNameValuePair(&h, "GATEWAY_INTERFACE", "CGI/1.1");
	addNameValuePair(&h, "SCRIPT_FILENAME", "proxy:fcgi://127.0.0.1:9000//var/www/html/info.php");
	sendWebData2(&h,fd,FCGI_PARAMS,10,NULL,0);
	sendWebData(fd,FCGI_PARAMS,10,NULL,0); 
	

	sendStdin(fd,10,NULL,0);
	getWebData(&c,fd);
	//sendData(fd,10,argv[1],strlen(argv[1])); 
}

