
//
// Created by jrinder on 4/16/18.
//

#ifndef SCANNER01_TABLES_H
#define SCANNER01_TABLES_H

#include <lzma.h>

typedef char Str100[10000];

#define SIZE 800

void popTables();

//=====================================GLOBAL VARIABLE TABLE===========================================================
typedef struct varTble{
    Str100 ident;
    Str100 type;
    int memLoc;
    int updteble;
    int nmeConst;
    Str100 constVal;
    int key;

} VARBLEGLBL;


void GSTaddToTble(char *, char *, int, int, int, char *, int);

int GSTsearchTble(char *);

int GSTstructSize();

char *GSTStrngRet(int, int);

int GSTIntRet(int, int);
//======================================END GLOBAL VARIABLE TABLE======================================================

//======================================GLOBAL PROCEDURE TABLE=========================================================
typedef struct proTble{
    Str100 ident;
    int args;
    int byValvar;
    Str100 argIdent;
    Str100 retType;
    int memLoc;
    int key;
} PROCGLBL;

void GPTaddToTble(char *, int, int, char *, char*, int, int);

int GPTsearchTble(char *);

char *GPTStrngRet(int,int);

int GPTIntRet(int,int);

int GPTstructSize();
//======================================END GLOBAL PROCEDURE TABLE=====================================================

//======================================GLOBAL TYPE TABLE==============================================================
typedef struct typeTble{
    Str100 ident;
    int bites;
    int type;
    Str100 baseType;
    int key;
    int memLoc;
} TYPEGLBL;

void GTTaddTotble(char *, int, int, char *, int, int);

int GTTsearchTble(char *);

int GTTstructSize();

int GTTsizeBits(char *);

//======================================END GLOBAL TYPE TABLE==========================================================

//======================================GLOBAL LITERAL TABLES==========================================================

typedef struct glLtrlTble{
    Str100 ident;
    Str100 type;
    Str100 value;
    int key;


} GLBLLTRL;

void GLTaddToTble(char *, char *, char *,int);

int GLTsearchTble(char *);

int GLTstructSize();

char *GLTStrngRet(int,int);

//========================================UNIVERSAL FUNCTIONS==========================================================

void clearLocalLbls();
//SymbolTableLocal=====================================================
typedef struct LVarTble{
    Str100 ident;
    Str100 type;
    Str100 offSet;
    Str100 callByType;
    int key;

} VARBLELCL;


void clearLocalTbles();//char *, char *, int, int, int, char *, int);

void LSTaddToTble(char *, char *, char *, char *, int);

int LSTsearchTble(char *);

int LSTstructSize();

char *LSTStrngRet(int, int);

void printIdent();

//LableTableLocal=====================================================

typedef struct LlblTble{
    Str100 ident;
    Str100 label;
    int key;
} LABELLCL;

void LLTaddToTable(char *, char *, int);



int LLTsearchTble(char *);

int LLTstructSize();

char *LLTStrngRet(int, int);

char *tokenType(char *);

#endif //SCANNER01_TABLES_H

