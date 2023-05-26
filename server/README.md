# README HTTP Server

## Authors
* Gabin Chognot
* Julien Da Costa
* Amelie Arnaud

## Table of contents

- [README HTTP Server](#readme-http-server)
  - [Authors](#authors)
  - [Table of contents](#table-of-contents)
- [How to use](#how-to-use)
  - [Libparser](#libparser)
  - [Configuration](#configuration)
  - [Compilation](#compilation)
  - [Execution](#execution)
- [Conception](#conception)
  - [Architecture](#architecture)
  - [Limitations](#limitations)
- [Tests](#tests)

# How to use
## Libparser

`libparser.so` : parser précompilé
* Peut être compilé avec :
  * ``gcc -Wall -g -o sock main.c -L. -lparser``
  * ``export LD_LIBRARY_PATH=.``
* Ou est utilisé par le Makefile pour compiler `httpparser`

`httpparser` : parser compilé, dont on peut modifier le ``main.c`` et l'API, qui utilise `libparser.so`

## Configuration

If you are a *h@ck3r*, use Burpsuite to intercept the request and modify the Host header to match "hidden-site" ou "master-site". "Localhost" is not supported in HTTP/1.1.

A more suitable solution would be to modifie `/etc/hosts` to add the following lines :
```
<ip>   hidden-site
<ip>   master-site
```
We recommand to use a local IP address, such as 127.0.0.2. The server is, however, listen to the 7777 port regardless of the IP address.

To do this, you can execute the following command in `server/src` (being in the sudousers is required) :
```
make hosts
```

## Compilation

To compile the server, you can execute the following command(s) in `server/src` :
```Makefile
make clean #If needed
make
```

## Execution

To execute the server, you can execute the following command(s) in `server/src` :
```
./server.exe
```

# Conception

## Architecture

* The server hosts two sites : master-site and hidden-site.
  * If no host specified in HTTP/1.0, the default host (master-site) is used.
* If the "/" target is asked, the target is changed to "/index.html". 
* The server supports GET, HEAD, and POST requests
* The server supports percent encoding.
* The server supports dot segment removal.
* The sever supports any media type, plus TXT, HTML, CSS, and JS files. 

## Limitations

* The server can't send a 501 (Unauthorized method) error, because there are no unauthorized methods, only unimplemented ones.
* The server can't handle chunked transfer encoding for now. It can't send a stream, and is limited to sending large files as a whole.
* The server doesn't use any PHP for now.

# Tests

You can find :
* Some CSS in every page.
* A JS script in [hidden-site:7777/index.html](hidden-site:7777/index.html). It changes randomly the background color of the page and the color of the text.
* Images and icons in [master-site:7777/index.html](master-site:7777/index.html) and [hidden-site:7777/index.html](hidden-site:7777/index.html)
* A pdf in [master-site:7777/about.html](master-site:7777/about.html)
* A video in [hidden-site:7777/aboutme.html](hidden-site:7777/aboutme.html). 
* A form in [master-site:7777/contact.html](master-site:7777/contact.html). It doesn't gives feedback, because there is no PHP server, but the results are printed in the terminal.
