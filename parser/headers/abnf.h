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