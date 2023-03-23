#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../headers/abnf.h"

char tchar_list[] = {'0x21', '0x23', '0x24', '0x25', '0x26', '0x27', '0x2a', '0x2b', '0x2d', '0x2e', '0x5e', '0x5f', '0x60', '0x7c', '0x7e'};


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
    node *new_struct = malloc(sizeof(node));
    struct_current->fils = new_struct;

    // Call the function for the first child, supposed to be method
    method(current_char, new_struct, label);

    // The end is known when the method() is done
    struct_current->fin = *current_char;

    // TODO : call the brother
}

/** \fn void method(char **current_char, node *struct_current, char *label)
 * \brief Parse the method of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * \param label : pointer to the array of labels
 * There is no token() function, as a method is always a token
*/
void method(char **current_char, node *struct_current, char *label){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = label+METHOD;
    struct_current->fils = NULL;
    struct_current->frere = NULL;

    // Allocate memory for the first child
    node *new_struct = malloc(sizeof(node));
    struct_current->fils = new_struct;
    tchar(current_char, new_struct, label);
    *current_char++;
    struct_current->fin = *current_char;

    // TODO : call the brother (SP)
}

void tchar(char **current_char, node *struct_current, char *label){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = label+TCHAR;
    struct_current->fils = NULL;
    struct_current->frere = NULL;

    // Allocate memory for the first child
    node *new_struct = malloc(sizeof(node));
    struct_current->fils = new_struct;

    // if it's a SP, end of method
    if(**current_char != 0x20) {
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

        // If the next char isn't a tchar, error
        } else if(!istchar(**current_char)){
            printf("Error: the method must be composed of tchars, not %c", **current_char);
            exit(1);
        }

        // The next char is a tchar, create a brother node and call tchar()
        *current_char++;
        // Allocate memory for the first child
        node *new_struct = malloc(sizeof(node));
        struct_current->frere = new_struct;
        tchar(current_char, new_struct, label);
    }
}

// Check if the char belongs to this list : "!" / "#" / "$" / "%" / "&" / "'" / "*" / "+" / "-" / "." / "^" / "_" / "`" / "|" / "~"
int istchar(char c){
    for(int i=0; i<15; i++){
        if(c == tchar_list[i]){
            return 1;
        }
    }
    return 0;
}


void alpha(char **current_char, node *struct_current, char *label){
    struct_current->label = label+ALPHA;
    struct_current->fils = NULL;
    struct_current->frere = NULL;
    struct_current->debut = *current_char;
    struct_current->fin = *current_char;
}

void digit(char **current_char, node *struct_current, char *label){
    struct_current->label = label+DIGIT;
    struct_current->fils = NULL;
    struct_current->frere = NULL;
    struct_current->debut = *current_char;
    struct_current->fin = *current_char;
}