#ifndef _API_
#define _API_
#include "../libparser/api.h"
#endif

#define ERROR -1
#define OK 1
#define KEEP_ALIVE 2
_Token* call_parser(char* requete,char *p,int* headersFound,int* isValid,void* root);
char *getHeaderValue(_Token* headers, char* headerName,int* counter);
int analyze(char* request,int clientID);
int dot_removal(char** s, int length);
char* get_extension(char *filename);
char* percent_encoding(char* request, int freeRequest);