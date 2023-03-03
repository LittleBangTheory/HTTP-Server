#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/abnf.h"
#include "../headers/utility.h"

int main(int argc, char const *argv[])
{
    /*Un fichier doit etre fourni en paramètre*/
    if(argc!=2){printf("Usage : ./simplegrammar.exe <file to parse>\n");return 1;}
    
    /*On creer un flux fic et on verifie qu'il soit bien ouvert*/
    FILE *fic = NULL;
    fic=fopen(argv[1],"r");
    if (fic==NULL){printf("Erreur ouverture fichier, le nom est-il correct ?\n");return 1;}

    /*Calcul taille requete*/
    int c=fgetc(fic);
    int taille=0;
    while(c!=EOF){
        taille++;
        c=fgetc(fic);
    }
    printf("Taille : %d\n",taille);
    if(taille > 500){printf("Erreur taille fichier\n");return 1;}
    fseek(fic,0,0);

    /*Copie en memoire de la requete*/
    char requete[taille+1];
    requete[taille]=0;
    for (int i = 0; i < taille; i++)
    {
        c=fgetc(fic);
        requete[i]=c;
        printf("requete[%d]=%c <-> %x\n",i,requete[i],requete[i]);
    }
    printf("requete[%d]=%c <-> %x\n",taille,requete[taille],requete[taille]);
    printf("Requete : %s\n",requete);
    
    /*Fin du programme*/
    fclose(fic);
    return 0;
}
