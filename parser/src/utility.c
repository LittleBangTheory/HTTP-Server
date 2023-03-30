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
char unreserved_list[] = {'-', '.', '_', '~'};

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


/** \fn void print_tree(node *struct_current)
 *  \brief Function to print the chained list recursively
 *  \param struct_current Pointer to the current element of the chained list
*/
void print_tree(node *struct_current, int depth){
    // indent
    for (int i = 0; i < depth*4; i++)
    {
        printf(" ");
    }
    
    // print the label
    printf("[%d:%s] = ",depth,struct_current->label);
    // print fields
    char *c=struct_current->debut;
    printf("\"");
    while (c!=NULL && c!=(struct_current->fin)+1 && (c<=struct_current->fin)) {
        printf("%c",*c);
        c++;
    }
    printf("\"");
    printf("\n");
        if (struct_current->fils != NULL) {
        print_tree(struct_current->fils, depth+1);
    }
    if (struct_current->frere != NULL) {
        print_tree(struct_current->frere, depth);
    }
}

/** \fn int istchar(char **current_char, node *struct_current, char *label)
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

/** \fn int isunreserved(char **current_char, node *struct_current, char *label)
 * \brief Check if the char belongs to the list of accepted characted for a unreserved
 * \param c : char to check
*/
int isunreserved(char c){
    if(isalpha(c) || isdigit(c)){
        return 1;
    } else {
        for(int i=0; i<4; i++){
            if(c == unreserved_list[i]){
                return 1;
            }
        }
    }
    return 0;
}   

/** \fn int ispct_encoded(char **current_char, node *struct_current, char *label)
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

/** \fn int issub_delims(char **current_char, node *struct_current, char *label)
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

/** \fn int ispchar(char **current_char, node *struct_current, char *label)
 * \brief Check if the char belongs to the list of accepted characted for a pchar
 * \param c : char to check
*/
int ispchar(char c){
    if(isunreserved(c) || ispct_encoded(c) || issub_delims(c) || c == ':' || c == '@'){
        return 1;
    }
    return 0;
}

/** \fn int isvchar(char **current_char, node *struct_current, char *label)
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

/** \fn int isobs_text(char **current_char, node *struct_current, char *label)
 * \brief Check if the char belongs to the list of accepted characted for a obs_text (%x80-FF)
 * \param c : char to check
 * 
*/
int isobs_text(char c){
    // check for every char between 0x80 and 0xff, with a logic AND
    if( 0x80 <= (c & 0xff) && (c & 0xff) <= 0xff){
        return 1;
    }
    return 0;
}

/** \fn int isobs_fold(char *current_char, node *struct_current, char *label)
 * \brief Check if the char belongs to the list of accepted characted for a obs_fold (%x0D %x0A *(%x20 / %x09))
 * \param current_char : char to check
*/
int isobs_fold(char *current_char){
    if(*current_char == '\r' && *(current_char+1) == '\n' && (*(current_char+2) == ' ' || *(current_char+2) == '\t')){
        return 1;
    }
    return 0;
}

/** \fn int isconnection_end(char *current_char)
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

/** \fn int isheader_end(char *current_char)
 * \brief Check if the char belongs to the list of accepted characted for a header_end (OWS CRLF)
 * \param current_char : char to check
*/
int isheader_end(char *current_char){
    while(*current_char == 0x20 || *current_char == 0x09){
        current_char++;
    }
    if(*current_char == '\r' && *(current_char+1) == '\n'){
        return 1;
    } else {
        return 0;
    }
}

/** \fn int isipv4(char *current_char)
 * \brief Check if the char and its followers are a valid ipv4 address (dec-octet "." dec-octet "." dec-octet "." dec-octet)
 * \param current_char : char to check
*/
int isipv4address(char *current_char){
    int n = 0;
    for(int i=0; i<4; i++){
        n = 0;
        if(!isdigit(*current_char)){
            return 0;
        }
        while(isdigit(*current_char) && n<4){
            current_char++;
        }
        if(*current_char != '.' && i < 3){
            return 0;
        }
        current_char++;
    }
    return 1;
}

/** \fn isip_literal(char *current_char)
 * \brief Check if the char and its followers are a valid ip_literal ( "[" ( IPv6address / IPvFuture  ) "]" ) -> call is ipv6 and isipvfuture
 * \param current_char : char to check
*/
int isip_literal(char *current_char){
    if(*current_char == '['){
        current_char++;
        if(isipv6address(current_char) || isipvfuture(current_char)){
            return 1;
        }
    }   
    return 0;
}

/** \fn isipv6(char *current_char)
 * \brief Check if the char and its followers are a valid ipv6 address
 * \param current_char : char to check
 * We only check if the beginning of the address is valid, the rest is checked by the function ipv6address
*/
int isipv6address(char *current_char){
    if(*current_char == ':'){
        current_char++;
        if(*current_char == ':'){
            current_char++;
            if(isls32(current_char) || ish16(current_char)){
                return 1;
            } 
        }
    } else if(ish16(current_char)){
        current_char++;
        if(*current_char == ':'){
            return 1;
        }
    }
    return 0;
}

/** \fn isipvfuture(char *current_char)
 * \brief Check if the char and its followers are a valid ipvfuture ( "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" ) )
 * \param current_char : char to check
*/
int isipvfuture(char *current_char){
    if(*current_char == 'v'){
        current_char++;
        if(isxdigit(*current_char)){
            current_char++;
            while(isxdigit(*current_char)){
                current_char++;
            }
            if(*current_char == '.'){
                current_char++;
                if(isunreserved(*current_char) || issub_delims(*current_char) || *current_char == ':'){
                    current_char++;
                    while(isunreserved(*current_char) || issub_delims(*current_char) || *current_char == ':'){
                        current_char++;
                    }
                    return 1;
                }
            }
        }
    }
    return 0;
}

/** \fn int ish16(char *current_char)
 * \brief Check if the char and its followers are a valid h16 ( 1*4HEXDIG )
 * \param current_char : char to check
*/
int ish16(char *current_char){
    if(isxdigit(*current_char)){
        current_char++;
        while(isxdigit(*current_char)){
            current_char++;
        }
        return 1;
    }
    return 0;
}

/** \fn int isls32(char *current_char)
 * \brief Check if the char and its followers are a valid ls32 ( ( h16 ":" h16 ) / IPv4address ) 
 * \param current_char : char to check
 * Important to check if the char after the ls32 is ']', otherwise it could be mistaken as a simple h16 ":" h16 in a ipv6 address
*/
int isls32(char *current_char){
    if(ish16(current_char) && *(current_char+1) == ':' && ish16(current_char+2) && *(current_char+3) == ']'){
        return 1;
    } else if(isipv4address(current_char)){
        return 1;
    }
    return 0;
}

/** \fn int ismedia_type_end(char *current_char)
 * \brief Check if the char belongs to the list of accepted chars for a media_type_end (OWS CRLF)
 * \param current_char : char to check
*/
int ismedia_type_end(char *current_char){
    while(*current_char == 0x20 || *current_char == 0x09){
        current_char++;
    }
    if(*current_char == '\r' && *(current_char+1) == '\n'){
        return 1;
    } else {
        return 0;
    }
}

/** \fn int isqdtext(char c)
 * \brief Check if the char belongs to the list of accepted characted for a qdtext
 * \param c : char to check
*/
int isqdtext(char c){
    if(c == 0x20 || c == 0x09 || c == 0x21 || (c >= 0x23 && c <= 0x5b) || (c >= 0x5d && c <= 0x7e) || isobs_text(c)){
        return 1;
    }
    return 0;
}

/** \fn int iscookie_octet(char c)
 * \brief Check if the char belongs to the list of accepted characted for a cookie_octet
 * \param c : char to check
 * Chars accepted : 0x21, 0x23-0x2b, 0x2d-0x3a, 0x3c-0x5b, 0x5d-0x7e
*/
int iscookie_octet(char c){
    if(c == 0x21 || ((0x23 <= c) && (c <= 0x2b)) || ((0x2d <= c) && (c<= 0x3a)) || ((0x3c <= c) && (c <= 0x5b)) || ((0x5d <= c) && (c <= 0x7e))){
        return 1;
    }
    return 0;
}

/** \fn int stringcompare(char *current_char, char *s2)
 * \brief Check if the char and its followers are equal to the string s2
 * \param current_char : char to check
 * \param s2 : string to compare to
*/
int stringcompare(char *current_char, char *s){
    while(*s != '\0'){
        if(*current_char != *s){
            return 0;
        }
        current_char++;
        s++;
    }
    return 1;
}

/** \fn int istransfer_extension_end(char *current_char)
 * \brief Check if the char belongs to the list of accepted chars for a transfer_extension_end (OWS ";")
 * \param current_char : char to check
*/
int istransfer_extension_end(char *current_char){
    while(*current_char == 0x20 || *current_char == 0x09){
        current_char++;
    }
    if(*current_char == ';'){
        return 1;
    } else {
        return 0;
    }
}

/** \fn int istransfer_encoding_end(char *current_char)
 * \brief Check if the char belongs to the list of accepted chars for a transfer_encoding_end (OWS ",")
 * \param current_char : char to check
*/
int istransfer_encoding_end(char *current_char){
    while(*current_char == 0x20 || *current_char == 0x09){
        current_char++;
    }
    if(*current_char == ','){
        return 1;
    } else {
        return 0;
    }
}