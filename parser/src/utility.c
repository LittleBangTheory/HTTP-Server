#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../headers/utility.h"

/** \file utility.c
 *  \brief Contains useful functions to print and delete the chained list
*/

/** \var char tchar_list[]
 * \brief List of accepted characters for a tchar
 * The list of accepted characters is : "!" / "#" / "$" / "%" / "&" / "'" / "*" / "+" / "-" / "." / "^" / "_" / "`" / "|" / "~"
*/
char tchar_list[] = {0x21, 0x23, 0x24, 0x25, 0x26, 0x27, 0x2a, 0x2b, 0x2d, 0x2e, 0x5e, 0x5f, 0x60, 0x7c, 0x7e};

/** \var char unreserved_list[]
 * \brief List of accepted characters for a unreserved
 * The list of accepted characters is : "-" / "." / "_" / "~"
*/
char unreserved_list[] = {0x2d, 0x2e, 0x5f, 0x7e};

/** \var char sub_delims_list[]
 * \brief List of accepted characters for a sub-delims
 * The list of accepted characters is : "!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" / "," / ";" / "="
*/
char sub_delims_list[] = {0x21, 0x24, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x3b, 0x3d};

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
    while (c!=(struct_current->fin)+1 && (c<=struct_current->fin)) {
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
 *  NOTE : This function is not used, as it has been replaced by the purge_tree() function in api.c
*/
/*
void delete_chained_list(node *struct_current){
    if (struct_current->frere != NULL) {
        delete_chained_list(struct_current->frere);
    }
    if (struct_current->fils != NULL) {
        delete_chained_list(struct_current->fils);
    }
    free(struct_current);
}
*/

/** \fn void istchar(char **current_char, node *struct_current, char *label)
 * \brief Check if the char belongs to the list of accepted characted for a tchar
 * \param c : char to check
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

/** \fn void isunreserved(char **current_char, node *struct_current, char *label)
 * \brief Check if the char belongs to the list of accepted characted for a unreserved
 * \param c : char to check
*/
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

/** \fn void ispct_encoded(char **current_char, node *struct_current, char *label)
 * \brief Check if the char belongs to the list of accepted characted for a pct_encoded
 * \param c : char to check
 * Only check if the first char is a %, too risky to check to chars forward, could be out of the string
*/
int ispct_encoded(char c){
    if(c == '%'){
        return 1;
    }
    return 0;
}

/** \fn void issub_delims(char **current_char, node *struct_current, char *label)
 * \brief Check if the char belongs to the list of accepted characted for a sub_delims
 * \param c : char to check
*/
int issub_delims(char c){
    for(int i=0; i<7; i++){
        if(c == sub_delims_list[i]){
            return 1;
        }
    }
    return 0;
}

/** \fn void ispchar(char **current_char, node *struct_current, char *label)
 * \brief Check if the char belongs to the list of accepted characted for a pchar
 * \param c : char to check
*/
int ispchar(char c){
    if(isunreserved(c) || ispct_encoded(c) || issub_delims(c) || c == ':' || c == '@'){
        return 1;
    }
    return 0;
}

/** \fn void isvchar(char **current_char, node *struct_current, char *label)
 * \brief Check if the char belongs to the list of accepted characted for a vchar
 * \param c : char to check
 * 
*/
int isvchar(char c){
    // check for every char between 0x21 and 0x7e
    if(c >= 0x21 && c <= 0x7e){
        return 1;
    }
    return 0;
}

/** \fn void isobs_text(char **current_char, node *struct_current, char *label)
 * \brief Check if the char belongs to the list of accepted characted for a obs_text (%x80-FF)
 * \param c : char to check
 * 
*/
int isobs_text(char c){
    // check for every char between 0x80 and 0xff
    if(c >= 0x80 && c <= 0xff){
        return 1;
    }
    return 0;
}

/** \fn void isobs_fold(char *current_char, node *struct_current, char *label)
 * \brief Check if the char belongs to the list of accepted characted for a obs_fold (%x0D %x0A *(%x20 / %x09))
 * \param current_char : char to check
*/
int isobs_fold(char *current_char){
    if(*current_char == '\r' && *current_char+1 == '\n' && *current_char+2 == ' ' || *current_char+2 == '\t'){
        return 1;
    }
    return 0;
}

/** \fn void isconnection_end(char *current_char)
 * \brief Check if the char belongs to the list of accepted characted for a connection_end (OWS ",")
 * \param current_char : char to check
*/
int isconnection_end(char *current_char){
    while(*current_char == 0x20 || *current_char == 0x09){
        current_char++;
    }
    if(*current_char == ','){
        return 1;
    } else {
        return 0;
    }
}

/** \fn void isheader_end(char *current_char)
 * \brief Check if the char belongs to the list of accepted characted for a header_end (OWS CRLF)
 * \param current_char : char to check
*/
int isheader_end(char *current_char){
    while(*current_char == 0x20 || *current_char == 0x09){
        current_char++;
    }
    if(*current_char == '\r' && *current_char+1 == '\n'){
        return 1;
    } else {
        return 0;
    }
}