#!/bin/bash

RED="\e[31m"
GREEN="\e[32m"
ENDCOLOR="\e[0m"

FILES="./alltests/simplesfiles/*"
for file in $FILES
do
RES=$(./ourParser $file | grep -a "TOKEN_EXITED" | wc -l)
SOL=$(./httpparser_ $file | wc -l)
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