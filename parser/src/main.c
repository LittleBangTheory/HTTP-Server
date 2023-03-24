#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

#include "../headers/api.h"
#include "../headers/abnf.h"
#include "../headers/utility.h"

#define true 1
#define false 0

void testMode(){
	char req[]="GET / HTTP/1.1";
	char *adr=&req[0];
	char* label[]={"start-line","method","tchar","SP","ALPHA","DIGIT"};
	node* tete=malloc(sizeof(node));
	start_line(&adr,tete,label);
	print_tree(tete,1);
	exit(true);
}


int main(int argc, char const *argv[])
{
	if (argc==3) testMode();
	if (argc!=2){printf("Usage : ./parsername <file>\n");return false;}
	char* label[]={"start-line","method","tchar","SP","ALPHA","DIGIT"};
	FILE* fic = NULL;
	fic=fopen(argv[1],"r");
	if(fic==NULL){printf("Erreur ouverture\n");return false;}

	//recopie memoire
	int requestSize=1;
	int i;
	int c=fgetc(fic);
	while(c!=EOF){requestSize++;c=fgetc(fic);}
	char Request[requestSize];
	fseek(fic,0,0);
	c=fgetc(fic);
	for (i = 0; i < requestSize-1; i++)
	{
		Request[i]=c;
		c=fgetc(fic);
	}
	Request[i]=0;
	printf("%s",Request);


	


}
