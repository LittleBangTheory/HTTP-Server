#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../headers/abnf.h"

char tchar_list[] = {'0x21', '0x23', '0x24', '0x25', '0x26', '0x27', '0x2a', '0x2b', '0x2d', '0x2e', '0x5e', '0x5f', '0x60', '0x7c', '0x7e'};

//start line = request line
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

//method = token
void method(char **current_char, node *struct_current, char *label){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = label+METHOD;
    struct_current->fils = NULL;
    struct_current->frere = NULL;

    // Allocate memory for the first child
    node *new_struct = malloc(sizeof(node));
    struct_current->fils = new_struct;
    while (**current_char != 0x20){
        tchar(current_char, new_struct, label);
        *current_char++;
    }
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

    // If the next char isn't a tchar, create a son node and call tchar()
    } else if(!istchar(**current_char)){
        printf("Error: the method must be composed of tchars, not %c", **current_char);
        exit(1);
    }

    // If the next char is not a space, create a brother node and call tchar() again
    if(**current_char != 0x20){
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

void alpha(char **current_char, node *struct_current, label){
    struct_current->label, "alpha");
    struct_current->fils = NULL;
    struct_current->frere = NULL;
    struct_current->debut = *current_char;
    struct_current->fin = *current_char;
    *current_char += 1;
}