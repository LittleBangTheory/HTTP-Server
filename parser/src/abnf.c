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

/* TODO :
- Store the icar like / in a separate struct, they are currently only stored in the parent struct, but not in their own struct
- Complete the function http_message()
- Add and complete all the son functions of http_message() (excepted start-line() and below)
- Check that our syntax corresponds to the one of the teacher (example : __alpha instead of alpha)
- Write a bash script to execute every test and compare the output with the expected one
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../headers/abnf.h"
#include "../headers/utility.h"

/** \fn void http_message(char **current_char, node *struct_current)
 * \brief Parse the http message
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
*/
void http_message(char **current_char, node *struct_current){
    // WORK IN PROGRESS - MAIN FUNCTION HERE
    // Call start_line
    // (Loop?)Call as many times as needed header-field+crlf
    // Call crlf
    // Call message body
}

/** \fn header_field(char **current_char, node *struct_current)
 * \brief Parse the header field
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * WORK IN PROGRESS, TO COMMENT IN ORDER TO TEST EVERYTHING ELSE
*/
void header_field(char **current_char, node *struct_current){
    // Init the struct (ptr, int...), and allocate memory for the first child
    struct_current->debut = *current_char;
    struct_current->label = HEADER_FIELD;
    struct_current->fils = NULL;

    // Allocate memory for the first child
    node *new_struct_1 = malloc(sizeof(node));
    struct_current->fils = new_struct_1;

    // Call the function for the first child, supposed to be field-name
    field_name(current_char, new_struct_1);

    if(**current_char != ':'){
        printf("Error : expected ':' in header-field, got '%c' instead\n", **current_char);
        exit(1);
    }
    // Allocate memory for the second child
    node *new_struct_2 = malloc(sizeof(node));
    new_struct_1->frere = new_struct_2;

    // Call the function for the second child, supposed to be ':'
    icar(current_char, new_struct_2);

    // Allocate memory for the third child
    new_struct_1 = new_struct_2;
    new_struct_2 = malloc(sizeof(node));
    new_struct_1->frere = new_struct_2;

    // Call the function for the third child, supposed to be OWS
    ows(current_char, new_struct_2);

    // Allocate memory for the fourth child
    new_struct_1 = new_struct_2;
    new_struct_2 = malloc(sizeof(node));
    new_struct_1->frere = new_struct_2;

    // Call the function for the fourth child, supposed to be field-value
    field_value(current_char, new_struct_2);

    // Allocate memory for the fifth child
    new_struct_1 = new_struct_2;
    new_struct_2 = malloc(sizeof(node));
    new_struct_1->frere = new_struct_2;

    // Call the function for the fifth child, supposed to be OWS
    ows(current_char, new_struct_2);

    // The end of the struct is known when the son functions are done
    struct_current->fin = *current_char;
}

/** \fn void field_name(char **current_char, node *struct_current)
 * \brief Parse the field name
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void field_name(char **current_char, node *struct_current){
    // Init the struct (ptr, int...), and allocate memory for the first child
    struct_current->debut = *current_char;
    struct_current->label = FIELD_NAME;
    struct_current->fils = NULL;

    // Allocate memory for the first child
    node *new_struct_1 = malloc(sizeof(node));
    struct_current->fils = new_struct_1;

    // Call the function for the first child, supposed to be token
    token(current_char, new_struct_1);

    // The end of the struct is known when the son functions are done
    struct_current->fin = *current_char;
}

/** \fn void ows(char **current_char, node *struct_current)
 * \brief Parse the ows
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void ows(char **current_char, node *struct_current){
    // Init the struct (ptr, int...), and allocate memory for the first child
    struct_current->debut = *current_char;
    struct_current->label = OWS;
    struct_current->fils = NULL;

    // Store as many SP and HTAB as needed
    while(**current_char == 0x20 || **current_char == 0x09) {
        // Allocate memory for the first child
        node *new_struct_1 = malloc(sizeof(node));
        struct_current->fils = new_struct_1;

        if(**current_char == 0x20){
            // Call the function for the first child, supposed to be SP
            sp(current_char, new_struct_1);
        }
        else if(**current_char == 0x09){
            // Call the function for the first child, supposed to be HTAB
            htab(current_char, new_struct_1);
        }

        // Allocate memory for the second child (if needed)
        if(*(*current_char+1) == 0x20 || *(*current_char+1) == 0x09){
            // Allocate memory for the second child
            node *new_struct_2 = new_struct_1;
            new_struct_1 = malloc(sizeof(node));
            new_struct_2->frere = new_struct_1;
        }
    }

    // Go one char back to respect convention (never go further than the end of the struct in the said function) 
    *current_char-=1;
    // The end of the struct is known when the son functions are done
    struct_current->fin = *current_char;
}

/** \fn void field_value(char **current_char, node *struct_current)
 * \brief Parse the field value
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void field_value(char **current_char, node *struct_current){
    // Init the struct (ptr, int...), and allocate memory for the first child
    struct_current->debut = *current_char;
    struct_current->label = FIELD_VALUE;
    struct_current->fils = NULL;

    // Store as many field-content as needed. 
    //TODO : STOP CONDITION TO BE CHECKED
    while(**current_char == '\n' || isvchar(**current_char)) {
        // Allocate memory for the first child
        node *new_struct_1 = malloc(sizeof(node));
        struct_current->fils = new_struct_1;

        // Call the function for the first child, supposed to be field-content or obs-fold
        if(**current_char == '\n'){
            // osb_fold : CRLF 1*( SP / HTAB )
            obs_fold(current_char, new_struct_1);
        } else if(isvchar(**current_char)){
            // field-content = field-vchar [ 1*( SP / HTAB ) field-vchar ]
            field_content(current_char, new_struct_1);
        }

        // Allocate memory for the second child (if needed)
        if(*(*current_char+1) != 0x0D && *(*current_char+1) != 0x0A){
            // Allocate memory for the second child
            node *new_struct_2 = new_struct_1;
            new_struct_1 = malloc(sizeof(node));
            new_struct_2->frere = new_struct_1;
        }
    }

    // Go one char back to respect convention (never go further than the end of the struct in the said function) 
    *current_char-=1;
    // The end of the struct is known when the son functions are done
    struct_current->fin = *current_char;
}

/** \fn void field_content(char **current_char, node *struct_current)
 * \brief Parse the field content
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * NOT TESTED NOR FINISHED 
*/
void field_content(char **current_char, node *struct_current){
    // Init the struct (ptr, int...), and allocate memory for the first child
    struct_current->debut = *current_char;
    struct_current->label = FIELD_CONTENT;
    struct_current->fils = NULL;

    // Allocate memory for the first child
    node *new_struct_1 = malloc(sizeof(node));
    struct_current->fils = new_struct_1;

    // Call the function for the first child, supposed to be field-vchar
    field_vchar(current_char, new_struct_1);

    // Allocate memory for the second child (if needed)
    if(*(*current_char+1) == 0x20 || *(*current_char+1) == 0x09){
        // Allocate memory for the second child
        node *new_struct_2 = new_struct_1;
        new_struct_1 = malloc(sizeof(node));
        new_struct_2->frere = new_struct_1;
    }

    // Store as many SP and HTAB as needed
    while(**current_char == 0x20 || **current_char == 0x09) {
        // Allocate memory for the first child
        node *new_struct_1 = malloc(sizeof(node));
        struct_current->fils = new_struct_1;

        if(**current_char == 0x20){
            // Call the function for the first child, supposed to be SP
            sp(current_char, new_struct_1);
        }
        else if(**current_char == 0x09){
            // Call the function for the first child, supposed to be HTAB
            htab(current_char, new_struct_1);
        }

        // Allocate memory for the second child (if needed)
        if(*(*current_char+1) == 0x20 || *(*current_char+1) == 0x09){
            // Allocate memory for the second child
            node *new_struct_2 = new_struct_1;
            new_struct_1 = malloc(sizeof(node));
            new_struct_2->frere = new_struct_1;
        }
    }

    // Go one char back to respect convention (never go further than the end of the struct in the said function) 
    *current_char-=1;
    // The end of the struct is known when the son functions are done
    struct_current->fin = *current_char;
}

/** \fn void start_line(char **current_char, node *struct_current)
 * \brief Parse the start line of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
*/
void start_line(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = START_LINE;
    struct_current->fils = NULL;

    // Allocate memory for the first child
    node *new_struct_1 = malloc(sizeof(node));
    struct_current->fils = new_struct_1;

    // Call the function for the first child, supposed to be request_line
    request_line(current_char, new_struct_1);

    // The end of the struct is known when the son function is done
    struct_current->fin = *current_char;
}

/** \fn void request_line(char **current_char, node *struct_current)
 * \brief Parse the request line of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
*/
void request_line(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = REQUEST_LINE;
    struct_current->fils = NULL;

    // Allocate memory for the first child
    node *new_struct_1 = malloc(sizeof(node));
    struct_current->fils = new_struct_1;
    // Call the function for the first child, supposed to be method
    method(current_char, new_struct_1);

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
    *current_char-=1;
    struct_current->fin = *current_char;
}

/** \fn void crlf(char **current_char, node *struct_current)
 * \brief Parse the CRLF of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
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

/** \fn void method(char **current_char, node *struct_current)
 * \brief Parse the method of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct 
 * 
*/
void method(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = METHOD;
    struct_current->fils = NULL;

    // Allocate memory for the first child
    node *new_struct_1 = malloc(sizeof(node));
    struct_current->fils = new_struct_1;
    // Call the function for the first child, supposed to be token
    token(current_char, new_struct_1);

    // The end is known when the son function is done
    struct_current->fin = *current_char;
}

/** \fn void token(char **current_char, node *struct_current)
 * \brief Parse the method of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
*/
void token(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = TOKEN;
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
*/
void request_target(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = REQUEST_TARGET;
    struct_current->fils = NULL;

    // Allocate memory for the first child
    node *new_struct_1 = malloc(sizeof(node));
    struct_current->fils = new_struct_1;
    absolute_path(current_char, new_struct_1);

    if(*(*current_char+1)=='?'){
        // Store the question mark
        current_char+=1;
        node *new_struct_2 = malloc(sizeof(node));
        new_struct_1->frere = new_struct_2;
        icar(current_char, new_struct_2);
        new_struct_1 = new_struct_2;

        // Move pass the question mark to store the query
        *current_char+=1;
        // Allocate memory for the first child
        new_struct_2 = malloc(sizeof(node));
        new_struct_1->frere = new_struct_2;
        query(current_char, new_struct_2);
    }
}

/** \fn void query(char **current_char, node *struct_current)
 * \brief Parse the query element of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
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
            node *new_struct_2 = malloc(sizeof(node));
            if(ispchar(**current_char)) {
                pchar(current_char, new_struct_1);
            } else {
                icar(current_char, new_struct_1);
            }
            new_struct_1->frere = new_struct_2;
            // move one struct forward 
            new_struct_1 = new_struct_2;
            *current_char+=1;
        }while(ispchar(*(*current_char+1) || *(*current_char+1)=='/' || *(*current_char+1)=='?'));
    }
    struct_current->fin = *current_char;
}

/** \fn void absolute_path(char **current_char, node *struct_current)
 * \brief Parse the absolute-path element of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void absolute_path(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = ABSOLUTE_PATH;
    struct_current->fils = NULL;

    if(**current_char=='/'){
        icar(current_char, struct_current);
        *current_char+=1;
        // Allocate memory for the first child
        node *new_struct = malloc(sizeof(node));
        struct_current->fils = new_struct;
        segment(current_char, new_struct);
    }
    // The end is known when the son functions are done
    struct_current->fin = *current_char;
}

/** \fn void segment(char **current_char, node *struct_current)
 * \brief Parse the segment element of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
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

/** \fn void pchar(char **current_char, node *struct_current)
 * \brief Parse the pchar element of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
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
    }else if(**current_char==':' || **current_char=='@'){
        icar(current_char, new_struct);
    }else{
        printf("Error : pchar not recognized");
        exit(1);
    }
    struct_current->fin = *current_char;
}

/** \fn void unreserved(char **current_char, node *struct_current)
 * \brief Parse the unreserved element of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
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
    }else if(**current_char=='-' || **current_char=='.' || **current_char=='_' || **current_char=='~'){
        icar(current_char, new_struct);
    }else{
        printf("Error : unreserved not recognized");
        exit(1);
    }
    *current_char+=1;
    struct_current->fin = *current_char;
}

/** \fn void pct_encoded(char **current_char, node *struct_current)
 * \brief Parse the pct-encoded element of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void pct_encoded(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = PCT_ENCODED;
    struct_current->fils = NULL;

    // Allocate memory for the child
    if(**current_char=='%'){
        icar(current_char, struct_current);
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

/** \fn void hexdig(char **current_char, node *struct_current)
 * \brief Parse the hexdig element of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
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

/** \fn void sub_delims(char **current_char, node *struct_current)
 * \brief Parse the sub_delims element of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
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
        // Allocate memory for the first child
        node *new_struct = malloc(sizeof(node));
        struct_current->fils = new_struct;

        // If the next char is a letter, call alpha()
        if(isalpha(**current_char)){
            alpha(current_char, new_struct);
        
        // If the next char is a digit, call digit()
        } else if(isdigit(**current_char)){
            digit(current_char, new_struct);

        // Else, it's an icar
        } else{
            icar(current_char, new_struct);
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

/** \fn void htab(char **current_char, node *struct_current)
 * \brief Parse the HTAB character of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void htab(char **current_char, node *struct_current){
    if (**current_char != 0x09){
        printf("Error: Expected a HTAB, not %c\n", **current_char);
        exit(1);
    }
    struct_current->label = HTAB;
    struct_current->fils = NULL;
    struct_current->debut = *current_char;
    struct_current->fin = *current_char;
}

/** \fn void icar(char **current_char, node *struct_current)75
 * \brief Parse the icar character of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void icar(char **current_char, node *struct_current){
    struct_current->label = ICAR;
    struct_current->fils = NULL;
    struct_current->debut = *current_char;
    struct_current->fin = *current_char;
}

/** \fn void obs_fold(char **current_char, node *struct_current)
 * \brief Parse the obs_fold (obs-fold = CRLF 1*( SP / HTAB )) characters of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void obs_fold(char **current_char, node *struct_current){
    struct_current->label=OBS_FOLD;
    struct_current->fils = NULL;
    struct_current->debut = *current_char;

    // Allocate memory
    node *new_struct = malloc(sizeof(node));
    node *new_struct2;
    struct_current->fils = new_struct;
    crlf(current_char,new_struct);

    //TODO : check if we have AT LEAST one
    while(**current_char==0x20 || **current_char==0x9){
        new_struct2=malloc(sizeof(node));
        new_struct->frere=new_struct2;
        if(**current_char==0x20){
            sp(current_char,new_struct2);
        }
        new_struct=new_struct2;
        }
    }

/** \fn void field_vchar(char **current_char, node *struct_current)
 * \brief Parse the field_vchar (field-vchar = VCHAR / obs-text) characters of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void field_vchar(char **current_char, node *struct_current){
    // TODO

}
