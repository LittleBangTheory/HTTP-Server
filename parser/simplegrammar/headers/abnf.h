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

void debut(int**, node*);
void fin(int**, node*);
void mot(int**, node*);
void nombre(int**, node*);
void separateur(int**, node*);
void ponct(int**, node*);