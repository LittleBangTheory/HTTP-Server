#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>

#ifndef _API_
#define _API_
#include "../libparser/api.h"
#endif
#include "../headers/request.h"
#include "../headers/answer.h"
#ifndef _SYNTAXE_
#define _SYNTAXE_
#include "../headers/syntaxe.h"
#endif

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
			*headersFound=(*headersFound)+1;
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
		if (found==0 && res==NULL){
			res=malloc(sizeof(char)*a+sizeof(char));
			strncpy(res,getElementValue(tmp->node,&a),a);
			res[a]=0;
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
* \param path ../http/hidden-site or ../http/master-site -> to be completed with the relative file path
* \return 0 si fichier inexistant, 1 sinon
*/
int existing(char* s,int longueur, char* path, int pathLen){
	// Get the total length of the path (path + file)
	int totalLen=pathLen+longueur;
	// Allocate the memory for the path
	char* complete_path = malloc(sizeof(char)*totalLen); 
	// Copy the path into the complete path
	strcpy(complete_path,path);
	// Concatenate the file name
	strncat(complete_path,s,longueur);
	// Check if the file exists
	int res = access(complete_path,F_OK)+1;
	// Free the memory
	free(complete_path);
	// Return the status of existence of the file
	return res;
}

/**
 * @brief remove the dot segments from the path
 * 
 * @param s initial path
 * @param length length of the path
 * @return int 
 */
int dot_removal(char** s, int length){
	// Allocate the memory for the buffer
	char* buffer = calloc(length,sizeof(char));
	// Index of the buffer
	int buffer_index = 0;
	// Add a '/' at the beginning of the buffer
	buffer[0]='/';
	buffer_index++;

	// Delimiter for the strtok function
	const char* delim = "/";
	// Get the first token
	char* token;
	token = strtok(*s,delim);

	// While there is a token and there is no space in this token
	while(token != NULL){
		// If the token is ".."
		if(strcmp(token,"..")==0){
			// If buffer isn't empty (excepted for the '/'), remove the last segment
			if(buffer_index>1){
				printf("old buffer : %.*s\n",buffer_index,buffer);
				buffer_index--;
				while(buffer[buffer_index-1]!='/'){
					buffer_index--;
				}
				printf("new buffer : %.*s\n",buffer_index,buffer);
			}
		}
		// If the token is "."
		else if(strcmp(token,".")==0){
			//remove the token
		}
		else{
			// Append the token value at the end of the buffer
			strcat(buffer,token);
			// Move the buffer index by the length of the added token
			buffer_index+=strlen(token);
			// Add a '/' at the end of the buffer
			buffer[buffer_index]='/';
			// Move the buffer index by 1
			buffer_index++;
		}

		// Get the next token
		token = strtok(NULL,delim);
	}

	// Remove the last '/' of the buffer if it isn't the only character
	if(buffer_index>1){
		buffer[buffer_index-1]=0;
	}
	
	// Allocate the memory for the initial string (same size as the buffer)
	*s = malloc(sizeof(char)*buffer_index);

	// Copy the buffer into the initial string
	strncpy(*s,buffer,buffer_index);

	// Free the memory of the buffer
	free(buffer);

	// Return the length of the initial string
	return buffer_index;
}

/**
* \fn char* percent_encoding(char* request)
* \brief Permet de transformer les %
* \param request target
* \return ptr vers la nouvelle target
*/
/*char* percent_encoding(char* request){
	char*res=malloc(sizeof(char)*(strlen(request)+1));
	int i = 0;
	while (request[i]!='%')
	{
		i++;
	}
	
}*/

/**
* \fn int analyze(char* request,int clientID)
* \brief Permet de tester la validité d'une requete
* \param request Requete entière
* \param clientID ID client
* \return -1 si requete invalide. 0 ou 1 sinon
*/
int analyze(char* request,int clientID){
	// Return value of 0 by default, 1 if the request is valid, -1 if the request is invalid
	int returnValue=0;
	// Declaration of the variables
	int occurences,validSyntax;
	// TODO : comment
	void* trees[4]={NULL,NULL,NULL,NULL};
	_Token* Tversion = call_parser(request,"HTTP_version",&occurences,&validSyntax,trees[0]);

	if (validSyntax==0){return ERROR;}

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

	// Get the headers
	int nbreHosts;
	char* connection = getHeaderValue(allHeaders, "Connection",&nbreHosts);
	char* accept_encoding = getHeaderValue(allHeaders, "Accept-Encoding",&nbreHosts);
	char* host = getHeaderValue(allHeaders, "Host",&nbreHosts);

	// Remove the dot segments from the path
	// Allocate a new string that only contains the target
	char* clean_target = calloc(target_length,sizeof(char));
	strncpy(clean_target,request_target,target_length);

	// Remove the dots
	target_length = dot_removal(&clean_target,target_length);
	printf("New request target is : %s, of length %d\n",clean_target, target_length);

	// If the request target is the server root, send the index.html file
	if (strncmp(clean_target,"/",target_length)==0)
	{
		free(clean_target);
		clean_target = calloc(12,sizeof(char));
		strncpy(clean_target,"/index.html",12);
		target_length = 11;
		printf("Definitive request target : %s\n",clean_target);
	}

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
	if((host == NULL && strncmp(version,"HTTP/1.0",8)!=0) || (host != NULL && strncmp(host, "hidden-site", 11)!=0 && strncmp(host, "master-site", 11)!=0 ) || (host != NULL && (nbreHosts!=1))){
		send_version_code("400 Bad Request", version2, clientID);
		content_length = send_type_length("../html/errors/400.html",clientID);
		body("../html/errors/400.html",clientID, content_length);
		returnValue=ERROR;

	// If the request target tries to reach a parent directory, send a 403 Forbidden
	} else {
		// If the client request for the Host "hidden-site", send it as host. Otherwise, use default host "master-site".
		if(host != NULL && strncmp(host, "hidden-site", 11) == 0){
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
		if(clean_target!=NULL && !existing(clean_target,target_length, path, pathLen)){/*le fichier n'existe pas*/
			send_version_code("404 Not Found", version2, clientID);
			content_length = send_type_length("../html/errors/404.html",clientID);
			body("../html/errors/404.html",clientID, content_length);
			returnValue=ERROR;
		// If the HTTP version is not supported, send a 505 HTTP Version Not Supported
		} else if(strncmp(version,"HTTP/1.0",8) && strncmp(version,"HTTP/1.1",8)){
			send_version_code("505 HTTP Version Not Supported", "HTTP/1.0", clientID);
			content_length = send_type_length("../html/errors/505.html",clientID);
			body("../html/errors/505.html",clientID, content_length);
			returnValue=ERROR;

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
			// Add the clean_target part at the end of the complete path
			strncat(complete,clean_target,target_length);
			// Send the headers
			send_version_code("200 OK", version2, clientID);

			//if the transfer encoding needs to be chunked, it will be treated here during sprint 4
			content_length = send_type_length(complete,clientID);

			// If the requested method is GET, send the body. Otherwise, juste the headers.
			if(strncmp(method,"GET",3)==0){
				body(complete,clientID, content_length);
			} else {
				// Not necessarly an error, but indicate to add a CRLF after the headers
				writeDirectClient(clientID,"\r\n",2);
			}

			returnValue=OK;
			printf("Connection : %s\n",connection);
			if (connection != NULL && strstr(connection,"keep-alive")!=NULL)
			{
				returnValue=KEEP_ALIVE;
				printf("KEEP ALIVE !\n");
			}
			

			free(complete);
		}
	}
	// Free the memory of the clean target
	printf("clean_target : %s\n",clean_target);
	free(clean_target);

	purgeElement(&allHeaders);
	purgeElement(&Tmethod);
	purgeElement(&Ttarget);
	for (size_t i = 0; i < 4; i++)
	{
		purgeTree(trees[i]);
	}

	printf("---------------------------------------------\n\n");
	
	return returnValue;
}