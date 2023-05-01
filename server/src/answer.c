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
* Server : nom du serveur 
* Content-Type : le format utilisé (exemple : text/html; charset=utf-8)
* Content-Language : La langue utilisée. (exemple : fr-FR)
* Content-Length : Taille de la représentation (en octet) 
* Date : Date et heure de la réponse
* Transfer-Encoding : chunked, compress, deflate, gzip, identity
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

int version(int code, char* version){
    // TODO : Comment envoyer le string de retour à main.c ?
    // TODO : Est-ce que les headers contiennent le CRLF ? Ou ajouté à l'envoie ?
    // HTTP Version + code de retour


    // Body ou média
}

int headers(char* headers[header_number][2], char* filename){
    // On remplit ici : Server, Content-langage, Content-Length, Date, Transfer-Encoding
    // Content-Type
    char* extension;
    if(strcmp(headers[Content_Type][0],empty_value)){
        // On envoie le type de fichier en fonction de l'extension
        extension = strrchr(filename, '.');
    } else {
        // On envoie le type de fichier en fonction de la valeur du header
        strcpy(extension, headers[Content_Type][1]);
    }
    if (!strcmp(extension, ".html")){
        strcpy(headers[Content_Type][1], "text/html; charset=utf-8");
    } else if (!strcmp(extension, ".css")){
        strcpy(headers[Content_Type][1], "text/css; charset=utf-8");
    } else if (!strcmp(extension, ".js")){
        strcpy(headers[Content_Type][1], "application/javascript; charset=utf-8");
    } else if (!strcmp(extension, ".jpg")){
        strcpy(headers[Content_Type][1], "image/jpeg");
    } else if (!strcmp(extension, ".png")){
        strcpy(headers[Content_Type][1], "image/png");
    } else if (!strcmp(extension, ".gif")){
        strcpy(headers[Content_Type][1], "image/gif");
    } else if (!strcmp(extension, ".svg")){
        strcpy(headers[Content_Type][1], "image/svg+xml");
    } else if (!strcmp(extension, ".ico")){
        strcpy(headers[Content_Type][1], "image/x-icon");
    } else if (!strcmp(extension, ".pdf")){
        strcpy(headers[Content_Type][1], "application/pdf");
    } else if (!strcmp(extension, ".json")){
        strcpy(headers[Content_Type][1], "application/json");
    } else if (!strcmp(extension, ".xml")){
        strcpy(headers[Content_Type][1], "application/xml");
    } else if (!strcmp(extension, ".zip")){
        strcpy(headers[Content_Type][1], "application/zip");
    } else if (!strcmp(extension, ".mp3")){
        strcpy(headers[Content_Type][1], "audio/mpeg");
    } else if (!strcmp(extension, ".mp4")){
        strcpy(headers[Content_Type][1], "video/mp4");
    } else if (!strcmp(extension, ".mpeg")){
        strcpy(headers[Content_Type][1], "video/mpeg");
    } else if (!strcmp(extension, ".webm")){
        strcpy(headers[Content_Type][1], "video/webm");
    } else if (!strcmp(extension, ".txt")){
        strcpy(headers[Content_Type][1], "text/plain; charset=utf-8");
    } else {
        strcpy(headers[Content_Type][1], "application/octet-stream");
    }

    // Content-Length
    // TODO : séparer le cas content length et le cas chunked (est-ce qu'on doit gérer le streaming ?)
    struct stat st;
    stat(filename, &st);
    int size = st.st_size;

    // Server
    strcpy(headers[Server][1], "Projet HTTP");

    // Content-Language
    strcpy(headers[Content_Language][1], "fr-FR");

    // Pour i de 0 à taille-1, pour j de 0 à 1, si headers[i][j] != empty_header, on l'envoie
    for (int i = 0; i < header_number; i++){
        if (!strcmp(headers[i][0],empty_value)){
            // TODO 
        }
    }
}

int body(char* filename){
    // Envoyer le body
    char * buffer = 0;
    long length;
    // TODO : ouvrir en mode binaire ?
    FILE * file = fopen (filename, "r+");

    if (file){
        // Get size : en octet si fichier ouvert en binaire, sinon dépendant de l'encodage
        fseek (file, 0, SEEK_END);
        length = ftell (file);
        fseek (file, 0, SEEK_SET);
        buffer = malloc (length);
        if (buffer){
            fread (buffer, 1, length, file);
        }
        fclose (file);
    }

    if (buffer){
    // start to process your data / extract strings here...
    }
}

