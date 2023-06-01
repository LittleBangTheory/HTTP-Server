# Projet-HTTP

## Authors

* Gabin CHOGNOT
* Julien DA COSTA
* Amelie ARNAUD

# About the project

This is a fully functionnal HTTP server written in C. It is able to parse the requests, and answer accordingly, handling any media, plus PHP scripts, for HTTP/1.0 and HTTP/1.1, and for the GET, HEAD, and POST methods.

More informations and documentation can be found in the `documents` folder.

# Parser

## PoC

`simplegrammarparser/` contains a PoC of a parser for a simple grammar, written in C, that was used as a base for building a more complex parser. It contains it's own simple grammar and test files.

## HTTP Parser

The parser in `parser/` can parse any HTTP request following the grammar in `parser/rfc/allrfc.abnf`, and display it as a tree, or search for a specific fields and for its number of occurences.

# HTTP Server

For more detailled instructions and informations, see `server/`

The server handles : 
  * [x] Reception of the requests
  * [x] Syntax and semantic verification.
  * [x] HTTP headers
  * [x] URL normalization
  * [x] Gestion de l'accès aux fichiers et type mime (basé sur l'extension) et le charset par défaut, gestion multi-sites
  * [x] Access to files using their mime type (or their extension) and the default charset
  * [x] Multi-sites management
  * [ ] Optional : Encoding management : Chunked / Deflate / gzip
  * [x] fastCGI interface management to receive and send requests to the PHP motor

