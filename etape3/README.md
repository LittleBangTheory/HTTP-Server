# TODO
* Inclure l'API dans le projet
* Analyse sémantique d'une requête (lister les règles)
* Génération réponse + gestion connexion (doit savoir se servir de l'API)
* Création d'un site web de test

# Notes
## Réponse 
Request :
``` http
GET /toto.html HTTP/1.1
<headers>
```
`headers` must contain a `content-length` field or a `transfer-encoding` field with the value `chunked`.

Response :
``` http
HTTP/1.1 200 OK
Content-Type: text/html
Content-Length: <content_length>
<message_body>
```
Bonne pratique : envoyer les éléments de réponse les uns après les autres dès qu'on les a. 

Exemple : envoyer `HTTP/1.1 200 OK` avant de savoir qu'on doit envoyer `Content-Type: text/html`