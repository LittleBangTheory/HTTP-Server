#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../headers/abnf.h"

/**
 * \file abnf.c
 * \brief ABNF parser 
 * contains the functions to parse the request and to check if it is valid
*/

/**
 * \fn void start(char **current_char, node *struct_current)
 * \brief Function to check the presence of 'start' in the beggining of the request
 * \param current_char Pointer of pointer to the current char of the request
 * \param struct_current Pointer to the current struct of the chained list. This parameter is initialized to NULL but allocated before the call of this function
*/
void debut(char **current_char, node *struct_current){
    // Check if the current char is 's' and the next 4 chars are 't', 'a', 'r', 't'
    // printf("CC=%c\n",*(current_char[0]+0));
    if (**current_char == 's' && *(current_char[0] + 1) == 't' && *(current_char[0] + 2) == 'a' && *(current_char[0] + 3) == 'r' && *(current_char[0] + 4) == 't'){
        // Init the struct (ptr, int...)
        struct_current->s = *current_char;
        strcpy(struct_current->label,"start");
        struct_current->taille = 5;
        struct_current->pere = NULL;
        struct_current->fils = NULL;
        struct_current->frere_g = NULL;
        struct_current->frere_d = NULL;
        // Move the current char pointer 5 chars forward
        *current_char += 5;
        printf("___Start valide___\n");
    } else {
        // Error
        printf("Requete invalide !(start)\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * \fn void fin(char **current_char, node *struct_current)
 * \brief Function to check the presence of 'fin' at the end of the request
 * \param current_char Pointer of pointer to the current char of the request
 * \param struct_current Pointer to the current struct of the chained list. This parameter is initialized to NULL but allocated before the call of this function
*/
void fin(char **current_char, node *struct_current){
    // Check if the current char is 's' and the next 4 chars are 't', 'a', 'r', 't'
    if (**current_char == 'f' && **(current_char + 1) == 'i' && **(current_char + 2) == 'n'){
        // Init the struct (ptr, int...)
        struct_current->s = current_char;
        struct_current->taille = 3;
        struct_current->pere = NULL;
        struct_current->fils = NULL;
        struct_current->frere_d = NULL;
        // Move the current char pointer 5 chars forward
        *current_char += 3;
    } else {
        // Error
        exit(EXIT_FAILURE);
    }
}

/** \fn void mot(char **current_char, node *struct_current)
 * \brief Function to check the composition of a word -> at least one char (a-z/A-Z) and a separator (SP, HTAB, -, _)
 * \param current_char Pointer of pointer to the current char of the request
 * \param struct_current Pointer to the current struct of the chained list. This parameter is initialized to NULL but allocated before the call of this function
*/
void mot(char **current_char, node *struct_current){
    strcpy(struct_current->label, "mot");
    struct_current->s = current_char;
    struct_current->taille = 0;
    while(isalpha(**current_char)){
        *current_char += 1;
        struct_current->taille += 1;
        //on s'arrête si la taille du mot dépasse la taille d'un unsigned int 
        if (struct_current->taille >= 4,294,967,295-1 ){
            // Error
            exit(EXIT_FAILURE);
        }
    }
    // Allocate and init the struct that contains the separator
    node* new_struct = malloc(sizeof(node));
    new_struct->pere = struct_current;
    struct_current->fils = new_struct;
    separateur(current_char, new_struct);
}

/** \fn void nombre(char **current_char, node *struct_current)
 *  \brief Function to check the presence of a number and to stock it in the struct
 *  \param current_char Pointer of pointer to the current char of the request
 *  \param struct_current Pointer to the current struct of the chained list. This parameter is initialized to NULL but allocated before the call of this function
*/
void nombre(char **current_char, node *struct_current){
    strcpy(struct_current->label, "nombre");
    struct_current->s = current_char;
    struct_current->taille = 0;
    while(isdigit(**current_char)){
        *current_char += 1;
        struct_current->taille += 1;
        //on s'arrête si la taille du mot dépasse la taille d'un unsigned int 
        if (struct_current->taille >= 4,294,967,295-1 ){
            // Error
            exit(EXIT_FAILURE);
        }
    }
    struct_current->frere_d = NULL;
    struct_current->fils = NULL;
}

/** \fn void separateur(char **current_char, node *struct_current)
 *  \brief Function to check the presence of a separator (SP, HTAB, -, _)
 * \param current_char Pointer of pointer to the current char of the request
 *  \param struct_current Pointer to the current struct of the chained list. This parameter is initialized to NULL but allocated before the call of this function
*/
void separateur(char **current_char, node *struct_current){
    if (**current_char == '%x20' || **current_char == '%x09' || **current_char == '%x2D' || **current_char == '%x5F'){
        strcpy(struct_current->label, "separateur");
        struct_current->fils = NULL;
        struct_current->frere_g = NULL;
        struct_current->frere_d = NULL;
        struct_current->s = current_char;
        struct_current->taille = 1;
        *current_char += 1;
    } else {
        // Error
        exit(EXIT_FAILURE);
    }
}

/** \fn void ponct(char **current_char, node *struct_current)
 *  \brief Function to check the presence of a punctuation (, / . / ! / ? / :)
 *  \param current_char Pointer of pointer to the current char of the request
 *  \param struct_current Pointer to the current struct of the chained list. This parameter is initialized to NULL but allocated before the call of this function
*/

void ponct(char **current_char, node *struct_current){
    if (**current_char == '%x2C' || **current_char == '%x2F' || **current_char == '%x2E' || **current_char == '%x21' || **current_char == '%x3F' || **current_char == '%x3A'){
        strcpy(struct_current->label, "ponct");
        struct_current->fils = NULL;
        struct_current->frere_d = NULL;
        struct_current->s = current_char;
        struct_current->taille = 1;
        *current_char += 1;
    } else {
        // Error
        exit(EXIT_FAILURE);
    }
}