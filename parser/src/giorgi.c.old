#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>

#ifndef ABNF_H
#include "../headers/abnf.h"
#define ABNF_H
#endif

#include "../headers/api.h"
#include "../headers/utility.h"

#define true 1
#define false 0

char HTTP_MESSAGE[]="HTTP_message";
char HEADER_FIELD[]="header-field";
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
char FIELD_NAME[]="field-name";
char FIELD_VALUE[]="field-value";
char OWS[]="OWS";
char HTAB[]="__htab";
char FIELD_CONTENT[]="field_content";
char OBS_FOLD[]="obs-fold";
char FIELD_VCHAR[]="field_vchar";
char VCHAR[]="__vchar";
char OBS_TEXT[]="obs_text";
char MESSAGE_BODY[]="message_body";
char OCTET[]="octet";
char CONNECTION[]="Connection";
char HOST[]="Host";
char CONTENT_LENGTH[]="Content-Length";
char CONTENT_TYPE[]="Content-Type";
char COOKIE[]="Cookie";
char TRANSFER_ENCODING[]="Transfer-Encoding";
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

int main(int argc,char *argv[])
{
	int res,i,fi;
	char *p=NULL,*addr;

	
        struct stat st;

	if (argc < 2 ) { printf("Usage: httpparser <file> <search>\nAttention <search> is case sensitive\n");  return 0; }
	/* ouverture du fichier contenant la requête */ 
	if ((fi=open(argv[1],O_RDWR)) == -1) {
                perror("open");
                return false;
        }
        if (fstat(fi, &st) == -1)           /* To obtain file size */
                return false;
        if ((addr=mmap(NULL,st.st_size,PROT_WRITE,MAP_PRIVATE, fi, 0)) == NULL )
                return false;

	// This is a special HACK since identificateur in C can't have character '-'

	if (argc == 3 ) { 
		p=argv[2]; 	
		printf("searching for %s\n",p); 
		while (*p) { 
			if (*p=='-') { *p='_'; }
			p++; 
		}
		p=argv[2]; 	
	}
	// call parser and get results. 
	if (res=parseur(addr,st.st_size)) {
		_Token *r,*tok; 
		void *root=NULL;
		root=getRootTree();
		//print_tree((node*)root,0);
		r=searchTree(root,"__vchar"); 
		tok=r; 
		while (tok->node) {
			int l; 
			char *s; 
			s=getElementValue(tok->node,&l); 
			printf("FOUND [%.*s]\n",l,s);
			tok=tok->next; 
		}
		purgeElement(&r);
		purgeTree(root);
	}
	close(fi);
	return(res); 
}
