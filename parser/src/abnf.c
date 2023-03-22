#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../headers/abnf.h"

//start line = request line
void start_line(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->s = *current_char;
    //TODO : add label start_line to struct_current->label 
    struct_current->fils = NULL;
    struct_current->frere = NULL;

    // Allocate memory for the first child
    node *new_struct = malloc(sizeof(node));
    struct_current->fils = new_struct;

    // Call the function for the first child, supposed to be method
    method(current_char, new_struct);
    //TODO : struct_current->taille à attribuer au return de la fonction fils pour savoir ou la start line s'arrête
}

//method = token
void method(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->s = *current_char;
    //TODO : add label method to struct_current->label 
    struct_current->fils = NULL;
    struct_current->frere = NULL;

    // Allocate memory for the first child
    node *new_struct = malloc(sizeof(node));
    struct_current->fils = new_struct;

    // TODO : Call tchar to store 
    //TODO : struct_current->taille à attribuer au return de la fonction fils pour savoir ou la methode s'arrête
}