#ifndef ABNF_H
    #define ABNF_H
    #include "../headers/abnf.h"
#endif
void print_request(node*, char*, int);
void print_tree(node*, int);
void delete_chained_list(node*);
int istchar(char c);
int ispchar(char c);
int isunreserved(char c);
int ispct_encoded(char c);
int issub_delims(char c);
int isvchar(char c);
int isobs_text(char c);
int isobs_fold(char *current_char);