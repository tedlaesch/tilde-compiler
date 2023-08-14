# tilde-compiler
A compiler to turn a simple language Tilde into the ASM assembly language used by UMSL

## How to run
For compilation on a Windows machine, make sure g++ and make tools are installed and run make in the source directory.
For compilation on a Unix machine, rename makefile-unix to makefile and run make. The only difference between the two files is rm versus del.

To run the Tilde compiler, call it from the command line, optionally providing the file name of Tilde source code. The -h flag can be provided instead to display the help message. If run without a file name, the compiler will ask for input from the keyboard. 

## Output
The compiler generates UMSL ASM code, a simple assembly language. Details and an online interpreter can be found here: https://comp.umsl.edu/assembler/index

## Tilde Language Grammar
The grammar for the Tilde Language is as follows
```
S -> A J
A -> ^ t1 A || ε
C -> * K
D -> [ t1
E -> ] K ~
F -> | G ~
G -> K Gp
Gp -> K H || ε
H -> { || }
J -> C ~ J ~ L || D ~ J || E ~ J || t1 F ~ J || ε
K -> t1 || t3
L -> D ~ || E ~ || F ~ || ε
```
The terminals for the Tilde Language are
```
t1 -> variables (a character A-z followed by an "_" followed by a digit 0-9)
t2 -> symbols (^, ~, *, |, [, ], {, })
t3 -> numbers (a "+" or a "-" followed by one or more digits 0-9)
```

## Tilde Language instructions 
Comments in code are surrounded by "&" symbols as shown below
```
^ c_9
^ f_5 & This is a comment &
^ a_2
```
Allocation of memory for a variable (also initializes to 0)
```
^ t1
```
Read number from keyboard and assign to variable
```
[ t1
```
Write out value of variable or number
```
] t1 ~
```
or
```
] t3 ~
```
Add numbers (or variables) and assign to variable
```
t1 | t3 t3 { ~
```
or 
```
t1 | t1 t1 { ~
```
Subtract numbers (or variables) and assign to variable
```
t1 | t3 t3 } ~
```
or 
```
t1 | t1 t1 } ~
```
Assign number or variable to variable
```
t1 | t3 ~
```
or
```
t1 | t1 ~
```
If statement (if variable after * is positive do C else L) (C is any previous instruction) (L is limited to read and write)
```
* t1 ~ C ~ L
```

Examples of all instructions can be found in the tilde_test_code file.
