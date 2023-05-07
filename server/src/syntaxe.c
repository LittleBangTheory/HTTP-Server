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
	// call parser and get results. 
	_Token *r,*tok;
	r=NULL;
	if ((res=parseur(requete,strlen(requete)))) { 
		void *root=NULL;
		root=getRootTree(); 
		r=searchTree(root,p); 
		tok=r; 
		while (tok) {
			*headersFound=(*headersFound)+1;
			/*int l;
			char *s; 
			s=getElementValue(tok->node,&l); 
			printf("FOUND [%.*s]\n",l,s);*/
			tok=tok->next;
		}
		//purgeElement(&r);
		purgeTree(root);
	}
	*isValid=res;
	return r;
}

char *getHeaderValue(message* request, char* headerName){
	int count,syntax,size;
    char* valueOfHeaders = NULL;
   	_Token* chained_results = call_parser(request->buf,headerName,&count,&syntax);
    _Token* tmp=chained_results;
	while (tmp)
	{
		valueOfHeaders=getElementValue(tmp->node,&size);
		// Interpretation here !
		printf("Trouvé : %s\n",valueOfHeaders);
		tmp=tmp->next;
	}
	purgeElement(&chained_results);

	return valueOfHeaders;
}

int analyze(message* request, char* filename, int clientID){
	char* version = getHeaderValue(request, "HTTP-Version");
	char* method = getHeaderValue(request, "Method");
	char* host = getHeaderValue(request, "Host");
	char* connection = getHeaderValue(request, "Connection");
	char* accept_encoding = getHeaderValue(request, "Accept-Encoding");

	if(host == NULL){
	send_version_code("400 Bad Request", version, clientID);
	} else if(/*le client veut accéder à ../..*/);
		send_version_code("403 Forbidden", version, clientID);
	} else if(/*le fichier n'existe pas*/){
		send_version_code("404 Not Found", version, clientID);
	} else if(version != "HTTP/1.0" && version != "HTTP/1.1"){
		send_version_code("505 HTTP Version Not Supported", version, clientID);
	} else if(!strcpy(method,"GET") && !strcpy(method,"HEAD") && !strcpy(method,"POST")){
		send_version_code("501 Not Implemented", version, clientID);
	} else if(/*le client veut accéder à un dossier*/){
		/*TODO : à utiliser pour le sprint 4 (CGI), mais à implémenter maintenant pour nous avancer
		Aka : appeler la fonction qui va bien, mais ne pas la coder pour le moment
		*/
	} else {
		send_version_code("200 OK", version, clientID);
		send_type_length(file, clientID);
	}
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


