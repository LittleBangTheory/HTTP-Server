/**
 * @file syntaxe.c
 * @brief Contains the functions to analyze the request, and call answer.c
 */

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <magic.h>

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

#include "../headers/fastcgi.h"

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
_Token* call_parser(char* requete,char *p,int* headersFound,int* isValid,void** root)
{
	int res;
	*headersFound=0;
	_Token *r,*tok;
	r=NULL;
	if ((res=parseur(requete,strlen(requete)))) {
		*root=getRootTree(); 
		r=searchTree(*root,p); 
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
				buffer_index--;
				while(buffer[buffer_index-1]!='/'){
					buffer_index--;
				}
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
	free(*s);
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
* \example string = percent_encoding(string)
* \return ptr vers la nouvelle target
*/
char* percent_encoding(char* request, int freeRequest){
	// Y'a t-il un % ?
	int i = 0;
	while (request[i]!=0 && request[i]!='%') i++;
	if (request[i]==0) return request; //Si il n'y a pas de %, on renvoie la requete telle quelle
	
	//b sera la chaine sans les %
    char* b = malloc(sizeof(char)*strlen(request)+sizeof(char));
    i = 0;
    int j = 0;
    while(i<strlen(request)){
        while(request[i]!=0 && request[i]!='%'){
            b[j]=request[i];
            i++;
            j++;
        }
        i++;
        //Si on a un %XX :
        if((((48<=request[i] && request[i]<=57) || (65<=request[i] && request[i]<=70)) && ((48<=request[i+1] && request[i+1]<=57) || (65<=request[i+1] && request[i+1]<=70)))){
            char tmp=request[i+2];
            request[i+2]=0;
            long number = strtol(request+i,NULL,16);
            request[i+2]=tmp;
            b[j]=(char)number;
            j++;
            i+=2;
        }
        // Sinon
        else{
            b[j]=request[i-1];
            b[j+1]=request[i];
            b[j+2]=request[i+1];
            j+=3;
            i+=2;
        }
    }

	// Free the memory of the initial string if needed
	free(request);
	request = malloc(sizeof(char)*strlen(b));
	strcpy(request,b);
	free(b);
	return request;
}

/**
 * @brief Get the extension object
 * 
 * @param filename 
 * @return char* 
 */
char* get_extension(char* filename){
	magic_t magic_cookie;
    const char *mime_type;
	char* final_mime_type;

    magic_cookie = magic_open(MAGIC_MIME_TYPE);
    if (magic_cookie == NULL) {
        printf("unable to initialize magic library\n");
        return NULL;
    }

    if (magic_load(magic_cookie, NULL) != 0) {
        printf("cannot load magic database - %s\n", magic_error(magic_cookie));
        magic_close(magic_cookie);
        return NULL;
    }

    mime_type = magic_file(magic_cookie, filename);
    // If type is a plain/text, we need to check if it is a CSS or a XML file 
    if(strstr(mime_type,"text/") != NULL){
		char* extension = strrchr(filename, '.');
        // If the file is a CSS file
        if(!strcmp(extension,".css")){
            mime_type = "text/css";
        }
        // If the file is a XML file
        else if(!strcmp(extension,".xml")){
            mime_type = "text/xml";
        }
		else if(!strcmp(extension,".php")){
			mime_type = "text/x-php";
		}
    } 
    // If it is a text, add the encoding
    if(strstr(mime_type,"text") != NULL){
        // Add the encoding
        char* encoding = "; charset=utf-8";
        // Allocate the memory
        final_mime_type = malloc(sizeof(char)*(strlen(mime_type)+strlen(encoding)+1));
        // Concatenate the string
        sprintf(final_mime_type,"%s%s",mime_type,encoding);
    } else {
        final_mime_type = malloc(sizeof(char)*(strlen(mime_type)+1));
        sprintf(final_mime_type,"%s",mime_type);
    }

    magic_close(magic_cookie);

	return final_mime_type;
}

/**
 * @brief Check if the file is a static file (text, image, audio, video, application)
 * 
 * @param mime_type 
 * @return int 
 */
int isStatic(char* mime_type){
	if((strstr(mime_type,"text") != NULL && strstr(mime_type, "x-php") == NULL)|| strstr(mime_type, "image") != NULL || strstr(mime_type, "audio") != NULL || strstr(mime_type, "video") != NULL || strstr(mime_type, "application") != NULL){
		return 1;
	}
	return 0;
}

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
	void* trees[6]={NULL, NULL, NULL, NULL, NULL, NULL};
	_Token* Tversion = call_parser(request,"HTTP_version",&occurences,&validSyntax,&trees[0]);

	if (validSyntax==0){
		char* version3 = "HTTP/1.1";
		send_version_code("400 Bad Request", version3, clientID);
		int content_length2 = send_type_length("../html/errors/400.html",clientID, "text/html");
		send_body("../html/errors/400.html",clientID, content_length2);
		return ERROR;
		}

	_Token* Tmethod = call_parser(request,"method",&occurences,&validSyntax,&trees[1]);
	_Token* allHeaders = call_parser(request,"header_field",&occurences,&validSyntax,&trees[2]);
	_Token* Ttarget = call_parser(request,"request_target",&occurences,&validSyntax,&trees[3]);
	_Token* Tbody = call_parser(request,"message_body",&occurences,&validSyntax,&trees[4]);
	// TODO : search only in the request target field and not in the whole request
	_Token* Tquery = call_parser(request,"query",&occurences,&validSyntax,&trees[5]);

	// Get the version and its length
	int version_length;
	char* version = getElementValue(Tversion->node,&version_length); // PAS UN HEADER !

	// Get the method and its length
	int method_length;
	char* method = getElementValue(Tmethod->node,&method_length); // PAS UN HEADER !
	if(strncmp(method,"GET",3) == 0){
		method = "GET";
	} else if (strncmp(method, "POST", 4) == 0){
		method = "POST";
	} else if (strncmp(method, "HEAD", 4) == 0){
		method = "HEAD";
	}

	// Get the target and its length (query included)
	int target_query_length;
	char* request_target = getElementValue(Ttarget->node,&target_query_length); // PAS UN HEADER !

	// Get the body and its length
	int body_length;
	char* body = getElementValue(Tbody->node,&body_length); // PAS UN HEADER !

	// Get the query in the request target (or not) and its length
	int query_length=0;
	int isQueryPresent=0;
	char* query = NULL;
	int index_query=0;
	char* query_sanitized=NULL;
	while (index_query<target_query_length && !isQueryPresent)
	{
		if (request_target[index_query]=='?')
		{
			isQueryPresent=1;
		}
		index_query++;
	}
	if (isQueryPresent)
	{
		query = getElementValue(Tquery->node,&query_length); // PAS UN HEADER !
		//printf("QUERY DETECTED : %.*s\n",query_length,query);
		query_sanitized = calloc(query_length,sizeof(char));
		strncpy(query_sanitized,query,query_length);
		printf("QUERY DETECTED : %s\n",query_sanitized);
	}


	// Get the headers
	int nbreHosts, nbre_referer, nbrContentLength;
	char* connection = getHeaderValue(allHeaders, "Connection",&nbreHosts);
	char* accept_encoding = getHeaderValue(allHeaders, "Accept-Encoding",&nbreHosts);
	char* host = getHeaderValue(allHeaders, "Host",&nbreHosts);
	char* hostPTR = host;
	char* referer = getHeaderValue(allHeaders,"Referer",&nbre_referer);
	char* request_content_length = getHeaderValue(allHeaders, "Content-Length", &nbrContentLength);
	char* content_type = getHeaderValue(allHeaders, "Content-Type", &nbrContentLength);
	
	int index_CL=14;
	//Parsing Content-length
	if (request_content_length!=NULL){
		while(request_content_length[index_CL]==':' || request_content_length[index_CL]==' ') index_CL++;
	}

	// Search for the '?' character in the request target
	int target_length = target_query_length - query_length;
	if(isQueryPresent) target_length--;

	// Remove the dot segments from the path
	// Allocate a new string that only contains the target
	char* clean_target = calloc(target_length,sizeof(char));
	strncpy(clean_target,request_target,target_length);
	printf("clean_target : %s\n", clean_target);
	// Remove the dots
	target_length = dot_removal(&clean_target,target_length);

	// If the request target is the server root, send the index.html file
	if (strncmp(clean_target,"/",target_length)==0)
	{
		free(clean_target);
		clean_target = calloc(12,sizeof(char));
		strncpy(clean_target,"/index.html",12);
		target_length = 11;
	}
	else
	{
		clean_target = percent_encoding(clean_target, 1);
		target_length=strlen(clean_target);
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
	char* site_path;
	int pathLen;
	// Declare the content length variable
	int content_length;

	// If the client request for the Host "hidden-site", send it as host. Otherwise, use default host "master-site".
	if(host != NULL && strncmp(host, "hidden-site", 11) == 0){
		pathLen = 20;
		site_path = "/html/hidden_site";
	} else if (host != NULL && strncmp(host, "www.fake.com", 12) == 0){
		pathLen = 21;
		site_path = "/html/www.fake.com";
	} else if (host != NULL && strncmp(host, "www.toto.com", 12) == 0){
		pathLen = 21;
		site_path = "/html/www.toto.com";
	} else {
		pathLen = 20;
		site_path = "/html/master_site";
	}
	char pwd[1000];
	getcwd(pwd,1000);
	printf("PWD is : %s\n",pwd);
	char* src_position = strstr(pwd,"/src");
	unsigned int src_index = src_position-pwd;
	char* path = malloc(sizeof(char)*(pathLen+src_index+1));
	strncpy(path,pwd,src_index);
	strncpy(path+src_index,site_path,pathLen);
	printf("Path : %s\n",path);
	pathLen += src_index+1;
	
	// Count for the path length (\0 included in pathLen)
	int totalLen=pathLen+target_length;
	// Declare the complete path
	char* complete = malloc(sizeof(char)*totalLen); 
	// Copy the path part at the beginning of the complete path
	strcpy(complete,path);
	// Add the clean_target part at the end of the complete path
	strncat(complete,clean_target,target_length);

	printf("Complete path : %s\n",complete);

	char* mime_type = get_extension(complete);
	printf("Mime type : %s\n",mime_type);

	FILE* file = fopen(complete,"r");

	if(strncmp(method, "POST", 4) == 0 && isStatic(mime_type)){
		method = "GET";
		method_length -= 1;
	}

	// If the Host header is missing in a HTTP/1.1 request, send a 400 Bad Request
	if((host == NULL && strncmp(version,"HTTP/1.0",8)!=0) || (host != NULL && strncmp(host, "hidden-site", 11)!=0 && strncmp(host, "master-site", 11)!=0 && strncmp(host, "www.fake.com", 12) != 0 && strncmp(host, "www.toto.com", 12) != 0 ) || (host != NULL && (nbreHosts!=1)) || (body_length != 0 && request_content_length!=NULL && atoi(&request_content_length[index_CL]) != body_length)){
		send_version_code("400 Bad Request", version2, clientID);
			returnValue=ERROR;

		// If the method is not supported, send a 501 Not Implemented
		} else if(strncmp(method,"GET",3) && strncmp(method,"HEAD",4) && strncmp(method,"POST",4)){
			send_version_code("501 Not Implemented", version2, clientID);
		content_length = send_type_length("../html/errors/400.html",clientID, "text/html");
		send_body("../html/errors/400.html", clientID, content_length);
		returnValue=ERROR;
	// If the request target tries to reach a parent directory, send a 403 Forbidden
	} else {
		// If it is a POST request, process the data before sending the page
		int response_code = 200;
		// Declare the relative path to fetch the pages
		if(strncmp(method,"POST",4) && clean_target!=NULL && !existing(clean_target,target_length, path, pathLen)){/*le fichier n'existe pas*/
			send_version_code("404 Not Found", version2, clientID);
			content_length = send_type_length("../html/errors/404.html",clientID, "text/html");
			send_body("../html/errors/404.html", clientID, content_length);
			returnValue=ERROR;
		// If the HTTP version is not supported, send a 505 HTTP Version Not Supported
		} else if(strncmp(version,"HTTP/1.0",8) && strncmp(version,"HTTP/1.1",8)){
			send_version_code("505 HTTP Version Not Supported", "HTTP/1.0", clientID);
			content_length = send_type_length("../html/errors/505.html",clientID, "text/html");
			send_body("../html/errors/505.html", clientID, content_length);
			returnValue=ERROR;

		// If the method is not supported, send a 501 Not Implemented
		} else if(strncmp(method,"GET",3) && strncmp(method,"HEAD",4) && strncmp(method,"POST",4)){
			send_version_code("501 Not Implemented", version2, clientID);
			content_length = send_type_length("../html/errors/501.html",clientID, "text/html");
			send_body("../html/errors/501.html", clientID, content_length);
		// If we can't open the file or get the type, send a 500 Internal Server Error
		} else if(strncmp(method, "POST", 4) != 0 && ((file == NULL || mime_type == NULL) || response_code != 200)){
			send_version_code("500 Internal Server Error", version2, clientID);
			content_length = send_type_length("../html/errors/500.html",clientID, "text/html");
			send_body("../html/errors/500.html", clientID, content_length);
		// Else, the request is valid
		} else {
			// If we got in here, we can close the file to reopen it in the body() function
			fclose(file);
			// If the request is a POST, process the data
			if (strncmp(mime_type, "text/x-php", 10) == 0){
				int res = process_php(complete, query_sanitized, body, body_length, method, clientID, version2, content_type);
				if (!res){
					returnValue=ERROR;
				} else {
					returnValue=OK;
				}
			// Otherwise, it is a GET of HEAD request
			} else {
				// Send the 200 OK code (+ date and server header)
				send_version_code("200 OK", version2, clientID);

				//if the transfer encoding needs to be chunked, it will be treated here during sprint 4
				content_length = send_type_length(complete,clientID, mime_type);

				// If the requested method is GET, send the body. Otherwise, juste the headers.
				if(strncmp(method,"GET",3)==0){
					send_body(complete, clientID, content_length);
				} else {
					// It is a HEAD request, so we just complete the headers by the last CRLF 
					writeDirectClient(clientID,"\r\n",2);
				}
				returnValue=OK;
			}
			printf("Connection : %s\n",connection);
			if (connection != NULL && strstr(connection,"keep-alive")!=NULL)
			{
				returnValue=KEEP_ALIVE;
				printf("KEEP ALIVE !\n");
			}
		}
	}

	// Free the memory
	// Request
	free(clean_target);
	free(complete);
	free(mime_type);
	free(path);
	if (query_sanitized!=NULL)
	{
		free(query_sanitized);
	}
	

	free(connection);
	free(accept_encoding);
	free(hostPTR);
	free(referer);
	//free(content_type_data);

	// Allocated by the parser

	for (size_t i = 0; i < 6; i++)
	{
		purgeTree(trees[i]);
	}
	purgeElement(&allHeaders);
	purgeElement(&Tmethod);
	purgeElement(&Ttarget);
	purgeElement(&Tbody);
	purgeElement(&Tversion);
	purgeElement(&Tquery);
	query = NULL;
	query_length = 0;
	printf("---------------------------------------------\n\n");
	
	return returnValue;
}