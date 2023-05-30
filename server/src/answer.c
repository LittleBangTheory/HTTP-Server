/** @file answer.c
 * @brief Responsible for building and sending the answer to the client.
 * 
 * Sends :
 * 
 *  Server : nom du serveur Content-Language : La langue utilisée. (exemple : fr-FR)
 * 
 *  Date : Date et heure de la réponse
 * 
 *  Content-Type : le format utilisé (exemple : text/html; charset=utf-8)
 * 
 *  Content-Length : Taille de la représentation (en octet) 
 * 
 * Doesn't still send :  
 * 
 *  Transfer-Encoding : chunked, compress, deflate, gzip, identity. Chunked est utilisé pour les réponses de type "streaming", obligatoire en l'absence de Content-Length
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <assert.h>
#include <time.h>
#include "../headers/request.h"
#include "../headers/answer.h"

/**
 * @brief Send the version of the HTTP protocol, the code of the return, and the date and server headers
 * @param code Return code of the request. Char* to allow to send "200" or "200 OK" for example
 * @param version Version of the HTTP protocol, format "HTTP/1.1"
 * @return int 
 */
int send_version_code(char* code, char* version, int clientID){
    /* SEND VERSION + CODE */
    // Define the string. +4 to account for one space, one \r, one \n
    char* string = calloc(strlen(version)+strlen(code)+strlen("\r\n")+5,sizeof(char));
    if (string == NULL){
        perror("malloc");
        return -1;
    }

    // Concatenate the string
    sprintf(string, "%s %s\r\n", version, code);

    printf("%s", string);

    // Send the string
    writeDirectClient(clientID,string,strlen(string));


    /* SEND SERVER + DATE + LANGUAGE */
    // Define the string

    // Get the date
    /*
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char s[64];
    size_t ret = strftime(s, sizeof(s), "%c", tm);
    assert(ret);
    */

    char buf[1000];
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);

    free(string);
    
    // Allocate the memory
    string = malloc(sizeof(char)*(strlen("Server: Projet HTTP\r\nDate: ")+strlen("\r\nContent-Language: fr-FR\r\n\0")+strlen(buf)+1));
    if (string == NULL){
        perror("malloc");
        return -1;
    }

    // Concatenate the string
    sprintf(string, "Server: Projet HTTP\r\nDate: %s\r\nContent-Language: fr-FR\r\n", buf);

    printf("%s", string);

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
<<<<<<< Updated upstream
int send_type_length(char* filename, int clientID, char* type){
    // On remplit ici : Server, Content-langage, Content-Length, Date, Transfer-Encoding

    char* string = malloc(sizeof(char)*(strlen("Content-Type: ")+strlen(type)+3));
    sprintf(string, "Content-Type: %s\r\n", type);
=======
int send_type_length(char* filename, int clientID, char* final_mime_type){
    // On remplit ici : Server, Content-langage, Content-Length, Date, Transfer-Encoding, Content-Type

    char* string = malloc(sizeof(char)*(strlen("Content-Type: ")+strlen(final_mime_type)+3));
    sprintf(string, "Content-Type: %s\r\n", final_mime_type);
>>>>>>> Stashed changes

    writeDirectClient(clientID,string,strlen(string));
    printf("%s", string);
    free(string);

    struct stat st;
    stat(filename, &st);
    int size = st.st_size;

    // Allocate the memory
    string = calloc(strlen("Content-Length: ")+size+3,sizeof(char));

    // Concatenate the string
    sprintf(string, "Content-Length: %d\r\n", size);

    printf("%s", string);

    // Send the string
    writeDirectClient(clientID,string,strlen(string));

    // Free the memory
    free(string);

    return size;
}

/**
 * @brief Send the body of the request.
 * @param filename relative path of the file
 * @param clientID ID of the client
 * @param size Size of the file
 * @return int 
 */
int send_body(FILE* file, int clientID, int size){
    // Envoyer le body
    char* buffer = NULL;
    buffer = calloc(size,sizeof(char));
    if (buffer == NULL){
        perror("malloc");
        return -1;
    }
    // Read the file
    fread (buffer, 1, size, file);
    fclose (file);

    // Concatenate the string
    //sprintf(string, "\r\n%s\r\n\r\n", buffer);

    printf("\r\n%.*s\r\n\r\n",size,buffer);

    // Send the strings
    writeDirectClient(clientID,"\r\n",2);
    writeDirectClient(clientID,buffer,size);
    writeDirectClient(clientID,"\r\n\r\n",4);

    // Free the memory
    free(buffer);

    return EXIT_SUCCESS;
}

