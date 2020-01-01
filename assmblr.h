//
// Created by jrinder on 2/20/18.
//

#ifndef SCANNER01_ASSMBLR_H
#define SCANNER01_ASSMBLR_H

void createNewAsmFle(char *);

void nullLine();

void printBeginAsm();

void writeLnesAsm(char *, char *);

void printLines(char *, char *, char *, char *);

void printNums(char *);

void printIdentifs(char *);

void printAssngment(char *);

void printAdditionLines(char *);

void printMultiLines(char *);

void printWritLn(char *, char*);

void printEndAsm();

void printProcdrPro(char *, char *);

void printMultiOps(char *op, char *ctype);

#endif //SCANNER01_ASSMBLR_H
