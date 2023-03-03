#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/utility.h"
#include "../headers/abnf.h"


void print_request(node *struct_current, char** first_char, int length){
    printf("message: ");
    for (int i = 0; i < length; i++) {
        printf("%s",*(first_char + i));
    }
    // call the print_tree function with and indent of 1
    print_tree(struct_current, 1);
}

/** \fn void print_tree(node *struct_current)
 *  \brief Function to print the tree
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
    
    for (int i = 0; i < struct_current->taille; i++) {
        // indent further
        for (int j = 0; j < depth+1; j++)
        {
            printf("\t");
        }
        if (isalpha(*(struct_current->s + i))) {
            printf("__alpha: %c",*(struct_current->s + i));
        } else if (isdigit(*(struct_current->s + i))) {
            printf("__digit: %c",*(struct_current->s + i));
        } else if (strcmp(struct_current->label,"separateur") || strcmp(struct_current->label,"ponct")) {
            if (struct_current->fils == NULL) {}
                printf("__icar: %c",*(struct_current->s + i));
            } else if (strcmp((struct_current->fils)->label, struct_current->label)) {
                print_tree(struct_current->fils, depth+1);
            } 
        }
    if (struct_current->frere_d != NULL) {
        print_tree(struct_current->frere_d, depth);
    }
}

/** \fn void delete_chained_list(node *struct_current)
 *  \brief Function to delete the chained list
 *  \param struct_current Pointer to the current element of the chained list
*/
void delete_chained_list(node *struct_current){
    if (struct_current->frere_d != NULL) {
        delete_chained_list(struct_current->frere_d);
    }
    if (struct_current->fils != NULL) {
        delete_chained_list(struct_current->fils);
    }
    free(struct_current);
}