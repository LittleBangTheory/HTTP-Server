typedef struct abnf node;
struct abnf {
   char label[11];
   char *s;
   unsigned int taille;
   node *fils;
   node *frere;
};

void debut(char**, node*);
void fin(char**, node*);
void mot(char**, node*);
void nombre(char**, node*);
void separateur(char**, node*);
void ponct(char**, node*);