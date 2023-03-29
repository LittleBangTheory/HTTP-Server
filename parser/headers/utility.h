#ifndef ABNF_H
    #define ABNF_H
    #include "../headers/abnf.h"
#endif
void print_request(node*, char*, int);
void print_tree(node*, int);
//void delete_chained_list(node*);
int istchar(char c);
int ispchar(char c);
int isunreserved(char c);
int ispct_encoded(char c);
int issub_delims(char c);
int isvchar(char c);
int isobs_text(char c);
int isobs_fold(char *current_char);
int isconnection_end(char *current_char);
int isheader_end(char *current_char);
int isipv4address(char *current_char);
int isip_literal(char *current_char);
int isipv6address(char *current_char);
int isipvfuture(char *current_char);
int ish16(char *current_char);
int isls32(char *current_char);
int ismedia_type_end(char *current_char);
int isqdtext(char c);
int iscookie_octet(char c);
int isspecialheader(char *current_char);