#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/utility.h"
#include <ctype.h>


/*#ifndef ABNF_H
    #define ABNF_H
    #include "../headers/abnf.h"
#endif*/

void print_node(node *nodes){
    printf("label : %s\n",nodes->label);
    printf("s : %s\n",nodes->s);
    printf("taille : %d\n\n",nodes->taille);
    if(nodes->frere!=NULL){
        printf("Son frere : \n");
        print_node(nodes->frere);
    }
    if(nodes->fils!=NULL){
        printf("Son fils : \n");
        print_node(nodes->fils);
    }
    return;
}

/**
 * \fn int isponct(char c)
 * \brief Function to check if a char is a ponct
 * \param c char to test
 * \returns 1 if yes ; 0 else
*/
int isponct(char c){
    if (c==','||c=='.'||c=='!'||c=='?'||c==':') return 1;
    else return 0;
}
//ponct = "," / "." / "!" / "?" / ":"

int main(int argc, char const *argv[])
{
    /*Un fichier doit etre fourni en paramètre*/
    if(argc!=2){printf("Usage : ./simplegrammar.exe <file to parse>\n");return 1;}
    
    /*On creer un flux fic et on verifie qu'il soit bien ouvert*/
    FILE *fic = NULL;
    fic=fopen(argv[1],"r");
    if (fic==NULL){printf("Erreur ouverture fichier, le nom est-il correct ?\n");return 1;} /*Detection erreur ouverture fichier*/

    /*Calcul taille requete*/
    int c=fgetc(fic);
    int taille=0;
    while(c!=EOF){
        taille++;
        c=fgetc(fic);
    }
    printf("Taille : %d\n",taille);
    if(taille > 500){printf("Erreur taille fichier\n");fclose(fic);return 1;}/*Requete trop longue -> sortie du programme*/
    fseek(fic,0,0);

    /*Copie en memoire de la requete*/
    char requete[taille+1];
    requete[taille]=0;
    for (int i = 0; i < taille; i++)
    {
        c=fgetc(fic);
        requete[i]=c;
        //printf("requete[%d]=%c <-> %x\n",i,requete[i],requete[i]);
    }
    //printf("requete[%d]=%c <-> %x\n",taille,requete[taille],requete[taille]); /*Verification*/
    printf("Requete : %s\n_________________________________________________\n\n",requete);

    /*ANALYSE REQUETE*/
    /*Check si la requete se termine par un LF*/
    if(requete[taille-1]!='\n'){printf("Requete invalide ! (LF)\n");fclose(fic);return 1;} /*Pas de LF -> sortie du programme*/
    printf("___LF valide___\n");

    node *master_node=malloc(sizeof(node));
    char *adr=&requete[0];

    /*Check si la requete commence par start*/
    debut(&adr,master_node);

    /*Préparation boucle while*/
    node *next=master_node;
    node *next2;

    while(*(adr+3)!='\n'){ /*Condition d'arret, on sait que \n existe*/
        next2=malloc(sizeof(node));
        next->frere=next2;
        if(isalpha(*adr)) { /*Mot suivi d'un ponct*/
            mot(&adr,next2);
            next=next2;
            next2=malloc(sizeof(node));
            next->frere=next2;
            ponct(&adr,next2);
            }
        else if(isdigit(*adr)) { /*Nombre suivi d'un separateur*/
            nombre(&adr,next2);
            next=next2;
            next2=malloc(sizeof(node));
            next->frere=next2;
            separateur(&adr,next2);
            }
        else if(isponct(*adr)){ /*[ponct] optionnel de la doc*/
            ponct(&adr,next2);
        }
        else {printf("Problème de lecture : '%c'\n",*adr);fclose(fic);return 1;}
        next=next2;
    }
    next2=malloc(sizeof(node));
    next->frere=next2;

    /*Check si la requete se termine par fin*/
    fin(&adr,next2);

    /*Affiche l'arbre*/
    print_tree(master_node,1);
    /*Fin du programme*/
    printf("\033[1;32m");
    printf("Exited normally\n");
    printf("\033[0m");
    fclose(fic);

    /*Liste les fichiers dont l'analyse s'est bien passée*/
    FILE *val=fopen("validations.txt","a");
    fprintf(val,"%s\n",argv[1]);
    fclose(val);

    return 0;
}

/*DEBBUGING NOTES :
(gdb) p *adr
$1 = 86 'V'
(gdb) p adr
$2 = 0x7fffffffdca5 "VKiMupi-.4 LCTKq-.fin\n"
*/