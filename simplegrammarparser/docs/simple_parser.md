On veut parser la grammaire suivante :
;SP = %x20
;DIGIT =  %x30-39
;ALPHA =  %x41-5A / %x61-7A   ; A-Z / a-z
;HTAB = %x09
;LF = %x0A 
nombre = 1*DIGIT 
ponct = "," / "." / "!" / "?" / ":"
separateur = SP / HTAB / "-" / "_" 
debut = "start" 
fin = "fin" 
mot = 1*ALPHA separateur  
message = debut 2*( mot ponct /nombre separateur ) [ponct] fin LF 

::: mermaid
graph TD
0(message) --> 1(debut)
0 --> 2(mot ponct /nombre separateur)
0 --> 3([ponct])
0 --> 4(fin)
0 --> 5(LF)

1 --> 10("start")
2 -->|OU| 11(mot ponct)
2 -->|OU| 12(nombre separateur)

11 --> 21(mot)
11 --> 22(ponct)
21 --> 30(1*ALPHA separateur)
30 --> 31(1*ALPHA)
31 -->|OU| 33(%x41-5A)
31 -->|OU| 34(%x61-7A)
30 --> 35(separateur)
35 -->|OU| 36(SP)
35 -->|OU| 37(HTAB)
35 -->|OU| 38("-")
35 -->|OU| 39("_")

22 -->|OU| 40(",")
22 -->|OU| 41(".")
22 -->|OU| 42("!")
22 -->|OU| 43("?")
22 -->|OU| 44(":")

12 --> 50(nombre)
50 --> 51(1*DIGIT)
51 --> 52(%x30-39)
12 --> 53(separateur)
:::

Structure : 
* label
* ptr début chaine
* int longueur chaine
* ptr frere
* ptr fils
* ptr père ?

Une fonction par mot de grammaire ? Ou une fonction récursive générale ? Comment gérer les caractères ?