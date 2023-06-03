# HTTP Parser

## Description

Parser for the entire HTTP grammar. The programme can parse a request of the HTTP grammar (see the [ABNF](rfc/allrfc.abnf)), by generating a tree that contains the different elements of the request, in the form of a chained list.

Each node (element) is linked in two directions. A brother (horizontally), and a son (vertically). The first node is the root of the tree, and here, it is the `http-request` element.


## Contains
Code :
* [Source files](src)
* [Header files](headers)

Utilities :
* Makefile to generate the executable file 
    ```
    make
    ```
* A doxyfile to generate the doxygen documentation

Scripts :
* `askabnf.sh` to search in the [ABNF](rfc/allrfc.abnf) file the grammar of the element you want to parse
    ```
    ./askabnf.sh <element>
    ```
* `compare.sh` to compare our parser's output with the reference one's
    ```
    ./compare.sh <filename>
    ```
* `test_requests.sh` to tests the parser for every file of a specified folder
    ```
    ./test_requests.sh <foldername>
    ```

Docs :
* RFCs and a tree view (in Mermaid) : `rfc/`
* Test files : `alltests/`
    * Simple tests : `alltests/simpletests.zip/`
    * Medium tests : `alltests/mediumtests.zip/`
    * Hard tests : `alltests/hardtests.zip/`

## How to use it

```
make
./ourParser <testfile> [<field to locate>]
```
### Example
```
./ourParser alltests/request header-field
```

## Understanding the code
* src/abnf.c : Contains the functions to parse the grammar
* src/utility.c : Contains the functions to print the tree, and to test the existence of an element (of type `int is<element>()`)
* src/api.c : Contains the api functions to allow the use of the parser in another executables
* src/main.c : Contains the main function that call the other three 
