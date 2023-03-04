#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/utility.h"

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
    //print_request(master_node,requete,taille);

    node *next=malloc(sizeof(node));
    master_node->frere=next;

    mot(&adr,next);

    node *next2=malloc(sizeof(node));
    next->frere=next2;

    ponct(&adr,next2);

    next=malloc(sizeof(node));
    next2->frere=next;
    nombre(&adr,next);

    next2=malloc(sizeof(node));
    next->frere=next2;
    separateur(&adr,next2);
    next=malloc(sizeof(node));
    next2->frere=next;
    mot(&adr,next);

    next2=malloc(sizeof(node));
    next->frere=next2;
    ponct(&adr,next2);

    next=malloc(sizeof(node));
    next2->frere=next;
    fin(&adr,next);
    //print_tree(master_node,1);

    //printf("PRINT NODE BELOW\n");
    //print_node(master_node);
    print_tree(master_node,1);
    /*Fin du programme*/
    printf("\033[1;32m");
    printf("Exited normally\n");
    fclose(fic);
    return 0;
}

/*DEBBUGING NOTES :
(gdb) p *adr
$1 = 86 'V'
(gdb) p adr
$2 = 0x7fffffffdca5 "VKiMupi-.4 LCTKq-.fin\n"
*/