#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>

#include "../libparser/api.h" 
#include "../headers/request.h"
#include "../headers/answer.h"

#define false 0 

/**
* \fn int call_parser(char* requete)
* \brief Permet de faire un appel au parseur.
* headersFound compte le nombre d'occurences positives à la recherche.
* PENSER A PURGE !
* \param requete Requete a traiter avec le parseur
* \param p Champ à rechercher
* \param headersFound Pointeur vers un entier modifié par la fonction et qui renvoie le nombre de nodes portant le label *p
* \param isValid Indique si la requête est valide syntaxiquement
* \return Liste chainée contenant toutes les occurences.
*/
_Token* call_parser(char* requete,char *p,int* headersFound,int* isValid)
{
	int res;
	*headersFound=0;
	_Token *r,*tok;
	r=NULL;
	if ((res=parseur(requete,strlen(requete)))) { 
		void *root=NULL;
		root=getRootTree(); 
		r=searchTree(root,p); 
		tok=r; 
		while (tok) {
			*headersFound=(*headersFound)+1;/*
			int l;
			char *s; 
			s=getElementValue(tok->node,&l); 
			printf("FOUND [%.*s]\n",l,s);*/
			tok=tok->next;
		}
		//purgeElement(&r);
		//purgeTree(root);
	}
	*isValid=res;
	return r;
}

char *getHeaderValue(_Token* headers, char* headerName){
    _Token* tmp=headers;
	int a;
	int found = 1;
	int len = strlen(headerName);
	char* res = NULL;
	while (tmp && found){
		found = strncmp(getElementValue(tmp->node,&a),headerName,len);
		//printf("Comparing %.*s and %.*s\n",len,headerName,len,getElementValue(tmp->node,&a));
		if (found==0){
			//printf("a=%d\n",a);
			res=malloc(sizeof(char)*a+sizeof(char));
			strncpy(res,getElementValue(tmp->node,&a),a);
			res[a]=0;
			//printf("REEEEEEEEES=%s\n",res);
		}
		tmp=tmp->next;
	}
	return res;
}

int existing(char* s,int longueur){
	char* path="../website"; //len=10
	int totalLen=10+longueur+1;
	char complete[totalLen];
	strcpy(complete,path);
	strncpy(&complete[10],s,longueur);
	complete[totalLen-1]=0;
	printf("\nEst-ce que %s existe ?\n",complete);
	return (access(complete,F_OK)+1);
}

int analyze(char* request,int clientID){
	int valeurRetour=0;
	int occurences,validSyntax;
	_Token* Tversion = call_parser(request,"HTTP_version",&occurences,&validSyntax);

	if (validSyntax==0){return 0;}

	_Token* Tmethod = call_parser(request,"method",&occurences,&validSyntax);
	_Token* allHeaders = call_parser(request,"header_field",&occurences,&validSyntax);
	_Token* Ttarget = call_parser(request,"request_target",&occurences,&validSyntax);

	int Lversion;
	char* version = getElementValue(Tversion->node,&Lversion); // PAS UN HEADER !

	int Lmethod;
	char* method = getElementValue(Tmethod->node,&Lmethod); // PAS UN HEADER !

	int Ltarget;
	char* request_target = getElementValue(Ttarget->node,&Ltarget); // PAS UN HEADER !

	char* host = getHeaderValue(allHeaders, "Host");
	char* connection = getHeaderValue(allHeaders, "Connection");
	char* accept_encoding = getHeaderValue(allHeaders, "Accept-Encoding");

	//DEBUG
	printf("# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #DEBUG\n");
	printf("version=%.*s\n",Lversion,version);
	printf("method=%.*s\n",Lmethod,method);
	printf("host=%s\n",host);
	printf("connection=%s\n",connection);
	printf("request-target=%.*s\n",Ltarget,request_target);
	printf("accept-encoding=%s\n",accept_encoding);
	printf("# # # # # # # # # ## # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #DEBUG END\n");

	char version2[9];
	strncpy(version2,version,8);
	version2[8]=0;

	if(host == NULL){
	send_version_code("400 Bad Request", version2, clientID);
	valeurRetour=-1;
	} else if(/*le client veut accéder à ../..*/0){
		send_version_code("403 Forbidden", version2, clientID);
		valeurRetour=-1;
	} else if(request_target!=NULL && !existing(request_target,Ltarget)){/*le fichier n'existe pas*/
		send_version_code("404 Not Found", version2, clientID);
		valeurRetour=-1;
	} else if(strncmp(version,"HTTP/1.0",8) && strncmp(version,"HTTP/1.1",8)){
		send_version_code("505 HTTP Version Not Supported", version2, clientID);
		valeurRetour=-1;
	} else if(strncmp(method,"GET",3) && strncmp(method,"HEAD",4) && strncmp(method,"POST",4)){
		send_version_code("501 Not Implemented", version2, clientID);
	} else if(/*le client veut accéder à un dossier*/0){
		/*TODO : à utiliser pour le sprint 4 (CGI), mais à implémenter maintenant pour nous avancer
		Aka : appeler la fonction qui va bien, mais ne pas la coder pour le moment
		*/
	} else {
		char* path="../website"; //len=10
		int totalLen=10+Ltarget+1;
		char complete[totalLen];
		strcpy(complete,path);
		strncpy(&complete[10],request_target,Ltarget);
		complete[totalLen-1]=0;
		send_version_code("200 OK", version2, clientID);
		send_type_length(complete, clientID);
		int content_length = send_type_length(complete,clientID);
		body(complete,clientID, content_length);
		valeurRetour=1;
	}

	purgeElement(&allHeaders);
	purgeElement(&Tmethod);
	purgeElement(&Ttarget);
	return valeurRetour;
}

/*
Cas non traités :
- le client veut utiliser une méthode non autorisée : redondant avec 501
*/

/*
HTTP request headers :

Host : required, used for multisite (for sprint 3 part 2)
Accept-Encoding : required but not used until sprint 4
Connection : required (for sprint 3 part 1)

Accept : unused
Accept-Language : unused
User-Agent : unused
*/

/*
Code de retour utiles :
200 OK
400 Bad Request
403 Forbidden (interdit, par exemple accès ../..)
404 Not Found (Si le fichier n'existe pas) 
405 Method Not Allowed (si par exemple méthode CONNECT alors qu'on fait que GET, HEAD, et POST
500 Internal Server Error
501 Not Implemented
505 HTTP Version Not Supported

Voir le wiki pour les détails
*/


