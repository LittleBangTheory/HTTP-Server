# Simple Grammar

PoC of the doability of a parser for the entire HTTP grammar. The programme can parse a request of a simple grammar, by generating a tree that contains the different elements of the request, in the form of a chained list.

Each node (element) is linked in two directions. A brother (horizontally), and a son (vertically). The first node is the root of the tree, and here, it is the `start` element.


## Contains
Code :
* [Source files](src)
* [Header files](headers)
* [Makefile to generate the executable file](Makefile)
* [A doxyfile to generate the doxygen documentation](Doxyfile)
* [A bash script to automatically test all the files in the test folder](test.sh)
* [An executable test file that served as the goal to follow during the development](testsimplegrammar)

Docs :
* [Documents relatives to the projet](docs)
    *  [Grammar list](docs/simpleabnf.abnf)
    *  [Grammar tree wrote in mermaid](docs/simple_parser.md)
* [Test files](testFile)

## How to use it

```
make
./simplegrammar.exe testFile/<filename>.txt
```	

## Understanding the code
* [abnf.c](src/abnf.c) : Contains the functions to parse the grammar
* [utility.c](src/utility.c) : Contains the functions to print and delete the chained list
* [main.c](src/main.c) : Contains the main function that call the other two
