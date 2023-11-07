# cs32-scrypt04
Group members: Aneesh Garg, Jiesheng He, Allen Qiu, Justin Wu

## General Purpose


## How to compile/run
`cd` to the `src` directory in the repository and run `make`. This should create 5 executable files - `calc`, `format`, `lex`, `parse`, and `scrypt`. While in the `src` directory, run `./{executable_name}` to run the specified executable. Input your 
statement(s), then press Ctrl + D to complete input.

`lex` is an executable that runs the lexer. After the user completes input, it prints the vector of tokens produced by the lexer. \
`parse` is obsolete, and does not support the new features of the language. \
`calc` is an executable that runs the infix parser. After every line the user enters, it prints the resulting tree and an evaluation for it. \
`format` is an executable that runs the statement parser. After the user completes input, it prints the tree of the given program. \
`scrypt`, like `format`, also runs the statement parser. After the user completes input, it evaluates the tree of the program.

## Branch Structure


## File Structure
