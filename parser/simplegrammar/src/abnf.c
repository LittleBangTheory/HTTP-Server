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
    if ((**current_char == 's' ||**current_char == 'S') && (*(current_char[0] + 1) == 't'||*(current_char[0] + 1) == 'T') && (*(current_char[0] + 2) == 'a'||*(current_char[0] + 2) == 'A') && (*(current_char[0] + 3) == 'r'||*(current_char[0] + 3) == 'R') && (*(current_char[0] + 4) == 't'||*(current_char[0] + 4) == 'T')){
        // Init the struct (ptr, int...)
        struct_current->s = *current_char;
        strcpy(struct_current->label,"debut");
        struct_current->taille = 5;
        struct_current->fils = NULL;
        struct_current->frere = NULL;

        node *new_struct = malloc(sizeof(node));
        struct_current->fils = new_struct;
        istring(current_char, new_struct, struct_current->taille);
        // The current char pointer is moved 5 chars forward by istring()
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
    if ((**current_char == 'f'||**current_char == 'F') && (*(current_char[0] + 1) == 'i'||*(current_char[0] + 1) == 'I') && (*(current_char[0] + 2) == 'n'||*(current_char[0] + 2) == 'N')){
        // Init the struct (ptr, int...)
        struct_current->s = *current_char;
        struct_current->taille = 3;
        struct_current->fils = NULL;
        struct_current->frere = NULL;
        strcpy(struct_current->label,"fin");

        node *new_struct = malloc(sizeof(node));
        struct_current->fils = new_struct;
        istring(current_char, new_struct, struct_current->taille);
        // The current char pointer is moved 3 chars forward by istring()
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
    struct_current->s = *current_char;
    struct_current->taille = 1;

    node *new_struct_1 = malloc(sizeof(node));
    struct_current->fils = new_struct_1;
    alpha(current_char, new_struct_1);
    // The current char pointer is moved 1 char forward by istring()
    while(isalpha(**current_char)){
        struct_current->taille += 1;
        node *new_struct_2 = malloc(sizeof(node));
        new_struct_1->frere = new_struct_2;
        alpha(current_char, new_struct_2);
        // move one struct forward 
        new_struct_1 = new_struct_2;
        //on s'arrête si la taille du mot dépasse la taille d'un unsigned int 
        if (struct_current->taille >= 4294967295-1){
            // Error
            printf("Word is too long !\n");
            exit(EXIT_FAILURE);
        }
    }
    // Allocate and init the struct that contains the separator
    struct_current->taille += 1;
    node* new_struct = malloc(sizeof(node));
    new_struct_1->frere = new_struct;
    separateur(current_char, new_struct);
}

/** \fn void nombre(char **current_char, node *struct_current)
 *  \brief Function to check the presence of a number and to stock it in the struct
 *  \param current_char Pointer of pointer to the current char of the request
 *  \param struct_current Pointer to the current struct of the chained list. This parameter is initialized to NULL but allocated before the call of this function
*/
void nombre(char **current_char, node *struct_current){
    strcpy(struct_current->label, "nombre");
    struct_current->s = *current_char;
    struct_current->taille = 1;

    node *new_struct_1 = malloc(sizeof(node));
    struct_current->fils = new_struct_1;
    digit(current_char, new_struct_1);
    while(isdigit(**current_char)){
        struct_current->taille += 1;
        node *new_struct_2 = malloc(sizeof(node));
        new_struct_1->frere = new_struct_2;
        digit(current_char, new_struct_2);
        // move one struct forward 
        new_struct_1 = new_struct_2;
        //on s'arrête si la taille du mot dépasse la taille d'un unsigned int 
        if (struct_current->taille >= 4294967295-1 ){
            // Error
            exit(EXIT_FAILURE);
        }
    }
    struct_current->frere = NULL;
    struct_current->fils = NULL;
}

/** \fn void separateur(char **current_char, node *struct_current)
 *  \brief Function to check the presence of a separator (SP, HTAB, -, _)
 * \param current_char Pointer of pointer to the current char of the request
 *  \param struct_current Pointer to the current struct of the chained list. This parameter is initialized to NULL but allocated before the call of this function
*/
void separateur(char **current_char, node *struct_current){
    if (**current_char == '\x20' || **current_char == '\x09' || **current_char == '\x2D' || **current_char == '\x5F'){
        strcpy(struct_current->label, "separateur");
        struct_current->frere = NULL;
        struct_current->s = *current_char;
        struct_current->taille = 1;

        node *new_struct = malloc(sizeof(node));
        struct_current->fils = new_struct;
        icar(current_char, new_struct);
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
    if (**current_char == '\x2C' || **current_char == '\x2E' || **current_char == '\x21' || **current_char == '\x3F' || **current_char == '\x3A'){
        strcpy(struct_current->label, "ponct");
        struct_current->fils = NULL;
        struct_current->frere = NULL;
        struct_current->s = *current_char;
        struct_current->taille = 1;
        
        node *new_struct = malloc(sizeof(node));
        struct_current->fils = new_struct;
        icar(current_char, new_struct);
    } else {
        // Error
        exit(EXIT_FAILURE);
    }
}

/** \fn void alpha(char **current_char, node *struct_current)
 *  \brief Function to save a lone char (a-z/A-Z)
 *  \param current_char Pointer of pointer to the current char of the request
 *  \param struct_current Pointer to the current struct of the chained list.
 * 
*/
void alpha(char **current_char, node *struct_current){
    if (isalpha(**current_char)){
        strcpy(struct_current->label, "__alpha");
        struct_current->fils = NULL;
        struct_current->frere = NULL;
        struct_current->s = *current_char;
        struct_current->taille = 1;
        *current_char += 1;
    } else {
        // Error
        exit(EXIT_FAILURE);
    }
}

/** \fn void digit(char **current_char, node *struct_current)
 *  \brief Function to save a lone digit (0-9)
 *  \param current_char Pointer of pointer to the current char of the request
 *  \param struct_current Pointer to the current struct of the chained list.
 * 
*/
void istring(char **current_char, node *struct_current, int taille){
    strcpy(struct_current->label, "__istring");
    struct_current->fils = NULL;
    struct_current->frere = NULL;
    struct_current->s = *current_char;
    struct_current->taille = taille;
    *current_char += taille;
}

/** \fn void icar(char **current_char, node *struct_current)
 *  \brief Function to save a special character
 *  \param current_char Pointer of pointer to the current char of the request
 *  \param struct_current Pointer to the current struct of the chained list.
*/
void icar(char **current_char, node *struct_current){
    if (1){
        strcpy(struct_current->label, "__icar");
        struct_current->fils = NULL;
        struct_current->frere = NULL;
        struct_current->s = *current_char;
        struct_current->taille = 1;
        *current_char += 1;
    } else {
        // Error
        exit(EXIT_FAILURE);
    }
}

/** \fn void digit(char **current_char, node *struct_current)
 *  \brief Function to save a number (0-9)
 *  \param current_char Pointer of pointer to the current char of the request
 *  \param struct_current Pointer to the current struct of the chained list.
*/
void digit(char **current_char, node *struct_current){
    if (isdigit(**current_char)){
        strcpy(struct_current->label, "__digit");
        struct_current->fils = NULL;
        struct_current->frere = NULL;
        struct_current->s = *current_char;
        struct_current->taille = 1;
        *current_char += 1;
    } else {
        // Error
        return;
        exit(EXIT_FAILURE);
    }
}

/** \fn void lf(char **current_char, node *struct_current)
 *  \brief Function to save a lone LF
 *  \param current_char Pointer of pointer to the current char of the request
 *  \param struct_current Pointer to the current struct of the chained list.
*/
void lf(char **current_char, node *struct_current){
    /*Erreur : **current_char n'est jamais un LF, alors on exit le programme dans la fonction lf()
    J'ai essayé de print **current_char dans le else mais ça ne print jamais le meme char (soit U soit V avec le même input), etrange
    Peut-être une fuite de mémoire*/
    if (**current_char == '\n'){
        strcpy(struct_current->label, "__lf");
        struct_current->fils = NULL;
        struct_current->frere = NULL;
        struct_current->s = *current_char;
        struct_current->taille = 1;
        *current_char += 1;
    } else {
        // Error
        exit(EXIT_FAILURE);
    }
}