#define ABNF_H
typedef struct abnf node;
struct abnf {
   char *label; //a def dans le main
   char *debut;
   char *fin;
   node *fils;
   node *frere;
};

extern char START_LINE[];
extern char METHOD[];
extern char TCHAR[];
extern char SP[];
extern char ALPHA[];
extern char DIGIT[];
extern char REQUEST_TARGET[];
extern char ABSOLUTE_PATH[];
extern char SEGMENT[];
extern char PCHAR[];
extern char UNRESERVED[];
extern char PCT_ENCODED[];
extern char SUB_DELIMS[];
extern char HEXDIG[];
extern char QUERY[];

void start_line(char **current_char, node *struct_current);
void token(char **current_char, node *struct_current);
void request_target(char **current_char, node *struct_current);
void absolute_path(char **current_char, node *struct_current);
void segment(char **current_char, node *struct_current);
void pchar(char **current_char, node *struct_current);
void tchar(char **current_char, node *struct_current);
void alpha(char **current_char, node *struct_current);
void digit(char **current_char, node *struct_current);
void sp(char **current_char, node *struct_current);void query(char **current_char, node *struct_current);
void sub_delims(char **current_char, node *struct_current);
void unreserved(char **current_char, node *struct_current);
void pct_encoded(char **current_char, node *struct_current);
void hexdig(char **current_char, node *struct_current);