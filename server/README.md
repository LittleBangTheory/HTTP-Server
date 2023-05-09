
# Libparser

`libparser.so` : parser précompilé
* Peut être compilé avec :
  * ``gcc -Wall -g -o sock main.c -L. -lparser``
  * ``export LD_LIBRARY_PATH=.``
* Ou est utilisé par le Makefile pour compiler `httpparser`

`httpparser` : parser compilé, dont on peut modifier le ``main.c`` et l'API, qui utilise `libparser.so`

# Configuration

If you are a h@ck3r, use Burpsuite to intercept the request and modify the Host header to match "hidden-site" ou "master-site". "Localhost" is not supported in HTTP/1.1.

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

# Conception
 
If no host specified in HTTP/1.0, the default host (master-site) is used.

If the "/" target is asked, the target is changed to "/index.html". 

The server can't send a 501 (Unauthorized method) error, because there are no unauthorized methods, only unimplemented ones.

# Tests

You can find some Javascript in [hidden-site:7777/index.html](hidden-site:7777/index.html)

You can find some CSS in every page.

You can find images and icons in [master-site:7777/index.html](master-site:7777/index.html) and [hidden-site:7777/index.html](hidden-site:7777/index.html)

You can find a pdf in [master-site:7777/about.html](master-site:7777/about.html)

You can find a video in [hidden-site:7777/aboutme.html](hidden-site:7777/aboutme.html)
