# README HTTP Server

Some detailled documentation, including a Doxygen Doc, is available in the `documentation` folder.

Run `./archive.sh` to create an archive of the project.

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
<ip>   www.fake.com
<ip>   www.toto.com
```
We recommand using a local IP address, such as 127.0.0.2. The server is, however, listening to the 8080 port regardless of the IP address.

To do this, you can execute the following command in `server/src` (being in the sudousers is required) :
```
make hosts
```

## Compilation

Before compilation, in `server/src`, export lib path and install libmagic : 
```
export LD_LIBRARY_PATH=.
sudo apt-get update
sudo apt-get -y install libmagic-dev
```

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

The server was built with the following architecture :
* It hosts two sites : master-site and hidden-site.
  * If no host specified in HTTP/1.0, the default host (master-site) is used.
* If the "/" target is asked, the target is changed to "/index.html". 
* It supports GET, HEAD, and POST requests
* It supports percent encoding.
* It supports dot segment removal.
* The sever supports any media type, plus TXT, HTML, CSS, and JS files. 
* If the client request a HTTP Version other than 1.0 or 1.1, we send a 505 (HTTP Version not supported) error. An other conception choice would have been to retrofit to 1.1 if the version was 1.x (with x > 1).
* It doesn't use chunked transfer encoding to send the answer to the PHP motor, it stores the answer in a buffer then sends it, we found this being more reliable.

The server handles the following error codes :
  * 200 : OK
  * 400 : Bad Request
  * 404 : Not Found
  * 501 : Not Implemented
  * 500 : Internal Server Error
  * 505 : HTTP Version not supported

# Tests

You can find :
* Some CSS in every page.
* A JS script in [hidden-site:8080/index.html](hidden-site:8080/index.html). It changes randomly the background color of the page and the color of the text.
* Images and icons in [master-site:8080/index.html](master-site:8080/index.html) and [hidden-site:8080/index.html](hidden-site:8080/index.html)
* A pdf in [master-site:8080/about.html](master-site:8080/about.html)
* A video in [hidden-site:8080/aboutme.html](hidden-site:8080/aboutme.html). 
* A form in [master-site:8080/contact.html](master-site:8080/contact.html). It works with GET and POST, and prints the result on the page.
