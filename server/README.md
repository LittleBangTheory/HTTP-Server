# TODO
* Inclure l'API dans le projet
* Analyse sémantique d'une requête -> Détailler les règles pour les headers
* Génération réponse + gestion connexion (doit savoir se servir de l'API)
* Création d'un site web de test

# Libparser

`libparser.so` : parser précompilé
* Peut être compilé avec :
  * ``gcc -Wall -g -o sock main.c -L. -lparser``
  * ``export LD_LIBRARY_PATH=.``
* Ou est utilisé par le Makefile pour compiler `httpparser`

`httpparser` : parser compilé, dont on peut modifier le ``main.c`` et l'API, qui utilise `libparser.so`