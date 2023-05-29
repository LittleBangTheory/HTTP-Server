#include <stdio.h>
#include <stdlib.h>
#include <magic.h>
#include <string.h>

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

    char* final_mime_type;
    mime_type = magic_file(magic_cookie, argv[1]);
    // If type is a plain/text, we need to check if it is a CSS or a XML file 
    if(strcmp(mime_type,"text/plain") == 0){
        // If the file is a CSS file
        if(strstr(argv[1],".css") != NULL){
            mime_type = "text/css";
        }
        // If the file is a XML file
        else if(strstr(argv[1],".xml") != NULL){
            mime_type = "text/xml";
        }

        // Add the encoding
        char* encoding = "; charset=utf-8";
        // Allocate the memory
        final_mime_type = malloc(sizeof(char)*(strlen(mime_type)+strlen(encoding)+1));
        // Concatenate the string
        sprintf(final_mime_type,"%s%s",mime_type,encoding);
    } else {
        final_mime_type = malloc(sizeof(char)*(strlen(mime_type)+1));
        sprintf(final_mime_type,"%s",mime_type);
    }
    printf("%s\n", final_mime_type);

    free(final_mime_type);
    magic_close(magic_cookie);
    return 0;
}