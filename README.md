# Projet-HTTP

# Ressources

Summary RFC 9110 - [HTTP Semantics](https://www.bortzmeyer.org/9110.html)

Summary RFC 9112 - [HTTP/1.1](https://www.bortzmeyer.org/9112.html)

# Releases

## Release 1 : Compréhension documentation RFC. Notamment :
  * RFC7230:
    * [ ] Chapitre 1.
    * [ ] Chapitre 2 (sauf 2.3 et 2.4)
    * [ ] Chapitre 3.
    * [ ] Chapitre 5.
    * [ ] Chapitre 6.
    * [ ] Chapitre 9.
  * RFC7231 :
   * [ ] Chapitre 4 (surtout 4.3.1, 4.3.2, 4.3.3)
   * [ ] Chapitre 5 (surtout 5.3)
   * [ ] Chapitre 6 (surtout 6.1, 6.3, 6.5, 6.6)
   * [ ] Chapitre 9

## Release 2 : Prise en compte des requêtes pour servir des fichiers locaux

### Sprints 1, 2, 3 :
 * [ ] Réception des requêtes, vérification syntaxique et sémantique
 * [ ] Gestion des entêtes HTTP
 * [ ] Normalisation de l'URL
 * [ ] Gestion de l'accès aux fichiers et type mime (basé sur l'extension) et le charset par défaut, gestion multi-sites
 * [ ] Optionnel : Gestion des encodages : Chunked / Deflate / gzip


### Sprint 4 :
  * [ ] Gestion de l'interface fastCGI
  * [ ] Gestion des réceptions fastCGI/émission HTTP
  * [ ] Démonstration avec un serveur d'application PHP
