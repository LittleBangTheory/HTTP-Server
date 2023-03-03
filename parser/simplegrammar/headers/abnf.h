typedef struct abnf node;
struct abnf {
   char *label;
   char *s;
   unsigned int taille;
   node *pere;
   node *fils;
   node *frere_g;
   node *frere_d;
};

void debut(char**, node*);
void fin(char**, node*);
void mot(char**, node*);
void nombre(char**, node*);
void separateur(char**, node*);
void ponct(char**, node*);