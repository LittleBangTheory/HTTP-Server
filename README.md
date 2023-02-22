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
