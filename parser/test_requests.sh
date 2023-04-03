#!/bin/bash

RED="\e[31m"
GREEN="\e[32m"
ENDCOLOR="\e[0m"

echo "Entrez le chemin du dossier contenant les requêtes"
echo "Exemple : \"./alltests/simpletests/*\""
read FILES
for file in $FILES
do
RES=$(./httpparser $file | grep -a "TOKEN_EXITED" | wc -l)
SOL=$(./giorgiparser $file | wc -l)
if [ "$RES" -eq 1 -a "$SOL" -ne 0 ] #Requete OK par nous et httpparser
then
echo -e "$file : ${GREEN}OK${ENDCOLOR}"
elif [ "$RES" -eq 0 -a "$SOL" -eq 0 ] #Requete rejete par nous et httpparser
then
echo -e "$file : ${GREEN}OK${ENDCOLOR}"
else #Conflit
echo -e "${RED}$file : not valid${ENDCOLOR}"
fi

done
