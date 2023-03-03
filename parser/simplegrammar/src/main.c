#include <stdio.h>
#include <stdlib.h>
#include "../headers/abnf.h"

/** \fn void print_tree(node *struct_current)
 *  \brief Function to print the tree
 *  \param struct_current Pointer to the first element of the chained list.
*/
int print_tree(node *struct_current, int depth){
    if (strcmp(struct_current->label,"start") || strcmp(struct_current->label,"fin")){
        printf("%s",struct_current->label);
    } else if (strcmp(struct_current->label,"mot")){
        printf("%s",struct_current->label);
    } else if (strcmp(struct_current->label,"nombre")){
        printf("%s",struct_current->label);
    } else if (strcmp(struct_current->label,"separateur")){
        printf("%s",struct_current->label);
    } else if (strcmp(struct_current->label,"ponct")){
        printf("%s",struct_current->label);
    }
}

int main(int argc, char const *argv[])
{
    if(argc<2){printf("Usage : ./main <file to parse>");return 1;}
    return 0;
}
