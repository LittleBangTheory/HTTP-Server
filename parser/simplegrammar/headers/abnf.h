typedef struct abnf{
   char *s;
   int taille;
   node *pere;
   node *fils;
   node *frere_g;
   node *frere_d;
}node;
