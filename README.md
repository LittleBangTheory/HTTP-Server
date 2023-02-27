# Projet-HTTP

# Releases

## Documentation RFC :
  * [RFC 9110](https://www.rfc-editor.org/rfc/rfc9110) (HTTP Semantics) - [Summary](https://www.bortzmeyer.org/9110.html) :
    * [ ] Chapitre 1 - Introduction.
    * [ ] Chapitre 3 - Terminology and Core Concepts.
    * [ ] Chapitre 6 - Message Abstraction.
    * [ ] Chapitre 7 - Routing HTTP Messages.
    * [ ] Chapitre 9 - Methods.
    * [ ] Chapitre 15 - Status Codes
    * [ ] Chapitre 17 - Security Considerations.
  * [RFC 9112](https://www.rfc-editor.org/rfc/rfc9112) (HTTP/1.1) - [Summary](https://www.bortzmeyer.org/9112.html) :
    * [ ] Chapitre 2 - Message
    * [ ] Chapitre 3 - Request Line
    * [ ] Chapitre 5 - Field Syntax
    * [ ] Chapitre 3 - Request Line
    * [ ] Chapitre 6 - Message Body
    * [ ] Chapitre 11 - Security Considerations

## Release 2 : Réalisation du parseur HTTP selon la grammaire Chamilo

### Sprint 1 - 07/03/2023 : Réaliser un parseur pour la *grammairesimple* indiquée sur chamilo.
* Analyser les fichiers de tests fournis.
* Objectif -> Valider sur un PoC la capacité à réaliser un parseur plus évolué.
* Matériel : un répertoire *simplegrammar* 
	* Grammaire abnf simple 
	* Fichiers de test (syntaxe valide) 
	* Un parseur de cette grammaire qui peut analyser les fichiers de tests et servir de référence 


### Sprint 2 - 31/03/2023 : Réaliser le parseur des éléments de la grammaire HTTP
* Objectifs
  * [ ] Réception des requêtes, vérification syntaxique et sémantique
  * [ ] Gestion des entêtes HTTP
  * [ ] Normalisation de l'URL
  * [ ] Gestion de l'accès aux fichiers et type mime (basé sur l'extension) et le charset par défaut, gestion multi-sites
  * [ ] Optionnel : Gestion des encodages : Chunked / Deflate / gzip
* Matériel : 
  * Répertoire *arbres* qui formalise de manière visuelle l'arbre issu du parseur. 
  * Parseur http qui peut servir de référence 
  * Jeu de test simple *premier-jeu-test.tar.gz* qui contient des messages HTTP (certains sont valides, et d'autres non, je vous laisse en exercice le soin de trouver pourquoi)
  Fichier *api.h*  et *main.c* pour l'interface et son usage. 
  * Jeu de test hardcore pour vérifier votre parseur 

## Release 3 : Interfaces
### Sprint 4 :
  * [ ] Gestion de l'interface fastCGI
  * [ ] Gestion des réceptions fastCGI/émission HTTP
  * [ ] Démonstration avec un serveur d'application PHP
