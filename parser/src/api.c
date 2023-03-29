#include "../headers/api.h"
#include "../headers/abnf.h"
#include <string.h>
#include <stdlib.h>
// Fonction qui retourne un pointeur (type opaque) vers la racine de l'arbre construit. 
void *getRootTree(){
    return &racine;
}

/*
Fonction interne d'aide a searchTree
*/
void locateFields(_Token* current,node* noeud,char* field){
    if(noeud==NULL)return;
    _Token* new=current;
    if (!strcmp(field,noeud->label)){
        new = malloc(sizeof(_Token));
        new->next=NULL;
        new->node=noeud;
        current->next=new;
    }
    locateFields(new,noeud->fils,field);
    locateFields(new,noeud->frere,field);

}

// Fonction qui recherche dans l'arbre tous les noeuds dont l'etiquette est egale à la chaine de caractères en argument.   
// Par convention si start == NULL alors on commence à la racine 
// sinon on effectue une recherche dans le sous-arbre à partir du noeud start 
_Token *searchTree(void *start,char *name){
    if (start==NULL){
      start=getRootTree();
    }
    _Token* head = NULL;
    locateFields(head,(node*)start,name);
    return head;
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
char *getElementValue(void *noeud,int *len){
    node* current = noeud;
    int taille=current->fin - current->debut;
    char* res = malloc((taille+2)*sizeof(char));
    for (int i = 0; i <= taille; i++)
    {
        res[i]=current->debut[i];
    }
    res[taille+1]=0;
    return res;
    
} 

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
    if (element->fils != NULL) {
        purgeTree(element->fils);
    }
    if (element->frere != NULL){
        purgeTree(element->frere);
    }
    free(element);
} 

// L'appel à votre parser un char* et une longueur à parser.  
int parseur(char *req, int len){
    return 0;
}