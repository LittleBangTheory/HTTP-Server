#!/bin/bash

if [ "$#" -eq 1 ]
then

`./giorgiparser_ $1 > 1.tmp`
`./httparser $1 > 2.tmp`
diff -a -w 1.tmp 2.tmp --color
`rm *.tmp`

else
echo "Usage : ./compare.sh <file>"
fi