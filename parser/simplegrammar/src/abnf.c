#include <stdlib.h>
#include <stdio.h>
#include "../headers/abnf.h"

/**
 * \file abnf.c
 * \brief ABNF parser 
*/

/**
 * \brief Start function
 * \param current_char Pointer of pointer to the current char of the request
 * \param struct_current Pointer to the current struct of the chained list. This parameter is initialized to NULL but allocated before the call of this function
*/
void start(int **current_char, node *struct_current){
    // Check if the current char is 's' and the next 4 chars are 't', 'a', 'r', 't'
    if (**current_char == 's' && **(current_char + 1) == 't' && **(current_char + 2) == 'a' && **(current_char + 3) == 'r' && **(current_char + 4) == 't'){
        // Init the struct (ptr, int...)
        struct_current->s = current_char;
        struct_current->taille = 5;
        struct_current->pere = NULL;
        struct_current->fils = NULL;
        struct_current->frere_g = NULL;
        struct_current->frere_d = NULL;
        // Move the current char pointer 5 chars forward
        *current_char += 5;
    } else {
        // Error
        exit(EXIT_FAILURE);
    }
}