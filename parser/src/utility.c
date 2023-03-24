#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../headers/utility.h"

char tchar_list[] = {0x21, 0x23, 0x24, 0x25, 0x26, 0x27, 0x2a, 0x2b, 0x2d, 0x2e, 0x5e, 0x5f, 0x60, 0x7c, 0x7e};
char unreserved_list[] = {0x2d, 0x2e, 0x5f, 0x7e};
char sub_delims_list[] = {0x21, 0x24, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x3b, 0x3d};

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
    // print fields
    char *c=struct_current->debut;
    while (c!=(struct_current->fin)+1) {
        printf("%c",*c);
        c++;
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

/** \fn void istchar(char **current_char, node *struct_current, char *label)
 * \brief Check if the char belongs to the list of accepted characted for a tchar
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * \param label : pointer to the array of labels
 * The list of accepted characters is : "!" / "#" / "$" / "%" / "&" / "" / "*" / "+" / "-" / "." / "^" / "_" / "`" / "|" / "~" / DIGIT / ALPHA
*/
int istchar(char c){
    if(isalpha(c) || isdigit(c)){
        return 1;
    } else {
        for(int i=0; i<15; i++){
            if(c == tchar_list[i]){
                return 1;
            }
        }
    }
    return 0;
}

int ispchar(char c){
    if(isunreserved(c) || ispct_encoded(c) || issub_delims(c) || c == ':' || c == '@'){
        return 1;
    }
    return 0;
}

int isunreserved(char c){
    if(isalpha(c) || isdigit(c)){
        return 1;
    } else {
        for(int i=0; i<15; i++){
            if(c == unreserved_list[i]){
                return 1;
            }
        }
    }
    return 0;
}

int ispct_encoded(char **current_char){
    if(**current_char == '%' && isxdigit(*(current_char+1)) && isxdigit(*(current_char+2))){
        return 1;
    }
    return 0;
}

int issub_delims(char **current_char){
    for(int i=0; i<7; i++){
        if(**current_char == sub_delims_list[i]){
            (*current_char)++;
            return 1;
        }
    }
    return 0;
}