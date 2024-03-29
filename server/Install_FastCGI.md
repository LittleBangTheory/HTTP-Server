# Configuration

## Apache

Installation d'apache2 :
```
#apt-get update 
#apt-get install apache2
```

Vérification que le serveur écoute sur 80 : 
```
#netstat -antp
```

## PHP

Installation de php7 :
```
apt-get install php-fpm
```

Modifier la ligne 36 de `/etc/php/7.x/fpm/pool.d/www.conf` :
```
listen = 9000
```
Relancer le processus et vérifier son écoute :
```
systemctl restart php7.x-fpm
netstat -antp
```

## Configuration du serveur

Installer le module pour que Apache redirige les requêtes PHP :
```
a2enmod proxy_fcgi 
systemctl restart apache2
```

Ajouter dans la section `virtualhost` de `/etc/apache2/sites-enabled/000-default.conf` :
```apacheconf
<FilesMatch "\.php$">
    SetHandler "proxy:fcgi://127.0.0.1:9000/"
</FilesMatch>
```

Créer le script `/var/www/html/info.php` et ajouter :
```php
<?php phpinfo(); ?>
```

## Test

Aller sur `http://127.0.0.1/info.php` et regarder avec Wireshark les échanges entre le serveur et le client. Filtrer en TCP sur le port 9000 (`tcp.port == 9000` sur la loopback), et `Decode as` FCGI. 


# Specifications

See [FastCGI Specification](http://www.mit.edu/~yandros/doc/specs/fcgi-spec.html) by Mark R. Brown.
