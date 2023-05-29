#include <stdio.h>
#include <stdlib.h>
#include <magic.h>

int main(int argc, char **argv) {
    magic_t magic_cookie;
    const char *mime_type;

    magic_cookie = magic_open(MAGIC_MIME_TYPE);
    if (magic_cookie == NULL) {
        printf("unable to initialize magic library\n");
        return 1;
    }

    if (magic_load(magic_cookie, NULL) != 0) {
        printf("cannot load magic database - %s\n", magic_error(magic_cookie));
        magic_close(magic_cookie);
        return 1;
    }

    mime_type = magic_file(magic_cookie, argv[1]);
    printf("%s\n", mime_type);

    magic_close(magic_cookie);
    return 0;
}