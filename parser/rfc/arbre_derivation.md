Objectif : créer un arbre de dérivation complet à partir de ```HTTP-message = start-line * ( header-field CRLF ) CRLF [ message-body ]```
 

::: mermaid
graph TD;
    0(http-message)-->1(start-line);
    0-->2([*header-field CRLF]);
    2-->3(header-field);
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

    20-->21(1*tchar)
    13-->22(origin-form)
    22-->23(absolute-path)
    23-->25(1*/segment)
    22-->24([?])
    22-->29([query])
    15-->26(HTTP-name)

    25-->|segment|30(1*pchar)
    30-->|pchar|31(unreserved)
    30-->|pchar|32(pct-encoded)
    30-->|pchar|33(sub-delims)
    30-->|pchar|34(:)
    30-->|pchar|35("@")
    29-->36(*pchar/'/'/'?')

    36-->|pchar|37(unreserved)
    36-->|pchar|38(pct-encoded)
    36-->|pchar|39(sub-delims)
    36-->|pchar|40(:)
    36-->|pchar|41("@")
    
    26-->50(%x48.54.54.50)
    15-->51(/)
    15-->52(DIGIT)
    15-->53(.)
    15-->54(DIGIT)

    3-->60(field-name)
    3-->61(:)
    3-->62(field-value)
:::
