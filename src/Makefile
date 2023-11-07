FLAGS = -Wall -Werror -Wextra -std=c++17

all:	lex parse calc format scrypt

lex: 	lex.cpp ./lib/lexer.cpp
	g++ -o lex $(FLAGS) lex.cpp ./lib/lexer.cpp 

parse: 	parse.cpp ./lib/lexer.cpp ./lib/parser.cpp
	g++ -o parse $(FLAGS) parse.cpp ./lib/lexer.cpp ./lib/parser.cpp 

calc:	calc.cpp ./lib/lexer.cpp ./lib/infixparser.cpp
	g++ -o calc $(FLAGS) calc.cpp ./lib/lexer.cpp ./lib/infixparser.cpp ./lib/value.cpp

format:	format.cpp ./lib/lexer.cpp ./lib/infixparser.cpp ./lib/statementparser.cpp
	g++ -o format $(FLAGS) format.cpp ./lib/lexer.cpp ./lib/infixparser.cpp ./lib/value.cpp ./lib/statementparser.cpp

scrypt:	scrypt.cpp ./lib/lexer.cpp ./lib/infixparser.cpp ./lib/statementparser.cpp
	g++ -o scrypt $(FLAGS) scrypt.cpp ./lib/lexer.cpp ./lib/infixparser.cpp ./lib/value.cpp ./lib/statementparser.cpp