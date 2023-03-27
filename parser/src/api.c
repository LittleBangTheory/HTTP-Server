#include "../headers/api.h"
#include "../headers/abnf.h"
#include <string.h>
#include <stdlib.h>
// Fonction qui retourne un pointeur (type opaque) vers la racine de l'arbre construit. 
void *getRootTree(){
    return &racine;
}

// Fonction qui recherche dans l'arbre tous les noeuds dont l'etiquette est egale à la chaine de caractères en argument.   
// Par convention si start == NULL alors on commence à la racine 
// sinon on effectue une recherche dans le sous-arbre à partir du noeud start 
_Token *searchTree(void *start,char *name){
    if (start==NULL){
      start=getRootTree();
    }
    node* noeud = start;
    _Token* tete=malloc(sizeof(_Token));

    while (noeud->frere!=NULL && noeud->fils!=NULL)
    if(!strcmp(name,noeud->label)){
        tete->node=noeud;
        tete->next=malloc(sizeof(_Token));
        tete=tete->next;
    }

    free(tete);
}

// fonction qui renvoie un pointeur vers char indiquant l'etiquette du noeud. (le nom de la rulename, intermediaire ou terminal) 
// et indique (si len!=NULL) dans *len la longueur de cette chaine.
char *getElementTag(void *noeud,int *len){
    node* element=noeud;
    if (len!=NULL){
        *len=strlen(element->label);
    }
    return element->label;
} 

// fonction qui renvoie un pointeur vers char indiquant la valeur du noeud. (la partie correspondnant à la rulename dans la requete HTTP ) 
// et indique (si len!=NULL) dans *len la longueur de cette chaine.
char *getElementValue(void *node,int *len){} 

// Fonction qui supprime et libere la liste chainée de reponse. 
void purgeElement(_Token **r){
    if ((*r)->next!=NULL){
        purgeElement(&((*r)->next));
    }
    free(*r);
} 

// Fonction qui supprime et libere toute la mémoire associée à l'arbre . 
void purgeTree(void *root){
    node *element=root;
    if (element->frere != NULL){
        purgeTree(element->frere);
    }
    if (element->fils != NULL) {
        purgeTree(element->fils);
    }
    free(element);
} 

// L'appel à votre parser un char* et une longueur à parser.  
int parseur(char *req, int len){} 
