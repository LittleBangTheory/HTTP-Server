#ifndef _API_
#define _API_
#include "../libparser/api.h"
#endif
_Token* call_parser(char* requete,char *p,int* headersFound,int* isValid);
char *getHeaderValue(_Token* headers, char* headerName,int* counter);
int analyze(char* request,int clientID);