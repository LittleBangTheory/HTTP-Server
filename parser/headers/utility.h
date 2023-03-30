#ifndef ABNF_H
    #define ABNF_H
    #include "../headers/abnf.h"
#endif
void print_request(node*, char*, int);
void print_tree(node*, int);
//void delete_chained_list(node*);
int istchar(unsigned char c);
int ispchar(unsigned char c);
int isunreserved(unsigned char c);
int ispct_encoded(unsigned char c);
int issub_delims(unsigned char c);
int isvchar(unsigned char c);
int isobs_text(unsigned char c);
int isobs_fold(unsigned char *current_char);
int isconnection_end(unsigned char *current_char);
int isheader_end(unsigned char *current_char);
int isipv4address(unsigned char *current_char);
int isip_literal(unsigned char *current_char);
int isipv6address(unsigned char *current_char);
int isipvfuture(unsigned char *current_char);
int ish16(unsigned char *current_char);
int isls32(unsigned char *current_char);
int ismedia_type_end(unsigned char *current_char);
int isqdtext(unsigned char c);
int iscookie_octet(unsigned char c);
int stringcompare(unsigned char *current_char, char *s);
int istransfer_extension_end(unsigned char *current_char);
int istransfer_encoding_end(unsigned char *current_char);