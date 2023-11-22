FLAGS = -g -Wall -Werror -Wextra -std=c++17

all:	lex calc format scrypt

lex: 	lex.cpp ./lib/lexer.cpp
	g++ -o lex $(FLAGS) lex.cpp ./lib/lexer.cpp 

calc:	calc.cpp ./lib/lexer.cpp ./lib/infixparser.cpp ./lib/function.cpp
	g++ -o calc $(FLAGS) calc.cpp ./lib/lexer.cpp ./lib/infixparser.cpp ./lib/value.cpp ./lib/function.cpp

format:	format.cpp ./lib/lexer.cpp ./lib/infixparser.cpp ./lib/statementparser.cpp ./lib/function.cpp
	g++ -o format $(FLAGS) format.cpp ./lib/lexer.cpp ./lib/infixparser.cpp ./lib/value.cpp ./lib/statementparser.cpp ./lib/function.cpp

scrypt:	scrypt.cpp ./lib/lexer.cpp ./lib/infixparser.cpp ./lib/statementparser.cpp ./lib/function.cpp
	g++ -o scrypt $(FLAGS) scrypt.cpp ./lib/lexer.cpp ./lib/infixparser.cpp ./lib/value.cpp ./lib/statementparser.cpp ./lib/function.cpp

