/** \file answer.c
 * This file will execute 2 or 3 types of request :
 *  - GET
 *  - HEAD
 *  - POST
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/* 
Code de retour utiles :
200 OK
400 Bad Request
403 Forbidden
404 Not Found
405 Method Not Allowed
500 Internal Server Error
501 Not Implemented
505 HTTP Version Not Supported

Specification : 
Tableau headers[][] contient les headers de la requête au format [nom][valeur]
On #DEFINE les codes de retour dans un .h
Si un header est inutilisé, on le met à NULL
*/

/* Headers to send :
[] Server : nom du serveur 
[] Content-Language : La langue utilisée. (exemple : fr-FR)
[] Date : Date et heure de la réponse
[] Content-Type : le format utilisé (exemple : text/html; charset=utf-8)
[] Content-Length : Taille de la représentation (en octet) 

[] Transfer-Encoding : chunked, compress, deflate, gzip, identity
    chunked est utilisé pour les réponses de type "streaming", obligatoire en l'absence de Content-Length
*/

/*
Useless headers :
* Last-Modified : Date et heure de la dernière modification du document
* Connection : close ou keep-alive
* Location : URL de redirection
* Content-Encoding : gzip, deflate, compress, identity
* Content-Disposition : inline ou attachment
* Content-Range : bytes 0-499/1234
* Accept-Ranges : bytes
* ETag : "1234"
* Expires : Date et heure d'expiration
* Cache-Control : no-cache, no-store, must-revalidate, private
* Pragma : no-cache
* Set-Cookie : nom=valeur; expires=Date; path=chemin; domain=domaine; secure
*/

#include "../headers/answer.h"

/**
 * @brief Send the version of the HTTP protocol, the code of the return, and the date and server headers
 * @param code Return code of the request. Char* to allow to send "200" or "200 OK" for example
 * @param version Version of the HTTP protocol, format "HTTP/1.1"
 * @return int 
 */
int send_version_code(char* code, char* version, int clientID){
    /* SEND VERSION + CODE */
    // Define the string. +4 to account for one space, one \r, one \n and one \0
    char* string = malloc(sizeof(char)*(strlen(version)+strlen(code)+4));
    if (string == NULL){
        perror("malloc");
        return -1;
    }

    // Concatenate the string
    sprintf(string, "%s %s %s", version, code,"\r\n\0");

    // Send the string
    writeDirectClient(clientID,string,strlen(string));


    /* SEND SERVER + DATE + LANGUAGE */
    // Define the string

    // Get the date
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char s[64];
    size_t ret = strftime(s, sizeof(s), "%c", tm);
    assert(ret);
    
    // Allocate the memory
    string = malloc(sizeof(char)*(strlen("Server: Projet HTTP\r\nDate: ")+strlen("\r\nContent-Language: fr-FR\r\n\0")+strlen(s)));
    if (string == NULL){
        perror("malloc");
        return -1;
    }

    // Concatenate the string
    sprintf(string, "Server: Projet HTTP\r\nDate: %s\r\nContent-Language: fr-FR\r\n\0", s);

    // Send the string
    writeDirectClient(clientID,string,strlen(string));

    // Free the memory
    free(string);

    return EXIT_SUCCESS;
}

/**
 * @brief Send the headers content-type and content-length
 * @param filename relative path of the file
 * @param clientID ID of the client
 * @return int 
 */
int send_type_length(char* filename, int clientID){
    // On remplit ici : Server, Content-langage, Content-Length, Date, Transfer-Encoding
    // Content-Type
    char* extension = strrchr(filename, '.');
    char* type;

    if (!strcmp(extension, ".html")){
        type = "text/html; charset=utf-8";
    } else if (!strcmp(extension, ".css")){
        type = "text/css; charset=utf-8";
    } else if (!strcmp(extension, ".js")){
        type = "application/javascript; charset=utf-8";
    } else if (!strcmp(extension, ".jpg")){
        type = "image/jpeg";
    } else if (!strcmp(extension, ".png")){
        type = "image/png";
    } else if (!strcmp(extension, ".gif")){
        type = "image/gif";
    } else if (!strcmp(extension, ".svg")){
        type = "image/svg+xml";
    } else if (!strcmp(extension, ".ico")){
        type = "image/x-icon";
    } else if (!strcmp(extension, ".pdf")){
        type = "application/pdf";
    } else if (!strcmp(extension, ".json")){
        type = "application/json";
    } else if (!strcmp(extension, ".xml")){
        type = "application/xml";
    } else if (!strcmp(extension, ".zip")){
        type = "application/zip";
    } else if (!strcmp(extension, ".mp3")){
        type = "audio/mpeg";
    } else if (!strcmp(extension, ".mp4")){
        type = "video/mp4";
    } else if (!strcmp(extension, ".mpeg")){
        type = "video/mpeg";
    } else if (!strcmp(extension, ".webm")){
        type = "video/webm";
    } else if (!strcmp(extension, ".txt")){
        type = "text/plain; charset=utf-8";
    } else {
        type = "application/octet-stream";
    }


    // TODO : séparer le cas content length et le cas chunked (est-ce qu'on doit gérer le streaming ?)
    // Content-Length
    struct stat st;
    stat(filename, &st);
    int size = st.st_size;

    // Allocate the memory
    char* string = malloc(sizeof(char)*(strlen("Content-Type: ")+strlen(type)+strlen("\r\nContent-Length: ")+strlen(size)+strlen("\r\n\0")));

    // Concatenate the string
    sprintf(string, "Content-Type: %s\r\nContent-Length: %d\r\n\0", type, size);

    // Send the string
    writeDirectClient(clientID,string,strlen(string));

    // Free the memory
    free(string);

    // Call body()
    body(filename, clientID, size);

    return EXIT_SUCCESS;
}

/**
 * @brief Send the body of the request. This function is called directly by send_type_length() because it needs the size of the file
 * @param filename relative path of the file
 * @param clientID ID of the client
 * @param size Size of the file
 * @return int 
 */
int body(char* filename, int clientID, int size){
    // Envoyer le body
    char* buffer;
    char* string;
    
    // TODO : ouvrir en mode binaire ?
    FILE * file = fopen (filename, "r+");

    if (file){
        buffer = malloc (size);
        if (buffer){
            fread (buffer, 1, size, file);
        } else {
            perror("malloc");
            return -1;
        }
        fclose (file);
    }

    if (buffer){
        // Allocate the memory
        string = malloc(sizeof(char)*(strlen("\r\n")+strlen(buffer)+strlen("\r\n\0")));
        if (string == NULL){
            perror("malloc");
            return -1;
        }

        // Concatenate the string
        sprintf(string, "\r\n%s\r\n\0", buffer);

        // Send the string
        writeDirectClient(clientID,buffer,size);

        // Free the memory
        free(buffer);
        free(string);
    }

    return EXIT_SUCCESS;
}

