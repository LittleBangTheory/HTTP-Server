#!/bin/bash

if [ "$#" -eq 1 ]
then

`./httpparser_ $1 > 1.tmp`
`./ourParser $1 > 2.tmp`
diff -w 1.tmp 2.tmp --color
`rm *.tmp`

else
echo "Usage : ./compare.sh <file>"
fi