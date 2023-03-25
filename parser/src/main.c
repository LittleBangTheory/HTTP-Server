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

char START_LINE[]="start-line";
char METHOD[]="method";
char TCHAR[]="tchar";
char SP[]="SP";
char ALPHA[]="ALPHA";
char DIGIT[]="DIGIT";
char REQUEST_TARGET[]="Request-Target";
char ABSOLUTE_PATH[]="absolute-path";
char SEGMENT[]="segment";
char PCHAR[]="pchar";
char UNRESERVED[]="unreserved";
char PCT_ENCODED[]="pct-encoded";
char SUB_DELIMS[]="sub-delims";
char HEXDIG[]="HEXDIG";
char QUERY[]="query";
char HTTP_VERSION[]="HTTP-version";
char HTTP_NAME[]="HTTP-name";
char CRLF[]="CRLF";

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
	printNode(tete);
	//print_tree(tete,1);
	exit(true);
}


int main(int argc, char const *argv[])
{
	if (argc==3) testMode();
	if (argc!=2){printf("Usage : ./parsername <file>\n");return false;}
	char* label[]={"start-line","method","tchar","SP","ALPHA","DIGIT"};
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


	


}
