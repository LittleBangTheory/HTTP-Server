typedef struct abnf node;
struct abnf {
   char *label; //a def dans le main
   char *s;
   unsigned int taille;
   node *fils;
   node *frere;
};