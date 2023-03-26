#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

#ifndef ABNF_H
#include "../headers/abnf.h"
#define ABNF_H
#endif

#include "../headers/api.h"
#include "../headers/utility.h"

#define true 1
#define false 0

char HTTP_MESSAGE[]="HTTP-message";
char HEADER_FIELD[]="header-field";
char REQUEST_LINE[]="request-line";
char START_LINE[]="start-line";
char METHOD[]="method";
char TOKEN[]="token";
char TCHAR[]="tchar";
char SP[]="__sp";
char ALPHA[]="ALPHA";
char DIGIT[]="DIGIT";
char REQUEST_TARGET[]="Request-Target";
char ABSOLUTE_PATH[]="absolute-path";
char SEGMENT[]="segment";
char PCHAR[]="pchar";
char UNRESERVED[]="unreserved";
char PCT_ENCODED[]="pct-encoded";
char SUB_DELIMS[]="sub-delims";
char HEXDIG[]="hexdig";
char QUERY[]="query";
char HTTP_VERSION[]="HTTP-version";
char HTTP_NAME[]="HTTP-name";
char CRLF[]="__crlf";
char ICAR[]="__icar";
char FIELD_NAME[]="field-name";
char FIELD_VALUE[]="field-value";
char OWS[]="__ows";
char HTAB[]="__htab";
char FIELD_CONTENT[]="field-content";
char OBS_FOLD[]="obs-fold";
char FIELD_VCHAR[]="field-vchar";
char VCHAR[]="__vchar";
char OBS_TEXT[]="obs-text";
char MESSAGE_BODY[]="message-body";
char OCTET[]="OCTET";

void printNode(node* obj){
	printf("label:%s\n",obj->label);
	printf("debut:%s\n",obj->debut);
	printf("fin:%s\n",obj->fin);
}

void testMode(){
	char req[]="GET / HTTP/1.1\n";
	char *adr=&req[0];
	node* tete=malloc(sizeof(node));
	start_line(&adr,tete);
	print_tree(tete,1);
	exit(true);
}


int main(int argc, char const *argv[])
{
	if (argc==3) testMode();
	if (argc!=2){printf("Usage : ./parsername <file>\n");return false;}
	FILE* fic = NULL;
	fic=fopen(argv[1],"r");
	if(fic==NULL){printf("Erreur ouverture\n");return false;}

	//recopie memoire
	int requestSize=1;
	int i;
	int c=fgetc(fic);
	while(c!=EOF){requestSize++;c=fgetc(fic);}
	char Request[requestSize];
	fseek(fic,0,0);
	c=fgetc(fic);
	for (i = 0; i < requestSize-1; i++)
	{
		Request[i]=c;
		c=fgetc(fic);
	}
	Request[i]=0;
	printf("%s",Request);
	char *adr=Request;
	node* tete=malloc(sizeof(node));
	http_message(&adr,tete);
	print_tree(tete,1);
}
