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
int is_pct_encoded(char c);
int sub_delims(char c);