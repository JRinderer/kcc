//
// Created by jrinder on 1/3/18.
//

#ifndef SCANNER01_SCANNER_H
#define SCANNER01_SCANNER_H

char mkeUprCse(char);
int testChar(char, char);
char getNxtPntdVal(FILE *);
int charType(char);
void build2dArry(char arry[LIMIT][MAX],int, int, char, FILE *, int);
void buildString(char arry[LIMIT][MAX],int, int, char, FILE *, int, char);
void build2dArryOps(char arry[LIMIT][MAX],int, int, char, FILE *);
int isExAcceptableChar(char);
void printTokens();
int isKeyWord(char *);
char build2dArryNum(char arry[LIMIT][MAX],int, int, char, FILE *);
void writeLnes(char *, char *);
int is2dOperator(char *);
int sizeOfPunctArry(char);
int isPunctPartOf2D(char, char);
void parseProg(char *);
void parseDcls(char*, char*);
char rewnd(char, FILE *);
int isPunct(char);
#endif //SCANNER01_SCANNER_H
