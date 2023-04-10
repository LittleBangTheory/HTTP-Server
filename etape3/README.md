# TODO
* Inclure l'API dans le projet
* Analyse sémantique d'une requête (lister les règles)
* Génération réponse + gestion connexion (doit savoir se servir de l'API)
* Création d'un site web de test

# Règles

## Request line

Method rules :
* [RFC 7231 4.3.1] GET - A payload within a GET request message has no defined semantics; sending a payload body on a GET request might cause some existing implementations to reject the request.
* [RFC 7231 4.3.2] HEAD - A payload within a HEAD request message has no defined semantics sending a payload body on a HEAD request might cause some existing implementations to reject the request.
* [RFC 7231 4.3.3] POST - The POST method requests that the target resource process the representation enclosed in the request according to the resource's own specific semantics.

Request-target :
* [RFC 7230 2.7.3] Normalisation de la request-taget conformement à la rfc3986, notamment le « percent encoding » et le « dot segment removal »

HTTP-version :
* [RFC 7230 2.7.3] 

## Headers :
* Transfer-Encoding-header [rfc 7230 – 4]
* Cookie-header / [rfc6265]
* Referer-header [rfc7231 – 5.5.2]
* User-Agent-header [rfc7231 -5.5.3]
* Accept-header [rfc7231 – 5.3.2]
* Accept-Encoding-header [rfc 7231 – 5.3.4]
* Content-Length-header [rfc7230 – 3.3.2 et 3.3.3]
* Host-header [rfc7230 – 5.3 et 5.4]
* Connection-header [rfc7230 – 6]
