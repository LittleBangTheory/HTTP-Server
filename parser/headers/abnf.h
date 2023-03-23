#define ABNF_H
typedef struct abnf node;
struct abnf {
   char *label; //a def dans le main
   char *debut;
   char *fin;
   node *fils;
   node *frere;
};

#define START_LINE 0
#define METHOD 1
#define TCHAR 2
#define SP 3
#define ALPHA 4
#define DIGIT 5
#define SP 6
#define REQUEST_TARGET 7

void start_line(char **current_char, node *struct_current, char *label);
void token(char **current_char, node *struct_current, char *label);
void request_target(char **current_char, node *struct_current, char *label);
void tchar(char **current_char, node *struct_current, char *label);
void alpha(char **current_char, node *struct_current, char *label);
void digit(char **current_char, node *struct_current, char *label);
void sp(char **current_char, node *struct_current, char *label);
int istchar(char c);