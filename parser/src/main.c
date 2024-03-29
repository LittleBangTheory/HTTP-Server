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

char HTTP_MESSAGE[]="HTTP_message";
char HEADER_FIELD[]="header_field";
char REQUEST_LINE[]="request_line";
char START_LINE[]="start_line";
char METHOD[]="method";
char TOKEN[]="token";
char TCHAR[]="tchar";
char SP[]="__sp";
char ALPHA[]="__alpha";
char DIGIT[]="DIGIT";
char REQUEST_TARGET[]="Request_Target";
char ABSOLUTE_PATH[]="absolute_path";
char SEGMENT[]="segment";
char PCHAR[]="pchar";
char UNRESERVED[]="unreserved";
char PCT_ENCODED[]="pct_encoded";
char SUB_DELIMS[]="sub_delims";
char HEXDIG[]="hexdig";
char QUERY[]="query";
char HTTP_VERSION[]="HTTP_version";
char HTTP_NAME[]="HTTP_name";
char CRLF[]="__crlf";
char ICAR[]="__icar";
char FIELD_NAME[]="field_name";
char FIELD_VALUE[]="field_value";
char OWS[]="OWS";
char HTAB[]="__htab";
char FIELD_CONTENT[]="field_content";
char OBS_FOLD[]="obs_fold";
char FIELD_VCHAR[]="field_vchar";
char VCHAR[]="__vchar";
char OBS_TEXT[]="obs_text";
char MESSAGE_BODY[]="message_body";
char OCTET[]="octet";
char CONNECTION[]="Connection";
char HOST[]="Host";
char CONTENT_LENGTH[]="Content_Length";
char CONTENT_TYPE[]="Content_Type";
char COOKIE[]="Cookie";
char TRANSFER_ENCODING[]="Transfer_Encoding";
char EXPECT[]="Expect";
char CONNECTION_OPTION[]="connection_options";
char URI_HOST[]="uri_host";
char PORT[]="uri_port";
char HOST_LOWER[]= "host";
char IP_LITERAL[]="ip_literal";
char IPV4_ADDRESS[]="ipv4_address";
char REG_NAME[]="reg_name";
char IPV6_ADDRESS[]="ipv6_address";
char IPVFuture[]="IPvFuture";
char IPV4ADDRESS[]="IPv4";
char IPVFUTURE[]="IPvFuture";
char DEC_OCTET[]="dec_octet";
char H16[]="h16";
char LS32[]="ls32";
char REGNAME[]="reg_name";
char MEDIA_TYPE[]="media_type";
char TYPE[]="type";
char SUBTYPE[]="subtype";
char PARAMETER[]="parameter";
char QUOTED_STRING[]="quoted_string";
char QDTEXT[]="qdtext";
char QUOTED_PAIR[]="quoted_pair";
char COOKIE_STRING[]="cookie_string";
char COOKIE_PAIR[]="cookie_pair";
char COOKIE_NAME[]="cookie_name";
char COOKIE_VALUE[]="cookie_value";
char COOKIE_OCTET[]="cookie_octet";
char TRANSFER_ENCODING_HEADER[]="transfer_encoding_header";
char EXPECT_HEADER[]="expect_header";
char ISTRING[]="__istring";
char TRANSFER_CODING[]="transfer_coding";
char TRANSFER_EXTENSION[]="transfer_extension";
char TRANSFER_PARAMETER[]="transfer_parameter";

node** racine;

int main(int argc, char const *argv[])
{
	if (argc==1){printf("Usage : ./parsername <file> [<field to locate>]\n");return false;}
	FILE* fic = NULL;
	fic=fopen(argv[1],"r");
	if(fic==NULL){printf("Erreur ouverture\n");return false;}
	char *p=NULL;

	//recopie memoire
	int requestSize=1;
	int i;
	int c=fgetc(fic);
	while(c!=EOF){requestSize++;c=fgetc(fic);}
	unsigned char Request[requestSize];
	fseek(fic,0,0);
	c=fgetc(fic);
	for (i = 0; i < requestSize-1; i++)
	{
		Request[i]=c;
		c=fgetc(fic);
	}
	Request[i]=0;
	//printf("%s",Request);
	unsigned char *adr=Request;
	node* tete=malloc(sizeof(node));
	racine=&tete;
	http_message(&adr,tete); //Premiere fonction de parsing
	print_tree(tete,0);
	if(argc==3){
		p=argv[2]; 	
		printf("Searching for %s\n",p);
		while (*p) { 
			if (*p=='-') { *p='_'; }
			p++; 
		}
		p=argv[2]; 	
		_Token* tok = searchTree(tete,p);
		while (tok->node) {
			int l; 
			char *s; 
			s=getElementValue(tok->node,&l);
			printf("FOUND [%.*s]\n",l,s);
			tok=tok->next; 
		}
	}
	printf("\033[0;32m");
	printf("TOKEN_EXITED\n");
	printf("\033[0m"); 
}
