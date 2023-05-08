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
_Token* call_parser(char* requete,char *p,int* headersFound,int* isValid,void* root)
{
	int res;
	*headersFound=0;
	_Token *r,*tok;
	r=NULL;
	if ((res=parseur(requete,strlen(requete)))) {
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

/**
* \fn char *getHeaderValue(_Token* headers, char* headerName)
* \brief Permet de renvoyer les valeurs des headers
* \param headers Liste chainée contenant TOUS les headers
* \param headerName Header à rechercher
* \return header recherché, NULL sinon
*/
char *getHeaderValue(_Token* headers, char* headerName,int* counter){
    _Token* tmp=headers;
	int a;
	int found = 1;
	int len = strlen(headerName);
	char* res = NULL;
	*counter=0;
	while (tmp){
		found = strncmp(getElementValue(tmp->node,&a),headerName,len);
		//printf("Comparing %.*s and %.*s\n",len,headerName,len,getElementValue(tmp->node,&a));
		if (found==0 && res==NULL){
			//printf("a=%d\n",a);
			res=malloc(sizeof(char)*a+sizeof(char));
			strncpy(res,getElementValue(tmp->node,&a),a);
			res[a]=0;
			//printf("REEEEEEEEES=%s\n",res);
		}
		if (found==0)
		{
			*counter=(*counter)+1;
		}
		
		tmp=tmp->next;
	}
	return res;
}
/**
* \fn int existing(char* s,int longueur)
* \brief Permet de tester l'existence d'un fichier
* \param s Le fichier en question dans le répertoire website /!\ ne pas ajouter ../website !
* \param longueur longueur du champ
* \return 0 si fichier inexistant, 1 sinon
*/
int existing(char* s,int longueur, char* path, int pathLen){
	int totalLen=pathLen+longueur;
	char* complete = malloc(sizeof(char)*totalLen); 
	strcpy(complete,path);
	strncat(complete,s,longueur);
	printf("\nEst-ce que %s existe ?\n",complete);
	int res = access(complete,F_OK)+1;
	if (res) printf("Oui\n");
	else printf("Oui\n");
	
	free(complete);
	return res;
}

/**
* \fn int analyze(char* request,int clientID)
* \brief Permet de tester la validité d'une requete
* \param request Requete entière
* \param clientID ID client
* \return -1 si requete invalide. 0 ou 1 sinon
*/
int analyze(char* request,int clientID){
	int valeurRetour=0;
	int occurences,validSyntax;
	void* trees[4]={NULL,NULL,NULL,NULL};
	_Token* Tversion = call_parser(request,"HTTP_version",&occurences,&validSyntax,trees[0]);

	if (validSyntax==0){return 0;}

	_Token* Tmethod = call_parser(request,"method",&occurences,&validSyntax,trees[1]);
	_Token* allHeaders = call_parser(request,"header_field",&occurences,&validSyntax,trees[2]);
	_Token* Ttarget = call_parser(request,"request_target",&occurences,&validSyntax,trees[3]);

	// Get the version and its length
	int version_length;
	char* version = getElementValue(Tversion->node,&version_length); // PAS UN HEADER !

	// Get the method and its length
	int method_length;
	char* method = getElementValue(Tmethod->node,&method_length); // PAS UN HEADER !

	// Get the target and its length
	int target_length;
	char* request_target = getElementValue(Ttarget->node,&target_length); // PAS UN HEADER !
	if (strncmp(request_target,"/",target_length)==0)
	{
		request_target="/index.html";
		target_length=11;
	}
	

	// Get the headers
	int nbreHosts;
	char* connection = getHeaderValue(allHeaders, "Connection",&nbreHosts);
	char* accept_encoding = getHeaderValue(allHeaders, "Accept-Encoding",&nbreHosts);
	char* host = getHeaderValue(allHeaders, "Host",&nbreHosts);

	/*
	//DEBUG
	printf("# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #DEBUG\n");
	printf("version=%.*s\n",version_length,version);
	printf("method=%.*s\n",method_length,method);
	printf("host=%s\n",host);
	printf("connection=%s\n",connection);
	printf("request-target=%.*s\n",target_length,request_target);
	printf("accept-encoding=%s\n",accept_encoding);
	printf("# # # # # # # # # ## # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #DEBUG END\n");
	*/
	// We only want the values
	int offset=5;
	if (host!=NULL){
		while(host[offset]==' ' ||host[offset]==':') offset++;
		host=&host[offset];
	}
	// Append the \0 to the version string
	char version2[9];
	strncpy(version2,version,8);
	version2[8]=0;

	// Declare the relative path to fetch the pages
	char* path;
	int pathLen;
	// Declare the content length variable
	int content_length;

	// If the Host header is missing in a HTTP/1.1 request, send a 400 Bad Request
	if((host == NULL && strncmp(version,"HTTP/1.0",8)!=0) || (strncmp(host, "hidden-site", 11)!=0 && strncmp(host, "master-site", 11)!=0 ) || (nbreHosts!=1)){
		send_version_code("400 Bad Request", version2, clientID);
		content_length = send_type_length("../html/errors/400.html",clientID);
		body("../html/errors/400.html",clientID, content_length);
		valeurRetour=-1;

	// If the request target tries to reach a parent directory, send a 403 Forbidden
	} else {
		// If the client request for the Host "hidden-site", send it as host. Otherwise, use default host "master-site".
		if(strncmp(host, "hidden-site", 11) == 0){
			pathLen = 20;
			//path = malloc(sizeof(char)*pathLen);
			//strcpy(path,"../html/hidden_site");
			path = "../html/hidden_site";
		} else {
			pathLen = 20;
			//path = malloc(sizeof(char)*pathLen); 
			//strcpy(path,"../html/master_site");
			path = "../html/master_site";
		} 
		// Declare the relative path to fetch the pages
		if(strstr(request_target,"..") != NULL){
			printf("request_target contient ..\n");
			send_version_code("403 Forbidden", version2, clientID);
			content_length = send_type_length("../html/errors/404.html",clientID);
			body("../html/errors/403.html",clientID, content_length);
			valeurRetour=-1;

		// If the requested file does not exist, send a 404 Not Found
		} else if(request_target!=NULL && !existing(request_target,target_length, path, pathLen)){/*le fichier n'existe pas*/
			send_version_code("404 Not Found", version2, clientID);
			content_length = send_type_length("../html/errors/404.html",clientID);
			body("../html/errors/404.html",clientID, content_length);
			valeurRetour=-1;

		// If the HTTP version is not supported, send a 505 HTTP Version Not Supported
		} else if(strncmp(version,"HTTP/1.0",8) && strncmp(version,"HTTP/1.1",8)){
			send_version_code("505 HTTP Version Not Supported", "HTTP/1.0", clientID);
			content_length = send_type_length("../html/errors/505.html",clientID);
			body("../html/errors/505.html",clientID, content_length);
			valeurRetour=-1;

		// If the method is not supported, send a 501 Not Implemented
		} else if(strncmp(method,"GET",3) && strncmp(method,"HEAD",4) && strncmp(method,"POST",4)){
			send_version_code("501 Not Implemented", version2, clientID);
			content_length = send_type_length("../html/errors/501.html",clientID);
			body("../html/errors/501.html",clientID, content_length);

		// Else, the request is valid
		} else {
			// Count for the path length (\0 included in pathLen)
			int totalLen=pathLen+target_length;
			// Declare the complete path
			char* complete = malloc(sizeof(char)*totalLen); 
			// Copy the path part at the beginning of the complete path
			strcpy(complete,path);
			// Add the request_target part at the end of the complete path
			strncat(complete,request_target,target_length);
			// Send the headers
			send_version_code("200 OK", version2, clientID);
			content_length = send_type_length(complete,clientID);

			// If the requested method is GET, send the body. Otherwise, juste the headers.
			if(strncmp(method,"GET",3)==0){
				body(complete,clientID, content_length);
			}

			valeurRetour=1;

			free(complete);
		}
	}

	purgeElement(&allHeaders);
	purgeElement(&Tmethod);
	purgeElement(&Ttarget);
	for (size_t i = 0; i < 4; i++)
	{
		purgeTree(trees[i]);
	}
	
	return valeurRetour;
}

/*
Cas non traités :
- le client veut utiliser une méthode non autorisée : redondant avec 501
- le client veut accéder à un dossier : à utiliser pour le sprint 4 (CGI), mais à implémenter maintenant pour nous avancer. Aka : appeler la fonction qui va bien, mais ne pas la coder pour le moment
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


