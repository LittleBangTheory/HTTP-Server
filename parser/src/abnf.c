/** \file abnf.c
 * \brief Contains the functions to parse the request
 * General rules :
 * - The function name is the name of the rule in the RFC
 * - The function takes 3 parameters : a pointer to the current char, a pointer to the current struct and a pointer to the array of labels
 * - The function returns nothing
 * - Every time a new structure is needed (to store a brother or a son), it is allocated by the father function, but initialized by the son or father function. Example : token() creates the struct for tchar(), but tchar() initializes it
 * - Every time brothers are needed, the father function calls the brother functions and link the brothers together. Example : request-line() creates the struct for method(), sp(), request-target(), sp() http_version(), and crlf(), and link them together
 * - If the reading pointer needs to be moved, it should be moved by the highest function possible. Example : tchar d   oesnt move the reading pointer, but token() does, exepted for the last one, that is moved by method(), etc.
 * - The end of the struct is known when the son functions are done
 * - Always check in a function that the character(s) to store is/are the right one(s) (example : in tchar(), check that the character is a tchar)
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../headers/abnf.h"
#include "../headers/utility.h"


/** \fn void start_line(char **current_char, node *struct_current)
 * \brief Parse the start line of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * \param label : pointer to the array of labels
 * There is no request_line() function, as a start line is always a request line
*/
void start_line(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = START_LINE;
    struct_current->fils = NULL;

    // Allocate memory for the first child
    node *new_struct_1 = malloc(sizeof(node));
    struct_current->fils = new_struct_1;
    // Call the function for the first child, supposed to be method
    token(current_char, new_struct_1);

    // Allocate memory for sp
    node *new_struct_2 = malloc(sizeof(node));
    new_struct_1->frere = new_struct_2;
    sp(current_char, new_struct_2);
    new_struct_1 = new_struct_2;
    *current_char+=1;

    // Allocate memory for request-target
    new_struct_2 = malloc(sizeof(node));
    new_struct_1->frere = new_struct_2;
    request_target(current_char, new_struct_2);

    // Allocate memory for sp
    new_struct_1 = new_struct_2;
    new_struct_2 = malloc(sizeof(node));
    new_struct_1->frere = new_struct_2;
    sp(current_char, new_struct_2);
    *current_char+=1;

    // Allocate memory for http_version
    new_struct_1 = new_struct_2;
    new_struct_2 = malloc(sizeof(node));
    new_struct_1->frere = new_struct_2;
    http_version(current_char, new_struct_2);
    *current_char+=1;

    // Allocate memory for crlf
    new_struct_1 = new_struct_2;
    new_struct_2 = malloc(sizeof(node));
    new_struct_1->frere = new_struct_2;
    crlf(current_char, new_struct_2);

    // The end is known when the son functions are done
    struct_current->fin = *current_char;
}

void crlf(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = CRLF;
    struct_current->fils = NULL;

    // Check if \n is present
    if (**current_char != '\n'){
        printf("Error : line feed expected, %c found\n", **current_char);
        exit(1);
    }
    *current_char+=1;

    // The end is known at the end
    struct_current->fin = *current_char;
}

/** \fn void http_version(char **current_char, node *struct_current)
 * \brief Parse the http version of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void http_version(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = HTTP_VERSION;
    struct_current->fils = NULL;

    // Allocate memory for the first child
    node *new_struct_1 = malloc(sizeof(node));
    struct_current->fils = new_struct_1;
    // Call the function for the first child, supposed to be http-name
    http_name(current_char, new_struct_1);

    // Check if / is present
    if (**current_char != '/'){
        printf("Error : slash expected, %c found\n", **current_char);
        exit(1);
    }
    *current_char+=1;

    // Allocate memory for digit
    node *new_struct_2 = malloc(sizeof(node));
    new_struct_1->frere = new_struct_2;
    digit(current_char, new_struct_2);
    *current_char+=1;

    // Check if . is present
    if(**current_char != '.'){
        printf("Error : dot expected, %c found", **current_char);
        exit(1);
    }
    *current_char+=1;

    // Allocate memory for digit
    new_struct_1 = new_struct_2;
    new_struct_2 = malloc(sizeof(node));
    new_struct_1->frere = new_struct_2;
    digit(current_char, new_struct_2);

    // The end is known when the son functions are done
    struct_current->fin = *current_char;
}

/** \fn void http_name(char **current_char, node *struct_current)
 * \brief Parse the name of the http version
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void http_name(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = HTTP_NAME;
    struct_current->fils = NULL;

    if(!(**current_char == 'H' && *(*current_char+1) == 'T' && *(*current_char+2) == 'T' && *(*current_char+3) == 'P')){
        printf("Error : HTTP expected, %c%c%c%c found\n", **current_char, *(*current_char+1), *(*current_char+2), *(*current_char+3));
        exit(1);
    }
    *current_char+=4;

    // The end is known when the son functions are done
    struct_current->fin = *current_char;
}

/** \fn void token(char **current_char, node *struct_current)
 * \brief Parse the method of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * \param label : pointer to the array of labels
 * There is no method() function, as a method is always a token
*/
void token(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = METHOD;
    struct_current->fils = NULL;

    // Allocate memory for the child and its brothers)
    node *new_struct_1 = malloc(sizeof(node));
    node *new_struct_2;
    tchar(current_char, new_struct_1);
    *current_char+=1;
    struct_current->fils = new_struct_1;
    do{
        new_struct_2 = malloc(sizeof(node));
        tchar(current_char, new_struct_2);
        new_struct_1->frere = new_struct_2;
        // move one struct forward 
        new_struct_1 = new_struct_2;
        *current_char+=1;
    }while(istchar(*(*current_char)));
    struct_current->fin = *current_char;
}

/** \fn void request_target(char **current_char, node *struct_current)
 * \brief Parse the request-target element of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * \param label : pointer to the array of labels
 * Ass origin-form() function
*/
void request_target(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = REQUEST_TARGET;
    struct_current->fils = NULL;

    // Allocate memory for the first child
    node *new_struct = malloc(sizeof(node));
    struct_current->fils = new_struct;
    absolute_path(current_char, new_struct);

    if(*(*current_char+1)=='?'){
        // Move pass the question mark to store the query
        *current_char+=2;
        // Allocate memory for the first child
        new_struct = malloc(sizeof(node));
        (struct_current->fils)->frere = new_struct;
        query(current_char, new_struct);
    }
}

void query(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = QUERY;
    struct_current->fils = NULL;

    // Allocate memory for the child and its brothers)
    node *new_struct_1 = malloc(sizeof(node));
    struct_current->fils = new_struct_1;
    if(ispchar(**current_char) || **current_char=='/' || **current_char=='?'){
        do{
            if(ispchar(**current_char)) {
                node *new_struct_2 = malloc(sizeof(node));
                pchar(current_char, new_struct_1);
                new_struct_1->frere = new_struct_2;
                // move one struct forward 
                new_struct_1 = new_struct_2;
            }
            *current_char+=1;
        }while(ispchar(*(*current_char+1) || *(*current_char+1)=='/' || *(*current_char+1)=='?'));
    }
    struct_current->fin = *current_char;
}

void absolute_path(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = ABSOLUTE_PATH;
    struct_current->fils = NULL;

    if(**current_char=='/'){
        *current_char+=1;
        // Allocate memory for the first child
        node *new_struct = malloc(sizeof(node));
        struct_current->fils = new_struct;
        segment(current_char, new_struct);
    }
    // The end is known when the son functions are done
    struct_current->fin = *current_char;
}

void segment(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = SEGMENT;
    struct_current->fils = NULL;

    if(ispchar(**current_char)){
        // Allocate memory for the first child
        node *new_struct_1 = malloc(sizeof(node));
        struct_current->fils = new_struct_1;
        do{
            node *new_struct_2 = malloc(sizeof(node));
            pchar(current_char, new_struct_2);
            // move one struct forward 
            new_struct_1->frere = new_struct_2;
            new_struct_1 = new_struct_2;
            *current_char+=1;
        }while(ispchar(*(*current_char+1)));
    }
    struct_current->fin = *current_char;
}

void pchar(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = PCHAR;
    struct_current->fils = NULL;
    // Allocate memory for the first child
    node *new_struct = malloc(sizeof(node));
    struct_current->fils = new_struct;
    if(isunreserved(**current_char)){
        unreserved(current_char, new_struct);
    }else if(ispct_encoded(**current_char)){
        pct_encoded(current_char, new_struct);
    }else if(issub_delims(**current_char)){
        sub_delims(current_char, new_struct);
    }else if(**current_char!=':' && **current_char!='@'){
        printf("Error : pchar not recognized");
        exit(1);
    }
    struct_current->fin = *current_char;
}

void unreserved(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = UNRESERVED;
    struct_current->fils = NULL;

    // Allocate memory for the first child
    node *new_struct = malloc(sizeof(node));
    struct_current->fils = new_struct;
    if(isalpha(**current_char)){
        alpha(current_char, new_struct);
    }else if(isdigit(**current_char)){
        digit(current_char, new_struct);
    }else if(!(**current_char=='-' || **current_char=='.' || **current_char=='_' || **current_char=='~')){
        printf("Error : unreserved not recognized");
        exit(1);
    }
    *current_char+=1;
    struct_current->fin = *current_char;
}

void pct_encoded(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = PCT_ENCODED;
    struct_current->fils = NULL;

    if(**current_char=='%'){
        *current_char+=1;
        // Allocate memory for the first child
        node *new_struct_1 = malloc(sizeof(node));
        struct_current->fils = new_struct_1;
        hexdig(current_char, new_struct_1);
        *current_char+=1;
        // Allocate memory for the first child
        node *new_struct_2 = malloc(sizeof(node));
        new_struct_1->frere = new_struct_2;
        hexdig(current_char, new_struct_2);
    }else{
        printf("Error : pct_encoded not recognized");
        exit(1);
    }
    struct_current->fin = *current_char;
}

void hexdig(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = HEXDIG;
    struct_current->fils = NULL;
    if(!isxdigit(**current_char)){
        printf("Error : hexdig not recognized");
        exit(1);
    }
    struct_current->fin = *current_char;
}

void sub_delims(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = SUB_DELIMS;
    struct_current->fils = NULL;
    if(!(issub_delims(**current_char))){
        printf("Error : sub_delims not recognized");
        exit(1);
    }
    struct_current->fin = *current_char;
}

/** \fn void tchar(char **current_char, node *struct_current)
 * \brief Parse the tchar of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * \param label : pointer to the array of labels
 * 
*/
void tchar(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = TCHAR;
    struct_current->fils = NULL;

    // Allocate memory for the first child
    node *new_struct = malloc(sizeof(node));
    struct_current->fils = new_struct;

    // if its a SP, end of method
    if(istchar(**current_char)) {
        // If the next char is a letter, create a son node and call alpha()
        if(isalpha(**current_char)){
            // Allocate memory for the first child
            node *new_struct = malloc(sizeof(node));
            struct_current->fils = new_struct;

            alpha(current_char, new_struct);
        
        // If the next char is a digit, create a son node and call digit()
        } else if(isdigit(**current_char)){
            // Allocate memory for the first child
            node *new_struct = malloc(sizeof(node));
            struct_current->fils = new_struct;

            digit(current_char, new_struct);
        }
    // If the next char isnt a tchar or a SP, error
    } else if(!istchar(**current_char)){
        printf("Error: the method must be composed of tchars, not %c", **current_char);
        exit(1);
    }
}

/** \fn void alpha(char **current_char, node *struct_current)
 * \brief Parse the alpha characters of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * \param label : pointer to the array of labels
 * 
*/
void alpha(char **current_char, node *struct_current){
    if (!isalpha(**current_char)){
        printf("Error: Expected a char, not %c", **current_char);
        exit(1);
    }
    struct_current->label = ALPHA;
    struct_current->fils = NULL;
    struct_current->debut = *current_char;
    struct_current->fin = *current_char;
}

/** \fn void digit(char **current_char, node *struct_current)
 * \brief Parse the digit characters of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * \param label : pointer to the array of labels
 * 
*/
void digit(char **current_char, node *struct_current){
    if (!isdigit(**current_char)){
        printf("Error: Expected a digit, not %c", **current_char);
        exit(1);
    }
    struct_current->label = DIGIT;
    struct_current->fils = NULL;
    struct_current->debut = *current_char;
    struct_current->fin = *current_char;
}

/** \fn void sp(char **current_char, node *struct_current)
 * \brief Parse the SP character of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * \param label : pointer to the array of labels
 * 
*/
void sp(char **current_char, node *struct_current){
    if (**current_char != 0x20){
        printf("Error: Expected a SP, not %c\n", **current_char);
        exit(1);
    }
    struct_current->label = SP;
    struct_current->fils = NULL;
    struct_current->debut = *current_char;
    struct_current->fin = *current_char;
}
