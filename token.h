//
// Created by jrinder on 1/3/18.
//

#ifndef SCANNER01_TOKEN_H
#define SCANNER01_TOKEN_H

#define MAX 80
#define LIMIT 3000

typedef enum {
    CONSTANT,
    KEYWORD,
    NUMBER,
    REL_OP, 	// such as ==  <  >  =!=    =>  =<
    OPERATOR,			// such as = :  +  -  *  / %
    PUNCT,		// such as . (  ) , { } ; [ ]

    UNDEF,		// undefined
    EOT 		// end of token
/*
	REL_OP_EQUAL, // ==
	REL_OP_GREATER, // >
	REL_OP_LESS_THAN, //
	REL_OP_NOT_EQUAL, // =!=
	REL_OP_NOT_GREATER, // =<
	REL_OP_NOT_LESS_THAN, // =>

	OP_ASSIGN, // =
	OP_COLON, // :
	OP_DIV, // /
	OP_MUL, // *
	OP_ADD, // +
	OP_SUBTRACT, // -
	OP_REMAINDER, // %

	DELIM_DOT, // .
	DELIM_LEFT_PARRENTHESE, // (
	DELIM_RIGHT_PARRENTHESE, // )
	DELIM_COMMA, // ,
	DELIM_LEFT_BRACE, // {
	DELIM_RIHGT_BRACE, // }
	DELIM_LEFT_BRACKET, // [
	DELIM_RIGHT_BRACKET, // ]
	DELIM_SEMICOLON, // ;
*/

} TokenType;

typedef struct {
    TokenType tokenType;
    char *instance;
    int lineNum;

} Token;

#endif //SCANNER01_TOKEN_H
