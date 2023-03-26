#!/bin/bash
if (($# == 1))
then
cat ./rfc/allrfc.abnf | grep --color=auto $1

if ((`cat ./rfc/allrfc.abnf | grep $1 | wc -l` == 0))
then
echo "No match found"
fi

else
echo "Usage : ./askabnf <field to locate>"
echo "Tips : adding ^ at the beginning of the field will search the lines starting with that fied. Example : ./askabnf.sh ^HTTP"
fi
