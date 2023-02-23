Objectif : créer un arbre de dérivation complet à partir de ```HTTP-message = start-line * ( header-field CRLF ) CRLF [ message-body ]```
 

::: mermaid
graph TD;
    0(http-message)-->1(start-line);
    0-->2(header-field);
    0-->3(CRLF);
    0-->4(CRLF);
    0-->5(message-body);
    1-->10(request-line);

    10-->11(method);
    10-->12(SP);
    10-->13(request-target);
    10-->14(SP);
    10-->15(http-version);
    10-->16(CRLF);
    11-->20(token);

    20-->21(*Détailler les méthodes*);
    13-->22(origin-form);
    22-->23(absolute-path);
    
:::