# Introduction

This project is the homework assignment for the Programming Languages (PL) course (Spring 2020) at Chung Yuan Christian University (CYCU). The project consists of three parts, each building on the previous one to create a comprehensive interpreter with enhanced features and functionality. It supports basic computational operations, array manipulations, loop constructs, syntax checks, and error handling. 

# Implementation

In the first part, we write an interpreter capable of interpreting commands using a defined syntax. This involves parsing the commands, executing them, and producing the expected outputs.

```plaintext
<Command>      ::= <Statement> ';' | <BooleanExp> ';' | <ArithExp> ';' | QUIT
<Statement>    ::= IDENT ':=' <ArithExp>
<BooleanExp>   ::= <ArithExp> ( '=' | '<>' | '>' | '<' | '>=' | '<=' ) <ArithExp>
<ArithExp>     ::= <Term> | <ArithExp> '+' <Term> | <ArithExp> '-' <Term>
<Term>         ::= <Factor> | <Term> '*' <Factor> | <Term> '/' <Factor>
<Factor>       ::= [ SIGN ] NUM | IDENT | '(' <ArithExp> ')'
```

The second part extends the functionality of the interpreter by adding a syntax checker to validate the commands. Additionally, a pretty printer is implemented to format the commands and system-supported functions in a readable manner. This part also supports error handling, ensuring they are correctly interpreted and displayed.  

The final part enhances the second part by supporting more complex operations and grammar checks.

