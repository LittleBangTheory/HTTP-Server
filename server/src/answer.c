/** \file answer.c
 * This file will execute 2 or 3 types of request :
 *  - GET
 *  - HEAD
 *  - POST
*/

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
*/

/* Headers to send :
* Server : nom du serveur 
* Content-Type : le format utilisé (exemple : text/html; charset=utf-8)
* Content-Language : La langue utilisée. (exemple : fr-FR)
* Content-Length : Taille de la représentation (en octet) 
* Connection : close ou keep-alive
* Date : Date et heure de la réponse
* Last-Modified : Date et heure de la dernière modification du document
* Transfer-Encoding : chunked, compress, deflate, gzip, identity
    chunked est utilisé pour les réponses de type "streaming", obligatoire en l'absence de Content-Length
*/

/*
Useless headers :
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

int answer(){
    // HTTP Version + code de retour
    // Tous les headers un par un 
    // Body ou média
}

