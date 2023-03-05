#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../headers/utility.h"

/** \file utility.c
 *  \brief Contains useful functions to print and delete the chained list
*/

/** \fn void print_request(node *struct_current, char* first_char, int length)
 *  \brief Function to print the entire parsed request and the chained list
 *  \param struct_current Pointer to the current element of the chained list
 *  \param first_char Pointer to the first char of the request
 *  \param length Length of the request
*/
void print_request(node *struct_current, char* first_char, int length){
    printf("message: %s\n",first_char);
    // call the print_tree function with and indent of 1
    print_tree(struct_current, 1);
}

/** \fn void print_tree(node *struct_current)
 *  \brief Function to print the chained list recursively
 *  \param struct_current Pointer to the current element of the chained list
*/
void print_tree(node *struct_current, int depth){
    // indent
    for (int i = 0; i < depth; i++)
    {
        printf("\t");
    }
    
    // print the label
    printf("%s: ",struct_current->label);
    for (int i = 0; i < struct_current->taille; i++) {
        printf("%c",*(struct_current->s + i));
    }
    printf("\n");
        if (struct_current->fils != NULL) {
        print_tree(struct_current->fils, depth+1);
    }
    if (struct_current->frere != NULL) {
        print_tree(struct_current->frere, depth);
    }
}

/** \fn void delete_chained_list(node *struct_current)
 *  \brief Function to delete the chained list and free the memory
 *  \param struct_current Pointer to the current element of the chained list
*/
void delete_chained_list(node *struct_current){
    if (struct_current->frere != NULL) {
        delete_chained_list(struct_current->frere);
    }
    if (struct_current->fils != NULL) {
        delete_chained_list(struct_current->fils);
    }
    free(struct_current);
}