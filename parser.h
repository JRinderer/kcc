//
// Created by jrinder on 2/9/18.
//

//#include <bits/types/FILE.h>

#ifndef SCANNER01_PARSER_H
#define SCANNER01_PARSER_H


char *typeToAdd;
char *valToAdd;
int procNUm;

void startParser();
void skipTabs(char c);
int isEmpty(FILE *);
FILE *filePntr; //made this a variable within parser.h due to issue passing file Pointers from function and returning
//the pointer
int wordNum;
char *lexm, *tokn;
void getNextStrngArry(int);
void matchLexTok(char *, char *,char *);
int compLexTok(char *, char *, char *);
void beginParser();
int parseDecDefs(char *);
void parseProc();
void parseTypDef();
void parseStatmntsLst();
void parseIndvStatmnt();
void parseWrtLne();
void parseAssngmnt();

//oldPal code
//void parseExprsn();
char *parseExprsn();
char *parseExprsn2();

char *parseTerms();

//old pal CODE
//void parseTerms();

void parseTypes();
void parseProcs();
void parseProcList();
void parseProcedure();
int allgnMent(int);
char *parseLbl(char *);
int parseHeaders();
char *parseDecTypes();
void parseBodies(int);
void parseExctblStats();
void parseAddTerms();
char *parseFactor();
void parseAddExprsn();
int parseDecList(char *);
char *parseLocalVal(char *);
char endChar;
int arryStrt;
int identifSize;
char *identifer[LIMIT];
void parseGlobal();
void parseConsts();
void parseConstLsts();
void parseConstnts();
int searchVarblTble(char *val, int);
//FUNC DEFS FOR EXECUTBLES
void parseReads();
void parseWrites();
void parseAssng();
void parseCalls();
void parseRets();
void parsegoTo();
void parseVoids();
void parseDos();
void parseIfs();
void parseWhles();
void parseCases();
void parseFors();
void parseNexts();
void parseBreaks();
//=========================

void parseArgs(char *);
void parseArgsList(char *, char *);


char *parseLval(char *);

char *parseComprs();
char *parseExprsn2();

int parseUnOps();

char *parseQuals(char *);

char *parseFuncs(char *);

int isMultiOp(char *);

int isAddOp(char *);

int isComparsonOp(char *);

char numToChar(int);

void VarbleInfo(char *lclIdent, char* lclType, char *addr, char *ptr);

#endif //SCANNER01_PARSER_H
