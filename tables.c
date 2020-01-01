
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "tables.h"

#include <memory.h>
#include <stdio.h>
#include "tables.h"

//===================================all structures created here======================================================
VARBLEGLBL varTble[SIZE]; //global variable structure
PROCGLBL proTble[SIZE]; //global procedure structure
TYPEGLBL typTble[SIZE]; //global type table
GLBLLTRL glbLtrl[SIZE]; //GLOBAL LITERAL TABLE
VARBLELCL lclVarTble[SIZE]; //local variable table
LABELLCL lclLablTble[SIZE]; //local label table

//===================================BEGIN GLOBAL VARIABLE TABLE======================================================
void GSTaddToTble(char *ident, char *type, int memLoc, int updteble, int nmeConst, char *constVale, int key){
    if(key<0){
        key=GSTstructSize();
    }
    strcpy(varTble[key].ident,ident);
    strcpy(varTble[key].type,type);
    varTble[key].memLoc=memLoc;
    varTble[key].updteble=updteble;
    varTble[key].nmeConst=nmeConst;
    strcpy(varTble[key].constVal,constVale);
    varTble[key].key=key;
}

int GSTsearchTble(char *ident){
    char *compStr;
    int size;

    size = GSTstructSize();

    for(int i =0; i<size;i++){
        compStr = varTble[i].ident;
        if(!strcmp(compStr,ident)){
            return i; //this effectively returns the KEY
        }
    }
    return -1;
}

char *GSTStrngRet(int key, int option){
    //based on the option number we return one of the structures variables

    if(option==1){
        return  varTble[key].ident;
    }else if(option ==2){
        return  varTble[key].type;
    }else if(option==3){
        return varTble[key].constVal;
    }
}

int GSTintRet(int key, int option){
    if(option==1){
        return varTble[key].memLoc;
    }else if(option==2){
        return varTble[key].updteble;
    }else if(option==3){
        return varTble[key].nmeConst;
    }
    //Key should never need to be extracted from this option.
}

int GSTstructSize(){
    int i = 0;

    while(strcmp(varTble[i].ident,"")){
        i++;
    }
    return i;

}
//=====================================END GLOBAL VARIABLE TABLE======================================================
void GLTaddToTble(char *labl, char *type, char *value, int key){
    if(key<0){
        key=GLTstructSize()+1;
    }
    glbLtrl[key].key = key;
    strcpy(glbLtrl[key].ident,labl);
    strcpy(glbLtrl[key].type,type);
    strcpy(glbLtrl[key].value,value);
}

int GLTsearchTble(char *ident){
    char *compStr;
    int size;

    size = GLTstructSize();

    for(int i =0; i<size;i++){
        compStr=glbLtrl[i].ident;
        if(!strcmp(compStr,ident)){
            return i;
        }
    }
    return -1;
}

char *GLTStrngRet(int key, int option){
    if(option == 1){
        return glbLtrl[key].ident;
    }else if(option == 2){
        return glbLtrl[key].type;
    }else if(option == 3){
        return glbLtrl[key].value;
    }
}

int GLTstructSize(){
    int i = 0;

    while(strcmp(glbLtrl[i].ident,"")){
        i++;
    }
    return i;
}

//=====================================GLOBAL LITERALS TABLES=========================================================


//=====================================BEGIN GLOBAL PROCEDURE TABLE===================================================
void GPTaddToTble(char *ident, int args, int byValvar, char *argIdent, char *retType, int memLoc, int key){
    if(key<0){
        key=GPTstructSize();
    }
    strcpy(proTble[key].ident,ident);
    proTble[key].memLoc=memLoc;
    proTble[key].args=args;
    proTble[key].byValvar=byValvar;
    proTble[key].key=key;
    strcpy(proTble[key].retType,retType);
    strcpy(proTble[key].argIdent,argIdent);

    int s;

    s = GPTstructSize();
}

int GPTsearchTble(char *ident){
    char *compStr;
    int size;

    size = GPTstructSize();
    for(int i=0;i<size;i++){
        compStr=proTble[i].ident;
        if(!strcmp(compStr,ident)){
            return i;
        }
    }
    return -1;
}

char *GPTStrngRet(int key, int option){
    if(option==1) {
        return proTble[key].ident;
    }else if(option==2) {
        return proTble[key].retType;
    }else if(option==3){
        return proTble[key].argIdent;
    }
}

int GPTIntRet(int key, int option){
    if(option==1){
        return proTble[key].args;
    }else if(option==2){
        return proTble[key].byValvar;
    }else if(option==3){
        return proTble[key].memLoc;
    }
}

int GPTstructSize(){
    int i = 0;

    while(strcmp(proTble[i].ident,"")){
        i++;
    }
    return i;
}
//====================================END GLOBAL PROCEDURE TABLE======================================================

//====================================BEGIN GLOBAL TYPE TABLE=========================================================
void GTTaddTotble(char *ident, int bites, int type, char *baseType, int key, int memLoc){
    if(key<0) {
        key = GTTstructSize();
    }
    strcpy(typTble[key].ident,ident);
    strcpy(typTble[key].baseType,baseType);
    typTble[key].bites=bites;
    typTble[key].key=key;
    typTble[key].type=type;
    typTble[key].memLoc=memLoc;
}

int GTTsearchTble(char *ident){
    char *compStr;
    int size;

    size = GTTstructSize();
    for(int i=0;i<size;i++){
        compStr=typTble[i].ident;
        if(!strcmp(compStr,ident)){
            return i;
        }
    }
    return -1;
}

int GTTstructSize(){
    int i = 0;

    while(strcmp(typTble[i].ident,"")){
        i++;
    }
    return i;
}

int GTTsizeBits(char *ident){
    int key;
    key = GTTsearchTble(ident);
    return (typTble[key].bites);
}

//====================================END GLOBAL TYPE TALBE===========================================================
void popTables(){

    GSTaddToTble("L_T","REAL",0,0,0,"",-1);

    GTTaddTotble("BOOL",8,0,"",-1,0);
    GTTaddTotble("INT",8,0,"",-1,0);
    GTTaddTotble("REAL",8,0,"",-2,0);
    GTTaddTotble("STRING",-1,0,"",-3,0);

    GPTaddToTble("MAIN",0,0,"","INT",0,-1);
    LSTaddToTble("TEST","REAL","","",-1);//use to ensure localTable is destroyed
    LLTaddToTable("TEST","REAL",-1);//use to ensure localTable is destroyed

}

//====================================================================================================================

//====================================CLEAR THE LOCAL TABLES==========================================================
//THESE ARE VARIABLE TABLES----------------------------------------------------------------------
void clearLocalTbles(){//char *ident, char *type, int memLoc, int updteble, int nmeConst, char *constVale, int key){
    memset(&lclVarTble,0,sizeof lclVarTble);

}
//==============================LOCAL SYMBOL/VARIBLE TABLE==========================================================
void LSTaddToTble(char *ident, char *type, char *offSet, char *callByType, int key){
    if(key<0){
        key=LSTstructSize();
    }
    strcpy(lclVarTble[key].ident,ident);
    strcpy(lclVarTble[key].type,type);
    strcpy(lclVarTble[key].offSet,offSet);
    strcpy(lclVarTble[key].callByType,callByType);
    lclVarTble[key].key=key;
}


int LSTsearchTble(char *ident){
    char *compStr;
    int size;

    size = LSTstructSize();
    for(int i=0;i<size;i++){
        compStr=lclVarTble[i].ident;
        if(!strcmp(compStr,ident)){
            return i;
        }
    }
    return -1;
}

int LSTstructSize(){
    int i = 0;

    while(strcmp(lclVarTble[i].ident,"")){
        i++;
    }
    return i;
}

//==============================END OF LOCAL SYMBOL/VARIABLE TABLE==================================================
void printIdent(){
    printf("\nThe identifier for this is %s\n",lclLablTble[0].ident);

}

//THESE ARE LABLE TABLES--------------------------------------------------------------------------
void LLTaddToTable(char *ident, char *label, int key){
    if(key<0){
        key=LLTstructSize()+1;
    }
    strcpy(lclLablTble[key].ident,ident);
    strcpy(lclLablTble[key].label,label);
    lclLablTble[key].key=key;
}


int LLTstructSize(){
    int i = 0;

    while(strcmp(lclLablTble[i].ident,"")){
        i++;
    }
    return i;
}

char *LLTStrngRet(int key, int option){
    if(option==1) {
        return lclLablTble[key].ident;
    }else if(option==2) {
        return lclLablTble[key].label;
    }
}

int LLTsearchTble(char *ident){
    char *compStr;
    int size;

    size = LLTstructSize();
    for(int i=0;i<size;i++){
        compStr=lclLablTble->ident;
        if(!strcmp(compStr,ident)){
            return i;
        }
    }
    return -1;
}

void clearLocalLbls(){
    memset(lclLablTble,0,sizeof lclLablTble);
}



//=============================NOT TABLE FUNCITON BUT SIMILAR====================================================
char *tokenType(char *tok){
    if(!strcmp(tok,"INT")){
        return "INT";
    }else if(!strcmp(tok,"REAL")){
        return "REAL";
    }else if(!strcmp(tok,"CSTRING")){
        return "STRING";
    }else if(!strcmp(tok,"BOOL")){
        return "BOOL";
    }
}