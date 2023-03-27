/** \file abnf.c
 * \brief Contains the functions to parse the request
 * General rules :
 * - The function name is the name of the rule in the RFC
 * - The function takes 3 parameters : a pointer to the current char, a pointer to the current struct and a pointer to the array of labels
 * - The function returns nothing
 * - Every time a new structure is needed (to store a brother or a son), it is allocated by the father function, but initialized by the son or father function. Example : token() creates the struct for tchar(), but tchar() initializes it
 * - Every time brothers are needed, the father function calls the brother functions and link the brothers together. Example : request-line() creates the struct for method(), sp(), request-target(), sp() http_version(), and crlf(), and link them together
 * - If the reading pointer needs to be moved, it should be moved by the highest function possible. Example : tchar d   oesnt move the reading pointer, but token() does, exepted for the last one, that is moved by method(), etc.
 * - The end of the struct is known when the son functions are done
 * - Always check in a function that the character(s) to store is/are the right one(s) (example : in tchar(), check that the character is a tchar)
*/

/* TODO :
- Check that our syntax corresponds to the one of the teacher (example : __alpha instead of alpha)
- Write a bash script to execute every test and compare the output with the expected one
- Write ipv6()
*/

/* NOTE :
- Those simple tests need to be failed (the requests are incorrect) :  6, 8, 9, 10, and 12 
- Those simple tests need to be passed (the requests are correct) : 1, 2, 3, 4, 5, 7, 11
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../headers/abnf.h"
#include "../headers/utility.h"

/** \fn void http_message(char **current_char, node *struct_current)
 * \brief Parse the http message
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
*/
void http_message(char **current_char, node *struct_current){
    // Init the struct (ptr, int...), and allocate memory for the first child
    struct_current->debut = *current_char;
    struct_current->label = HTTP_MESSAGE;
    struct_current->fils = NULL;

    // Allocate memory for the first child
    node *new_struct_1 = malloc(sizeof(node));
    struct_current->fils = new_struct_1;

    // Call the function for the first child, supposed to be start-line
    start_line(current_char, new_struct_1);
    *current_char += 1;

    // Allocate memory for the second child. Whatever the next field is, we know there will be a brother
    node *new_struct_2;

    // If the next char is not a crlf, we have a header-field
    if(**current_char != '\r' || *(*current_char+1) != '\n'){
        do{
            new_struct_2 = malloc(sizeof(node));
            new_struct_1->frere = new_struct_2;
            // Call the function for the second child, supposed to be header-field
            header_field(current_char, new_struct_2);
            *current_char += 1;

            // Allocate memory for the third child
            new_struct_1 = new_struct_2;
            new_struct_2 = malloc(sizeof(node));
            new_struct_1->frere = new_struct_2;

            // Call the function for the third child, supposed to be crlf
            crlf(current_char, new_struct_2);

            new_struct_1 = new_struct_2;
            *current_char += 1;
        // If the next char is not a crlf, we have another header-field
        }while(**current_char != '\r' || *(*current_char+1) != '\n');
    }

    // Allocate memory for the fourth child, supposed to be crlf
    new_struct_1 = new_struct_2;
    new_struct_2 = malloc(sizeof(node));
    new_struct_1->frere = new_struct_2;

    // Call the function for the fourth child, supposed to be crlf
    crlf(current_char, new_struct_2);
    *current_char += 1;

    // If we have an octet, we have a message-body
    if(**current_char != '\0'){
        new_struct_1 = new_struct_2;
        new_struct_2 = malloc(sizeof(node));
        new_struct_1->frere = new_struct_2;

        // Call the function for the fifth child, supposed to be message-body
        message_body(current_char, new_struct_2);
    }
}

/** \fn void message_body(char **current_char, node *struct_current)
 * \brief Parse the message body
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void message_body(char **current_char, node *struct_current){
    // Init the struct (ptr, int...), and allocate memory for the first child
    struct_current->debut = *current_char;
    struct_current->label = MESSAGE_BODY;
    struct_current->fils = NULL;

    // Allocate memory for the first child
    node *new_struct_1 = malloc(sizeof(node));
    node *new_struct_2;
    struct_current->fils = new_struct_1;

    while(**current_char != '\0'){
        // Call the function for the first child, supposed to be octet
        octet(current_char, new_struct_1);

        if(*(*current_char + 1) != '\0'){
            *current_char += 1;
            // Allocate memory for the second child
            new_struct_2 = new_struct_1;
            new_struct_1 = malloc(sizeof(node));
            new_struct_2->frere = new_struct_1;
        }
    }
}

/** \fn void octet(char **current_char, node *struct_current)
 * \brief Parse the octet
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
*/
void octet(char **current_char, node *struct_current){
    // Init the struct (ptr, int...), and allocate memory for the first child
    struct_current->debut = *current_char;
    struct_current->label = OCTET;
    struct_current->fils = NULL;
    struct_current->fin = *current_char;
}

/** \fn header_field(char **current_char, node *struct_current)
 * \brief Parse the header field
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * WORK IN PROGRESS, TO COMMENT IN ORDER TO TEST EVERYTHING ELSE
*/
void header_field(char **current_char, node *struct_current){
    // Init the struct (ptr, int...), and allocate memory for the first child
    struct_current->debut = *current_char;
    struct_current->label = HEADER_FIELD;
    struct_current->fils = NULL;

    // Allocate memory for the first child
    node *new_struct_1 = malloc(sizeof(node));
    struct_current->fils = new_struct_1;

    // Call the function for the first child, supposed to be field-name
    field_name(current_char, new_struct_1);
    *current_char += 1;

    if(**current_char != ':'){
        printf("Error : expected ':' in header-field, got '%c' instead\n", **current_char);
        exit(1);
    }
    // Allocate memory for the second child
    node *new_struct_2 = malloc(sizeof(node));
    new_struct_1->frere = new_struct_2;

    // Call the function for the second child, supposed to be ':'
    icar(current_char, new_struct_2);
    *current_char += 1;

    // Keep the pointer to new_struct_1 to identify the header type after
    node *new_struct_3 = malloc(sizeof(node));
    new_struct_2->frere = new_struct_3;

    if(**current_char == 0x20 || **current_char == 0x09) {
        // Call the function for the third child, supposed to be OWS
        ows(current_char, new_struct_3);
    }
    *current_char += 1;

    //Treat every case for header_field : Connection-header / Content-Length-header / Content-Type-header / Cookie-header / Transfer-Encoding-header / Expect-header / Host-header / ( field-name ":" OWS field-value OWS ) 
    if(strcmp(new_struct_1->label, "Connection") == 0){
        // Call the function for the fourth child, supposed to be Connection-header
        connection_header(current_char, new_struct_3);
    }
    else if(strcmp(new_struct_1->label, "Content-Length") == 0){
        // Call the function for the fourth child, supposed to be Content-Length-header
        content_length_header(current_char, new_struct_3);
    }
    else if(strcmp(new_struct_1->label, "Content-Type") == 0){
        // Call the function for the fourth child, supposed to be Content-Type-header
        content_type_header(current_char, new_struct_3);
    }
    else if(strcmp(new_struct_1->label, "Cookie") == 0){
        // Call the function for the fourth child, supposed to be Cookie-header
        cookie_header(current_char, new_struct_3);
    }
    else if(strcmp(new_struct_1->label, "Transfer-Encoding") == 0){
        // Call the function for the fourth child, supposed to be Transfer-Encoding-header
        transfer_encoding_header(current_char, new_struct_3);
    }
    else if(strcmp(new_struct_1->label, "Expect") == 0){
        // Call the function for the fourth child, supposed to be Expect-header
        expect_header(current_char, new_struct_3);
    }
    else if(strcmp(new_struct_1->label, "Host") == 0){
        // Call the function for the fourth child, supposed to be Host-header
        host_header(current_char, new_struct_3);
    }
    else{
        // Allocate memory for the fourth child
        new_struct_2 = new_struct_3;
        new_struct_3 = malloc(sizeof(node));
        new_struct_2->frere = new_struct_3;

        // Call the function for the fourth child, supposed to be field-value
        field_value(current_char, new_struct_3);
    }

    if(*(*current_char+1) == 0x20 || *(*current_char+1) == 0x09) {
        *current_char += 1;
        // Allocate memory for the fifth child
        new_struct_1 = new_struct_2;
        new_struct_2 = malloc(sizeof(node));
        new_struct_1->frere = new_struct_2;

        // Call the function for the fifth child, supposed to be OWS
        ows(current_char, new_struct_2);
    }

    // The end of the struct is known when the son functions are done
    struct_current->fin = *current_char;
}

/** \fn void connection(char **current_char, node *struct_current)
 * \brief Parse the connection header
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void connection_header(char **current_char, node *struct_current){
    // Connection = *( "," OWS ) connection-option *( OWS "," [ OWS connection-option ] )
    // Init the struct (ptr, int...), and allocate memory for the first child
    struct_current->debut = *current_char;
    struct_current->label = CONNECTION;
    struct_current->fils = NULL;

    // Allocate memory for the first child
    node *new_struct_1 = malloc(sizeof(node));
    node *new_struct_2;
    struct_current->fils = new_struct_1;

    // Call the function for ',' and OWS
    while(**current_char == ',' && (*(*current_char+1) == 0x20 || *(*current_char+1) == 0x09)){
        // Call the function for ','
        icar(current_char, new_struct_1);
        *current_char += 1;

        // Allocate memory for the second child
        new_struct_2 = new_struct_1;
        new_struct_1 = malloc(sizeof(node));
        new_struct_2->frere = new_struct_1;

        // Call the function for OWS
        ows(current_char, new_struct_2);
        *current_char += 1;

        // Allocate memory for the third child
        new_struct_2 = new_struct_1;
        new_struct_1 = malloc(sizeof(node));
        new_struct_2->frere = new_struct_1;
    }
    
    // Call the function for connection-option
    connection_option(current_char, new_struct_1);
    *(current_char)+=1;

    while(isconnection_end(*current_char)){
        // Allocate memory for ows
        new_struct_2 = new_struct_1;
        new_struct_1 = malloc(sizeof(node));
        new_struct_2->frere = new_struct_1;
        ows(current_char, new_struct_1);
        *current_char += 1;

        // Allocate memory for ','
        new_struct_2 = new_struct_1;
        new_struct_1 = malloc(sizeof(node));
        new_struct_2->frere = new_struct_1;
        icar(current_char, new_struct_1);
        *current_char+=1;

        // If the next element is a [OWS connection-option] (optional)
        if(!isheader_end(*current_char) && !isconnection_end(*current_char)){
            // Allocate memory for ows
            new_struct_2 = new_struct_1;
            new_struct_1 = malloc(sizeof(node));
            new_struct_2->frere = new_struct_1;

            // Call the function for OWS
            ows(current_char, new_struct_1);
            *current_char += 1;

            // Allocate memory for connection-option
            new_struct_2 = new_struct_1;
            new_struct_1 = malloc(sizeof(node));
            new_struct_2->frere = new_struct_1;

            // Call the function for connection-option
            connection_option(current_char, new_struct_1);
            *current_char += 1;
        }
    }

    // The end of the struct is known when the son functions are done
    *current_char -= 1;
    struct_current->fin = *current_char;
}

/** \fn void host_header(char **current_char, node *struct_current)
 * \brief Parse the host header
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void host_header(char **current_char, node *struct_current){
    // Host = uri_host [ ":" host-port ]
    // Init the struct (ptr, int...), and allocate memory for the first child
    struct_current->debut = *current_char;
    struct_current->label = HOST;
    struct_current->fils = NULL;

    // Allocate memory for the first child
    node *new_struct_1 = malloc(sizeof(node));
    struct_current->fils = new_struct_1;

    // Call the function for the first child, supposed to be host-name
    uri_host(current_char, new_struct_1);

    // If the next element is a ':' (optional)
    if(*(*current_char+1) == ':'){
        *current_char += 1;
        // Allocate memory for the second child
        node *new_struct_2 = malloc(sizeof(node));
        new_struct_1->frere = new_struct_2;

        // Call the function for the second child, supposed to be ':'
        icar(current_char, new_struct_2);
        *current_char += 1;

        // Allocate memory for the third child
        node *new_struct_3 = malloc(sizeof(node));
        new_struct_2->frere = new_struct_3;

        // Call the function for the third child, supposed to be host-port
        host_port(current_char, new_struct_3);
    }

    // The end of the struct is known when the son functions are done
    struct_current->fin = *current_char;
}

/** \fn void uri_host(char **current_char, node *struct_current)
 * \brief Parse the uri-host
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void uri_host(char **current_char, node *struct_current){
    // uri-host = IP-literal / IPv4address / reg-name
    // Init the struct (pParse the hosttr, int...), and allocate memory for the first child
    struct_current->debut = *current_char;
    struct_current->label = URI_HOST;
    struct_current->fils = NULL;

    // Allocate memory for the first child
    node *new_struct_1 = malloc(sizeof(node));
    struct_current->fils = new_struct_1;

    // Call host
    host(current_char, new_struct_1);

    // The end of the struct is known when the son functions are done
    struct_current->fin = *current_char;
}

/** \fn void host(char **current_char, node *struct_current)
 * \brief Parse the host
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void host(char **current_char, node *struct_current){
    // host = hostname / IPv4address / IPv6reference
    // Init the struct (ptr, int...), and allocate memory for the first child
    struct_current->debut = *current_char;
    struct_current->label = HOST;
    struct_current->fils = NULL;

    // Allocate memory for the first child
    node *new_struct_1 = malloc(sizeof(node));
    struct_current->fils = new_struct_1;

    // Call the function for the first child, supposed to be IP-literal / IPv4address / reg-name
    if(isip_literal(*current_char)){
        ip_literal(current_char, new_struct_1);
    }else if(isipv4address(*current_char)){
        ipv4address(current_char, new_struct_1);
    }else{
        reg_name(current_char, new_struct_1);
    }

    // The end of the struct is known when the son functions are done
    struct_current->fin = *current_char;
}

/** \fn ipv4address(char **current_char, node *struct_current)
 * \brief Parse the ipv4address
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 *
*/
void ipv4address(char **current_char, node *struct_current){
    // IPv4address = dec-octet "." dec-octet "." dec-octet "." dec-octet
    // Init the struct (ptr, int...), and allocate memory for the first child
    struct_current->debut = *current_char;
    struct_current->label = IPV4ADDRESS;
    struct_current->fils = NULL;

    // Allocate memory for the first child
    node *new_struct_1 = malloc(sizeof(node));
    node *new_struct_2;
    struct_current->fils = new_struct_1;

    for(int i=0; i<4; i++){
        // Call the function for the first child, supposed to be dec-octet
        dec_octet(current_char, new_struct_1);
        *current_char += 1;

        // Allocate memory for the second child
        new_struct_2 = new_struct_1;
        new_struct_1 = malloc(sizeof(node));
        new_struct_2->frere = new_struct_1;

        // Call the function for the second child, supposed to be '.'
        if(**current_char != '.'){
            printf("Error : ipv4address : '.' expected");
            exit(1);
        }
        icar(current_char, new_struct_1);

        // Allocate memory for the third child
        if(i != 3){
            new_struct_2 = new_struct_1;
            new_struct_1 = malloc(sizeof(node));
            new_struct_2->frere = new_struct_1;
        }

    }
}

/** \fn void dec_octet(char **current_char, node *struct_current)
 * \brief Parse the dec-octet
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
*/
void dec_octet(char **current_char, node *struct_current){
    // TODO
}

/** \fn void ip_literal(char **current_char, node *struct_current)
 * \brief Parse the ip-literal
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
*/
void ip_literal(char **current_char, node *struct_current){
    // IP-literal = "[" ( IPv6address / IPvFuture  ) "]"
    // Init the struct (ptr, int...), and allocate memory for the first child
    struct_current->debut = *current_char;
    struct_current->label = IP_LITERAL;
    struct_current->fils = NULL;

    // Allocate memory for the first child
    node *new_struct_1 = malloc(sizeof(node));
    struct_current->fils = new_struct_1;

    // Call the function for the first child, supposed to be '['
    if(**current_char == '['){
        icar(current_char, new_struct_1);
        *current_char += 1;
    } else {
        printf("Error : '[' expected, %c found", **current_char);
        exit(1);
    }

    // Allocate memory for the second child
    node *new_struct_2 = malloc(sizeof(node));
    new_struct_1->frere = new_struct_2;

    // Call the function for the second child, supposed to be IPv6address / IPvFuture
    if(isipv6address(*current_char)){
        ipv6address(current_char, new_struct_2);
    }else{
        ipvfuture(current_char, new_struct_2);
    }

    // Allocate memory for the third child
    node *new_struct_3 = malloc(sizeof(node));
    new_struct_2->frere = new_struct_3;

    // Call the function for the third child, supposed to be ']'
    if(**current_char == ']'){
        icar(current_char, new_struct_1);
    } else {
        printf("Error : ']' expected, %c found", **current_char);
        exit(1);
    }

    // The end of the struct is known when the son functions are done
    struct_current->fin = *current_char;
}

/** \fn void ipv6address(char **current_char, node *struct_current)
 *  \brief Function to parse an IPv6address
 *  \param current_char : pointer to the current char
 *  \param struct_current : pointer to the current struct
*/
void ipv6address(char **current_char, node *struct_current){
    // TODO 
}

/** \fn void ipvfuture(char **current_char, node *struct_current)
 *  \brief Function to parse an IPvFuture
 *  \param current_char : pointer to the current char
 *  \param struct_current : pointer to the current struct
*/
void ipvfuture(char **current_char, node *struct_current){
    // IPvFuture     = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )

    // Init the struct (ptr, int...), and allocate memory for the first child
    struct_current->debut = *current_char;
    struct_current->label = IPVFUTURE;
    struct_current->fils = NULL;

    // Allocate memory for the first child
    node *new_struct_1 = malloc(sizeof(node));
    struct_current->fils = new_struct_1;

    // Call the function for the first child, supposed to be 'v'
    if(**current_char == 'v'){
        icar(current_char, new_struct_1);
        *current_char += 1;
    } else {
        printf("Error : 'v' expected, %c found", **current_char);
        exit(1);
    }

    // Allocate memory for the second child
    node *new_struct_2 = malloc(sizeof(node));
    new_struct_1->frere = new_struct_2;

    // Call the function for the second child, supposed to be 1*HEXDIG
    while(isxdigit(**current_char)){
        hexdig(current_char, new_struct_2);
        *current_char += 1;

        // Allocate memory for the next child
        if(isxdigit(**current_char)){
            new_struct_1 = new_struct_2;
            new_struct_2 = malloc(sizeof(node));
            new_struct_1->frere = new_struct_2;
        }
    }

    // Allocate memory for the third child
    new_struct_1 = new_struct_2;
    new_struct_2 = malloc(sizeof(node));
    new_struct_1->frere = new_struct_2;

    // Call the function for the third child, supposed to be '.'
    if(**current_char == '.'){
        icar(current_char, new_struct_2);
        *current_char += 1;
    } else {
        printf("Error : '.' expected, %c found", **current_char);
        exit(1);
    }

    // Allocate memory for the fourth child
    new_struct_1 = new_struct_2;
    new_struct_2 = malloc(sizeof(node));
    new_struct_1->frere = new_struct_2;

    // Call the function for the fourth child, supposed to be 1*( unreserved / sub-delims / ":" )
    while(isunreserved(**current_char) || issubdelims(**current_char) || **current_char == ':'){
        if(isunreserved(**current_char)){
            unreserved(current_char, new_struct_2);
        }else if(issubdelims(**current_char)){
            subdelims(current_char, new_struct_2);
        }else{
            icar(current_char, new_struct_2);
        }
        *current_char += 1;

        // Allocate memory for the next child
        if(isunreserved(**current_char) || issubdelims(**current_char) || **current_char == ':'){
            new_struct_1 = new_struct_2;
            new_struct_2 = malloc(sizeof(node));
            new_struct_1->frere = new_struct_2;
        }
    }

    // The end of the struct is known when the son functions are done
    *current_char -= 1;
    struct_current->fin = *current_char;
}

void content_length_header(char **current_char, node *struct_current){}
void content_type_header(char **current_char, node *struct_current){}
void cookie_header(char **current_char, node *struct_current){}
void transfer_encoding_header(char **current_char, node *struct_current){}
void expect_header(char **current_char, node *struct_current){}
void connection_option(char **current_char, node *struct_current){
    // connection-option = token
    // Init the struct (ptr, int...), and allocate memory for the first child
    struct_current->debut = *current_char;
    struct_current->label = CONNECTION_OPTION;
    struct_current->fils = NULL;

    // Allocate memory for the first child
    node *new_struct_1 = malloc(sizeof(node));
    struct_current->fils = new_struct_1;

    // Call the function for the first child, supposed to be token
    token(current_char, new_struct_1);

    // The end of the struct is known when the son functions are done
    struct_current->fin = *current_char;
}

/** \fn void field_name(char **current_char, node *struct_current)
 * \brief Parse the field name
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void field_name(char **current_char, node *struct_current){
    // Init the struct (ptr, int...), and allocate memory for the first child
    struct_current->debut = *current_char;
    struct_current->label = FIELD_NAME;
    struct_current->fils = NULL;

    // Allocate memory for the first child
    node *new_struct_1 = malloc(sizeof(node));
    struct_current->fils = new_struct_1;

    // Call the function for the first child, supposed to be token
    token(current_char, new_struct_1);

    // The end of the struct is known when the son functions are done
    struct_current->fin = *current_char;
}

/** \fn void ows(char **current_char, node *struct_current)
 * \brief Parse the ows
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void ows(char **current_char, node *struct_current){
    if(**current_char == 0x20 || **current_char == 0x09){
        // Init the struct (ptr, int...), and allocate memory for the first child
        struct_current->debut = *current_char;
        struct_current->label = OWS;
        struct_current->fils = NULL;

        // Store as many SP and HTAB as needed
        while(**current_char == 0x20 || **current_char == 0x09) {
            // Allocate memory for the first child
            node *new_struct_1 = malloc(sizeof(node));
            struct_current->fils = new_struct_1;

            if(**current_char == 0x20){
                // Call the function for the first child, supposed to be SP
                sp(current_char, new_struct_1);
            }
            else if(**current_char == 0x09){
                // Call the function for the first child, supposed to be HTAB
                htab(current_char, new_struct_1);
            }
            *current_char += 1;
            // Allocate memory for the second child (if needed)
            if(*(*current_char) == 0x20 || *(*current_char) == 0x09){
                // Allocate memory for the second child
                node *new_struct_2 = new_struct_1;
                new_struct_1 = malloc(sizeof(node));
                new_struct_2->frere = new_struct_1;
            }
        }

        // Go one char back to respect convention (never go further than the end of the struct in the said function) 
        *current_char-=1;
        // The end of the struct is known when the son functions are done
        struct_current->fin = *current_char;
    }
}

/** \fn void field_value(char **current_char, node *struct_current)
 * \brief Parse the field value
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void field_value(char **current_char, node *struct_current){
    // Init the struct (ptr, int...), and allocate memory for the first child
    struct_current->debut = *current_char;
    struct_current->label = FIELD_VALUE;
    struct_current->fils = NULL;
    node *new_struct_1, *new_struct_2;
    if(isobs_fold(*current_char) || isvchar(**current_char)){
        // Allocate memory for the first child
        new_struct_1 = malloc(sizeof(node));
        struct_current->fils = new_struct_1;
    }

    // Store as many field-content as needed. 
    while(isobs_fold(*current_char) || isvchar(**current_char)) {

        // Call the function for the first child, supposed to be field-content or obs-fold
        if(isobs_fold(*current_char)){
            // osb_fold : CRLF 1*( SP / HTAB )
            obs_fold(current_char, new_struct_1);
        } else if(isvchar(**current_char)){
            // field-content = field-vchar [ 1*( SP / HTAB ) field-vchar ]
            field_content(current_char, new_struct_1);
        }

        *current_char += 1;
        // Allocate memory for the second child (if needed)
        if(isobs_fold(*current_char) || isvchar(**current_char)){
            // Allocate memory for the second child
            new_struct_2 = new_struct_1;
            new_struct_1 = malloc(sizeof(node));
            new_struct_2->frere = new_struct_1;
        }
    }

    // Go one char back to respect convention (never go further than the end of the struct in the said function) 
    *current_char-=1;
    // The end of the struct is known when the son functions are done
    struct_current->fin = *current_char;
}

/** \fn void field_content(char **current_char, node *struct_current)
 * \brief Parse the field content
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
*/
void field_content(char **current_char, node *struct_current){
    // Init the struct (ptr, int...), and allocate memory for the first child
    struct_current->debut = *current_char;
    struct_current->label = FIELD_CONTENT;
    struct_current->fils = NULL;

    // Allocate memory for the first child
    node *new_struct_1 = malloc(sizeof(node));
    struct_current->fils = new_struct_1;

    // Call the function for the first child, supposed to be field-vchar
    field_vchar(current_char, new_struct_1);

    // Allocate memory for the second child (if it's a SP or HTAB)
    if(*(*current_char+1) == 0x20 || *(*current_char+1) == 0x09){
        // Go one char forward
        *current_char+=1;
        // Allocate memory for the second child
        node *new_struct_2 = new_struct_1;
        new_struct_1 = malloc(sizeof(node));
        new_struct_2->frere = new_struct_1;

        // Store as many SP and HTAB as needed
        while(**current_char == 0x20 || **current_char == 0x09) {
            if(**current_char == 0x20){
                // Call the function for the first child, supposed to be SP
                sp(current_char, new_struct_1);
            } else if(**current_char == 0x09){
                // Call the function for the first child, supposed to be HTAB
                htab(current_char, new_struct_1);
            }

            // Allocate memory for the second child (if needed)
            if(*(*current_char+1) == 0x20 || *(*current_char+1) == 0x09){
                // Allocate memory for the second child
                new_struct_2 = new_struct_1;
                new_struct_1 = malloc(sizeof(node));
                new_struct_2->frere = new_struct_1;
            }
            *current_char+=1;
        }

        // If we had a SP or a HTAB, we expect a field-vchar after
        new_struct_2 = new_struct_1;
        new_struct_1 = malloc(sizeof(node));
        new_struct_2->frere = new_struct_1;
        field_vchar(current_char, new_struct_1);
    }

    // The end of the struct is known when the son functions are done
    struct_current->fin = *current_char;
}

/** \fn void start_line(char **current_char, node *struct_current)
 * \brief Parse the start line of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
*/
void start_line(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = START_LINE;
    struct_current->fils = NULL;

    // Allocate memory for the first child
    node *new_struct_1 = malloc(sizeof(node));
    struct_current->fils = new_struct_1;

    // Call the function for the first child, supposed to be request_line
    request_line(current_char, new_struct_1);

    // The end of the struct is known when the son function is done
    struct_current->fin = *current_char;
}

/** \fn void request_line(char **current_char, node *struct_current)
 * \brief Parse the request line of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
*/
void request_line(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = REQUEST_LINE;
    struct_current->fils = NULL;

    // Allocate memory for the first child
    node *new_struct_1 = malloc(sizeof(node));
    struct_current->fils = new_struct_1;
    // Call the function for the first child, supposed to be method
    method(current_char, new_struct_1);
    *current_char += 1;

    // Allocate memory for sp
    node *new_struct_2 = malloc(sizeof(node));
    new_struct_1->frere = new_struct_2;
    sp(current_char, new_struct_2);
    new_struct_1 = new_struct_2;
    *current_char+=1;

    // Allocate memory for request-target
    new_struct_2 = malloc(sizeof(node));
    new_struct_1->frere = new_struct_2;
    request_target(current_char, new_struct_2);
    *current_char+=1;

    // Allocate memory for sp
    new_struct_1 = new_struct_2;
    new_struct_2 = malloc(sizeof(node));
    new_struct_1->frere = new_struct_2;
    sp(current_char, new_struct_2);
    *current_char+=1;

    // Allocate memory for http_version
    new_struct_1 = new_struct_2;
    new_struct_2 = malloc(sizeof(node));
    new_struct_1->frere = new_struct_2;
    http_version(current_char, new_struct_2);
    *current_char+=1;

    // Allocate memory for crlf
    new_struct_1 = new_struct_2;
    new_struct_2 = malloc(sizeof(node));
    new_struct_1->frere = new_struct_2;
    crlf(current_char, new_struct_2);

    // The end is known when the son functions are done
    struct_current->fin = *current_char;
}

/** \fn void crlf(char **current_char, node *struct_current)
 * \brief Parse the CRLF of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void crlf(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = CRLF;
    struct_current->fils = NULL;

    // Check if \n is present
    if (**current_char != '\r' || *(*current_char+1) != '\n'){
        printf("Error : carriage return + line feed expected, %c found\n", **current_char);
        exit(1);
    }

    // Go at the end of the struct (crlf)
    *current_char+=1;
    // The end is known at the end
    struct_current->fin = *current_char;
}

/** \fn void http_version(char **current_char, node *struct_current)
 * \brief Parse the http version of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void http_version(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = HTTP_VERSION;
    struct_current->fils = NULL;

    // Allocate memory for the first child
    node *new_struct_1 = malloc(sizeof(node));
    struct_current->fils = new_struct_1;
    // Call the function for the first child, supposed to be http-name
    http_name(current_char, new_struct_1);

    // Check if / is present
    if (**current_char != '/'){
        printf("Error : slash expected, %c found\n", **current_char);
        exit(1);
    }
    *current_char+=1;

    // Allocate memory for digit
    node *new_struct_2 = malloc(sizeof(node));
    new_struct_1->frere = new_struct_2;
    digit(current_char, new_struct_2);
    *current_char+=1;

    // Check if . is present
    if(**current_char != '.'){
        printf("Error : dot expected, %c found", **current_char);
        exit(1);
    }
    *current_char+=1;

    // Allocate memory for digit
    new_struct_1 = new_struct_2;
    new_struct_2 = malloc(sizeof(node));
    new_struct_1->frere = new_struct_2;
    digit(current_char, new_struct_2);

    // The end is known when the son functions are done
    struct_current->fin = *current_char;
}

/** \fn void http_name(char **current_char, node *struct_current)
 * \brief Parse the name of the http version
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void http_name(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = HTTP_NAME;
    struct_current->fils = NULL;

    if(!(**current_char == 'H' && *(*current_char+1) == 'T' && *(*current_char+2) == 'T' && *(*current_char+3) == 'P')){
        printf("Error : HTTP expected, %c%c%c%c found\n", **current_char, *(*current_char+1), *(*current_char+2), *(*current_char+3));
        exit(1);
    }
    *current_char+=4;

    // The end is known when the son functions are done
    struct_current->fin = *current_char;
}

/** \fn void method(char **current_char, node *struct_current)
 * \brief Parse the method of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct 
 * 
*/
void method(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = METHOD;
    struct_current->fils = NULL;

    // Allocate memory for the first child
    node *new_struct_1 = malloc(sizeof(node));
    struct_current->fils = new_struct_1;
    // Call the function for the first child, supposed to be token
    token(current_char, new_struct_1);

    // The end is known when the son function is done
    struct_current->fin = *current_char;
}

/** \fn void token(char **current_char, node *struct_current)
 * \brief Parse the method of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
*/
void token(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = TOKEN;
    struct_current->fils = NULL;

    // Allocate memory for the child and its brothers)
    node *new_struct_1 = malloc(sizeof(node));
    node *new_struct_2;
    tchar(current_char, new_struct_1);
    *current_char+=1;
    struct_current->fils = new_struct_1;
    do{
        new_struct_2 = malloc(sizeof(node));
        tchar(current_char, new_struct_2);
        new_struct_1->frere = new_struct_2;
        // move one struct forward 
        new_struct_1 = new_struct_2;
        *current_char+=1;
    }while(istchar(*(*current_char)));

    *current_char-=1;
    struct_current->fin = *current_char;
}

/** \fn void request_target(char **current_char, node *struct_current)
 * \brief Parse the request-target element of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
*/
void request_target(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = REQUEST_TARGET;
    struct_current->fils = NULL;

    // Allocate memory for the first child
    node *new_struct_1 = malloc(sizeof(node));
    struct_current->fils = new_struct_1;
    absolute_path(current_char, new_struct_1);

    if(*(*current_char+1)=='?'){
        // Store the question mark
        current_char+=1;
        node *new_struct_2 = malloc(sizeof(node));
        new_struct_1->frere = new_struct_2;
        icar(current_char, new_struct_2);
        new_struct_1 = new_struct_2;

        // Move pass the question mark to store the query
        *current_char+=1;
        // Allocate memory for the first child
        new_struct_2 = malloc(sizeof(node));
        new_struct_1->frere = new_struct_2;
        query(current_char, new_struct_2);
    }

    // The end is known when the son functions are done
    struct_current->fin = *current_char;
}

/** \fn void query(char **current_char, node *struct_current)
 * \brief Parse the query element of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void query(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = QUERY;
    struct_current->fils = NULL;

    // Allocate memory for the child and its brothers)
    node *new_struct_1 = malloc(sizeof(node));
    struct_current->fils = new_struct_1;
    while(ispchar(**current_char) || **current_char=='/' || **current_char=='?'){
        node *new_struct_2 = malloc(sizeof(node));
        if(ispchar(**current_char)) {
            pchar(current_char, new_struct_1);
        } else {
            icar(current_char, new_struct_1);
        }
        new_struct_1->frere = new_struct_2;
        // move one struct forward 
        new_struct_1 = new_struct_2;
        *current_char+=1;
    }

    // Go back one char to be on the last char of the query
    *current_char-=1;
    struct_current->fin = *current_char;
}

/** \fn void absolute_path(char **current_char, node *struct_current)
 * \brief Parse the absolute-path element of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void absolute_path(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = ABSOLUTE_PATH;
    struct_current->fils = NULL;

    if(**current_char=='/'){
        icar(current_char, struct_current);
        *current_char+=1;
        // Allocate memory for the first child
        node *new_struct = malloc(sizeof(node));
        struct_current->fils = new_struct;
        segment(current_char, new_struct);
    }
    // The end is known when the son functions are done
    struct_current->fin = *current_char;
}

/** \fn void segment(char **current_char, node *struct_current)
 * \brief Parse the segment element of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void segment(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = SEGMENT;
    struct_current->fils = NULL;

    if(ispchar(**current_char)){
        // Allocate memory for the first child
        node *new_struct_1 = malloc(sizeof(node));
        struct_current->fils = new_struct_1;
        do{
            node *new_struct_2 = malloc(sizeof(node));
            pchar(current_char, new_struct_2);
            // move one struct forward 
            new_struct_1->frere = new_struct_2;
            new_struct_1 = new_struct_2;
            *current_char+=1;
        }while(ispchar(*(*current_char+1)));
    }
    *current_char-=1;
    struct_current->fin = *current_char;
}

/** \fn void pchar(char **current_char, node *struct_current)
 * \brief Parse the pchar element of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void pchar(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = PCHAR;
    struct_current->fils = NULL;
    // Allocate memory for the first child
    node *new_struct = malloc(sizeof(node));
    struct_current->fils = new_struct;
    if(isunreserved(**current_char)){
        unreserved(current_char, new_struct);
    }else if(ispct_encoded(**current_char)){
        pct_encoded(current_char, new_struct);
    }else if(issub_delims(**current_char)){
        sub_delims(current_char, new_struct);
    }else if(**current_char==':' || **current_char=='@'){
        icar(current_char, new_struct);
    }else{
        printf("Error : pchar not recognized");
        exit(1);
    }
    struct_current->fin = *current_char;
}

/** \fn void unreserved(char **current_char, node *struct_current)
 * \brief Parse the unreserved element of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void unreserved(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = UNRESERVED;
    struct_current->fils = NULL;

    // Allocate memory for the first child
    node *new_struct = malloc(sizeof(node));
    struct_current->fils = new_struct;
    if(isalpha(**current_char)){
        alpha(current_char, new_struct);
    }else if(isdigit(**current_char)){
        digit(current_char, new_struct);
    }else if(**current_char=='-' || **current_char=='.' || **current_char=='_' || **current_char=='~'){
        icar(current_char, new_struct);
    }else{
        printf("Error : unreserved not recognized");
        exit(1);
    }
    *current_char+=1;
    struct_current->fin = *current_char;
}

/** \fn void pct_encoded(char **current_char, node *struct_current)
 * \brief Parse the pct-encoded element of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void pct_encoded(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = PCT_ENCODED;
    struct_current->fils = NULL;

    // Allocate memory for the child
    if(**current_char=='%'){
        icar(current_char, struct_current);
        *current_char+=1;
        // Allocate memory for the first child
        node *new_struct_1 = malloc(sizeof(node));
        struct_current->fils = new_struct_1;
        hexdig(current_char, new_struct_1);
        *current_char+=1;
        // Allocate memory for the first child
        node *new_struct_2 = malloc(sizeof(node));
        new_struct_1->frere = new_struct_2;
        hexdig(current_char, new_struct_2);
    }else{
        printf("Error : pct_encoded not recognized");
        exit(1);
    }
    struct_current->fin = *current_char;
}

/** \fn void hexdig(char **current_char, node *struct_current)
 * \brief Parse the hexdig element of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void hexdig(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = HEXDIG;
    struct_current->fils = NULL;
    if(!isxdigit(**current_char)){
        printf("Error : hexdig not recognized");
        exit(1);
    }
    struct_current->fin = *current_char;
}

/** \fn void sub_delims(char **current_char, node *struct_current)
 * \brief Parse the sub_delims element of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void sub_delims(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->label = SUB_DELIMS;
    struct_current->fils = NULL;
    if(!(issub_delims(**current_char))){
        printf("Error : sub_delims not recognized");
        exit(1);
    }
    struct_current->fin = *current_char;
}

/** \fn void tchar(char **current_char, node *struct_current)
 * \brief Parse the tchar of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void tchar(char **current_char, node *struct_current){
    // Init the struct (ptr, int...)
    struct_current->debut = *current_char;
    struct_current->fin = *current_char;
    struct_current->label = TCHAR;
    struct_current->fils = NULL;

    // Allocate memory for the first child
    node *new_struct = malloc(sizeof(node));
    struct_current->fils = new_struct;

    // if its a SP, end of method
    if(istchar(**current_char)) {
        // Allocate memory for the first child
        node *new_struct = malloc(sizeof(node));
        struct_current->fils = new_struct;

        // If the next char is a letter, call alpha()
        if(isalpha(**current_char)){
            alpha(current_char, new_struct);
        
        // If the next char is a digit, call digit()
        } else if(isdigit(**current_char)){
            digit(current_char, new_struct);

        // Else, it's an icar
        } else{
            icar(current_char, new_struct);
        }
    // If the next char isnt a tchar or a SP, error
    } else if(!istchar(**current_char)){
        printf("Error: the method must be composed of tchars, not %c\n", **current_char);
        exit(1);
    }
}

/** \fn void alpha(char **current_char, node *struct_current)
 * \brief Parse the alpha characters of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void alpha(char **current_char, node *struct_current){
    if (!isalpha(**current_char)){
        printf("Error: Expected a char, not %c", **current_char);
        exit(1);
    }
    struct_current->label = ALPHA;
    struct_current->fils = NULL;
    struct_current->debut = *current_char;
    struct_current->fin = *current_char;
}

/** \fn void digit(char **current_char, node *struct_current)
 * \brief Parse the digit characters of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void digit(char **current_char, node *struct_current){
    if (!isdigit(**current_char)){
        printf("Error: Expected a digit, not %c", **current_char);
        exit(1);
    }
    struct_current->label = DIGIT;
    struct_current->fils = NULL;
    struct_current->debut = *current_char;
    struct_current->fin = *current_char;
}

/** \fn void sp(char **current_char, node *struct_current)
 * \brief Parse the SP character of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void sp(char **current_char, node *struct_current){
    if (**current_char != 0x20){
        printf("Error: Expected a SP, not %c\n", **current_char);
        exit(1);
    }
    struct_current->label = SP;
    struct_current->fils = NULL;
    struct_current->debut = *current_char;
    struct_current->fin = *current_char;
}

/** \fn void htab(char **current_char, node *struct_current)
 * \brief Parse the HTAB character of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void htab(char **current_char, node *struct_current){
    if (**current_char != 0x09){
        printf("Error: Expected a HTAB, not %c\n", **current_char);
        exit(1);
    }
    struct_current->label = HTAB;
    struct_current->fils = NULL;
    struct_current->debut = *current_char;
    struct_current->fin = *current_char;
}

/** \fn void icar(char **current_char, node *struct_current)75
 * \brief Parse the icar character of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void icar(char **current_char, node *struct_current){
    struct_current->label = ICAR;
    struct_current->fils = NULL;
    struct_current->debut = *current_char;
    struct_current->fin = *current_char;
}

/** \fn void obs_fold(char **current_char, node *struct_current)
 * \brief Parse the obs_fold (obs-fold = CRLF 1*( SP / HTAB )) characters of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void obs_fold(char **current_char, node *struct_current){
    struct_current->label=OBS_FOLD;
    struct_current->fils = NULL;
    struct_current->debut = *current_char;

    // Allocate memory
    node *new_struct = malloc(sizeof(node));
    node *new_struct2;
    struct_current->fils = new_struct;
    crlf(current_char,new_struct);
    // We can safely increment the current char because we always have at least one SP or HTAB after
    *current_char+=1;

    //Check if we have AT LEAST one SP or HTAB
    while(**current_char==0x20 || **current_char==0x09){
        new_struct2=malloc(sizeof(node));
        new_struct->frere=new_struct2;
        if(**current_char==0x20){
            sp(current_char,new_struct2);
        } else if(**current_char==0x09){
            htab(current_char,new_struct2);
        }

        // Go one char further
        *current_char+=1;
        new_struct=new_struct2;
    }

    // We decrement the current char to respect the convention
    *current_char-=1;
    struct_current->fin = *current_char;
}

/** \fn void field_vchar(char **current_char, node *struct_current)
 * \brief Parse the field_vchar (field-vchar = VCHAR / obs-text) characters of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void field_vchar(char **current_char, node *struct_current){
    struct_current->label=FIELD_VCHAR;
    struct_current->fils = NULL;
    struct_current->debut = *current_char;

    // Allocate memory
    node *new_struct = malloc(sizeof(node));
    struct_current->fils = new_struct;

    // If the next char is a VCHAR, call vchar()
    if(isvchar(**current_char)){
        vchar(current_char, new_struct);
    // If the next char is a obs-text, call obs_text()
    } else if(isobs_text(**current_char)){
        obs_text(current_char, new_struct);
    // Else, it's an error
    } else{
        printf("Error: Expected a VCHAR or an obs-text, not %c", **current_char);
        exit(1);
    }

    // The end of the field_vchar is the end of the obs_text or vchar
    struct_current->fin = *current_char;
}

/** \fn void vchar(char **current_char, node *struct_current)
 * \brief Parse the vchar (vchar = %x21-7E) characters of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void vchar(char **current_char, node *struct_current){
    
    if (!isvchar(**current_char)){
        printf("Error: Expected a VCHAR, not %c", **current_char);
        exit(1);
    }
    struct_current->label = VCHAR;
    struct_current->fils = NULL;
    struct_current->debut = *current_char;
    struct_current->fin = *current_char;
}

/** \fn void obs_text(char **current_char, node *struct_current)
 * \brief Parse the obs_text (obs-text = %x80-FF) characters of the request
 * \param current_char : pointer to the current char
 * \param struct_current : pointer to the current struct
 * 
*/
void obs_text(char **current_char, node *struct_current){
    if (!isobs_text(**current_char)){
        printf("Error: Expected an obs-text, not %c", **current_char);
        exit(1);
    }
    struct_current->label = OBS_TEXT;
    struct_current->fils = NULL;
    struct_current->debut = *current_char;
    struct_current->fin = *current_char;
}

void host_port(char** current_char, node* struct_current){}

int isipv4address(char* current_char){}

int isipv6address(char* current_char){}

int issubdelims(char c){}

int subdelims(char** current_char, node* struct_current){}

void reg_name(char** current_char, node* struct_current){}