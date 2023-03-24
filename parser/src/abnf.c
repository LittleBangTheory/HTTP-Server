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


/** \fn void start_line(char **current_char, node *struct_current, char *label)
 * \brief Parse the start line of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * \param label : pointer to the array of labels
 * There is no request_line() function, as a start line is always a request line
*/
void start_line(char **current_char, node *struct_current, char *label){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = label+START_LINE;
    struct_current->fils = NULL;
    struct_current->frere = NULL;

    // Allocate memory for the first child
    node *new_struct_1 = malloc(sizeof(node));
    struct_current->fils = new_struct_1;
    // Call the function for the first child, supposed to be method
    token(current_char, new_struct_1, label);
    *current_char+=1;

    // Allocate memory for sp
    node *new_struct_2 = malloc(sizeof(node));
    new_struct_1->frere = new_struct_2;
    sp(current_char, new_struct_2, label);
    new_struct_1 = new_struct_2;
    *current_char+=1;

    // Allocate memory for request-target
    new_struct_2 = malloc(sizeof(node));
    new_struct_1->frere = new_struct_2;
    request_target(current_char, new_struct_2, label);

    // TODO : call request-target() sp() http_version() crlf()

    // The end is known when the son functions are done
    struct_current->fin = *current_char;
}

/** \fn void token(char **current_char, node *struct_current, char *label)
 * \brief Parse the method of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * \param label : pointer to the array of labels
 * There is no method() function, as a method is always a token
*/
void token(char **current_char, node *struct_current, char *label){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = label+METHOD;
    struct_current->fils = NULL;
    struct_current->frere = NULL;

    // Allocate memory for the child and its brothers)
    node *new_struct_1 = malloc(sizeof(node));
    struct_current->fils = new_struct_1;
    do{
        node *new_struct_2 = malloc(sizeof(node));
        tchar(current_char, new_struct_1, label);
        new_struct_1->frere = new_struct_2;
        // move one struct forward 
        new_struct_1 = new_struct_2;
        *current_char+=1;
    }while(istchar(*(*current_char+1)));
    struct_current->fin = *current_char;
}

/** \fn void request_target(char **current_char, node *struct_current, char *label)
 * \brief Parse the request-target element of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * \param label : pointer to the array of labels
 * Ass origin-form() function
*/
void request_target(char **current_char, node *struct_current, char *label){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = label+REQUEST_TARGET;
    struct_current->fils = NULL;
    struct_current->frere = NULL;

    // Allocate memory for the first child
    node *new_struct = malloc(sizeof(node));
    struct_current->fils = new_struct;
    absolute_path(current_char, new_struct, label);

    //TODO : call question_mark() and query() as brothers of absolute_path() 

}

void absolute_path(char **current_char, node *struct_current, char *label){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = label+ABSOLUTE_PATH;
    struct_current->fils = NULL;
    struct_current->frere = NULL;

    if(**current_char=='/'){
        *current_char+=1;
        // Allocate memory for the first child
        node *new_struct = malloc(sizeof(node));
        struct_current->fils = new_struct;
        segment(current_char, new_struct, label);
    }
    // The end is known when the son functions are done
    struct_current->fin = *current_char;
}

void segment(char **current_char, node *struct_current, char *label){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = label+SEGMENT;
    struct_current->fils = NULL;
    struct_current->frere = NULL;

    // Allocate memory for the first child
    node *new_struct_1 = malloc(sizeof(node));
    struct_current->fils = new_struct_1;
    do{
        node *new_struct_2 = malloc(sizeof(node));
        pchar(current_char, new_struct_2, label);
        // move one struct forward 
        new_struct_1->frere = new_struct_2;
        new_struct_1 = new_struct_2;
        *current_char+=1;
    }while(ispchar(*(*current_char+1)));
    struct_current->fin = *current_char;
}

void pchar(char **current_char, node *struct_current, char *label){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = label+PCHAR;
    struct_current->fils = NULL;
    struct_current->frere = NULL;

    // Allocate memory for the first child
    node *new_struct = malloc(sizeof(node));
    struct_current->fils = new_struct;
    if(isunreserved(**current_char)){
        unreserved(current_char, new_struct, label);
    }

    //unreserved / pct-encoded / sub-delims / ":" / "@"

}

/** \fn void tchar(char **current_char, node *struct_current, char *label)
 * \brief Parse the tchar of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * \param label : pointer to the array of labels
 * 
*/
void tchar(char **current_char, node *struct_current, char *label){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = label+TCHAR;
    struct_current->fils = NULL;
    struct_current->frere = NULL;

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

            alpha(current_char, new_struct, label);
        
        // If the next char is a digit, create a son node and call digit()
        } else if(isdigit(**current_char)){
            // Allocate memory for the first child
            node *new_struct = malloc(sizeof(node));
            struct_current->fils = new_struct;

            digit(current_char, new_struct, label);
        }
    // If the next char isnt a tchar or a SP, error
    } else if(!istchar(**current_char)){
        printf("Error: the method must be composed of tchars, not %c", **current_char);
        exit(1);
    }
}

/** \fn void alpha(char **current_char, node *struct_current, char *label)
 * \brief Parse the alpha characters of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * \param label : pointer to the array of labels
 * 
*/
void alpha(char **current_char, node *struct_current, char *label){
    if (!isalpha(**current_char)){
        printf("Error: Expected a char, not %c", **current_char);
        exit(1);
    }
    struct_current->label = label+ALPHA;
    struct_current->fils = NULL;
    struct_current->frere = NULL;
    struct_current->debut = *current_char;
    struct_current->fin = *current_char;
}

/** \fn void digit(char **current_char, node *struct_current, char *label)
 * \brief Parse the digit characters of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * \param label : pointer to the array of labels
 * 
*/
void digit(char **current_char, node *struct_current, char *label){
    if (!isdigit(**current_char)){
        printf("Error: Expected a digit, not %c", **current_char);
        exit(1);
    }
    struct_current->label = label+DIGIT;
    struct_current->fils = NULL;
    struct_current->frere = NULL;
    struct_current->debut = *current_char;
    struct_current->fin = *current_char;
}

/** \fn void sp(char **current_char, node *struct_current, char *label)
 * \brief Parse the SP character of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * \param label : pointer to the array of labels
 * 
*/
void sp(char **current_char, node *struct_current, char *label){
    if (**current_char != 0x20){
        printf("Error: Expected a SP, not %c", **current_char);
        exit(1);
    }
    struct_current->label = label+SP;
    struct_current->fils = NULL;
    struct_current->frere = NULL;
    struct_current->debut = *current_char;
    struct_current->fin = *current_char;
}
