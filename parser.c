//
// Created by jrinder on 2/9/18.
//
#include "token.h"
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include "parser.h"
#include "scanner.h"
#include "assmblr.h"
#include "tables.h"

char token[LIMIT][MAX];
char lexem[LIMIT][MAX];
int arryC=0;

char *TwoDimOp[6] = {"<>","<=",">=", "<",">","="};

char *cons[5] = {"NUMERIC","DECIMAL","REAL","CSTRING"};

char *lclId="";
char *lclTy="";
char lcladdr[LIMIT]="_";
int pntr=0;

void startParser(){
    char currentChar;
    int intI=0;
    int intJ=0;
    int intA=0;
    int intB=0;
    int chrType;
    int lineNo=1;
    int totalI=0;
    int isDecml = 0;
    int isCString = 0;

    //fillDumyData();//How best to fill in data

    //====================================OPEN THE .SCAN FILE==========================================================
    if ((filePntr = fopen("scnr.scan", "r"))==NULL){
        printf("Error unable to open .scan file.\n"); //if the file returns null this error is printed otherwise the
        //above statement sets the file pointer equal to the given file
    }
    //>>>>>>>>>>>>>>>>>>>>>>Before we can even begin parsing the TOKENS and LEXEME we have to build the chars into
    //>>>>>>>>>>>>>>>>>>>>>>words as we did in the scanner. Once we've built the words we can then begin parsing them
    //There is an issue in here with the .scan file where the CSTRINGS with a space ending in ' are being separated.
     fseek(filePntr, 0, SEEK_END);
     if(isEmpty(filePntr)==1){
         printf("The .scan file is empty\n");
     }else {
         rewind(filePntr);
         reset:
         while((currentChar=fgetc(filePntr))!='.'){
             chrType = charType(currentChar);
             if(currentChar=='\n'){
                 currentChar=fgetc(filePntr);
                 fseek(filePntr, -1, SEEK_CUR);
                 goto reset;
             }
             //need a test for decimal
             if(currentChar!='\t'){
                 build2dArry(token, intI, intJ, currentChar, filePntr, chrType);
                 if(!strcmp(token[intI],"DECIMAL")){
                     isDecml = 1;
                 }if(!strcmp(token[intI],"CSTRING")){
                     isCString = 1;
                 }
                 intI++; intJ=0;
             }else if(currentChar=='\t'){
                 if(isDecml == 1){
                     skipTabs(currentChar);
                     currentChar=fgetc(filePntr);
                     chrType=charType(currentChar);
                     build2dArryNum(lexem,intA,intB,currentChar,filePntr);
                     intA++; intB=0;
                     isDecml=0;
                 }else if(isCString == 1) {
                     skipTabs(currentChar);
                     currentChar = fgetc(filePntr);
                     chrType = charType(currentChar);
                     //buildString(words,intlArryi,intlArryj,chr,filePntr,chrType,'\'');
                     buildString(lexem, intA, intB, currentChar, filePntr, chrType, '\'');
                     currentChar=fgetc(filePntr);
                     intA++;intB=0;
                     isCString = 0;
                 }else{
                     skipTabs(currentChar);
                     currentChar=fgetc(filePntr);
                     chrType=charType(currentChar);
                     build2dArry(lexem, intA, intB, currentChar, filePntr, chrType);
                     intA++; intB=0;
                 }
             }
             totalI++;
             fseek(filePntr, -1, SEEK_CUR);
             if((testChar(currentChar,'\n')==0)){
                 lineNo++;
             }
         }
     }
    token[intA+1][0] = '.\0';
    //=================================End building chars into strings=================================================
    //===============================================Begin parser======================================================
    arryStrt = 0;
    tokn = token[arryStrt];
    lexm = lexem[arryStrt];

    beginParser();
    printf("The number of identifiers is : %d",identifSize);
}

void beginParser(){
    popTables();
    parseGlobal();
    parseDecDefs(procNUm);
    parseProcList();
    //getNextStrngArry(arryStrt);
    //displayTest();
    printf("The last token is %s",tokn);
}

void parseGlobal(){
    if(compLexTok(lexm,tokn,"GLOBAL")==1){
        return;
    }else{
        getNextStrngArry(arryStrt);
        parseConsts();
        //parseTypDefs();
        //parseDcls()
        parseProcs();

    }
}

void parseTypDefs(){
    if(compLexTok(lexm,tokn,"TYPE")==1){
        return;
    }else{
        getNextStrngArry(arryStrt);
        //parseTypes();
    }
}

void parseTypes(){

}

void parseProcList(){

    parseProcedure();
    while(compLexTok(lexm,tokn,"PROCEDURE")==0){
        parseProcedure();
    }

}

void parseProcedure(){
    int tmpProc;
    tmpProc = parseHeaders();
    parseBodies(tmpProc);


}

void parseBodies(int proc){
    char *procName;
    char *procType;
    int *procParam;// might need another parameter variable
    char *procRet;

    clearLocalTbles();
    clearLocalLbls();

    procName = GPTStrngRet(proc,1);//returns identifier
    procType = GPTStrngRet(proc,3);
    procRet = GPTStrngRet(proc,2);
    procParam = GPTIntRet(proc,1);//parameters might need to stored as a structure within a structure or an array...

    if(procParam==NULL){
        int offset =parseDecDefs("local");
        int algnMent = allgnMent(offset); //need to convert to string
        char numToStrn[25];
        sprintf(numToStrn,"%d", offset);
        matchLexTok(lexm,tokn,"BEGIN");
        printProcdrPro(procName,numToStrn);
        //emitProcesProlog(procName, algnMent); //has to be converted to string
        parseStatmntsLst();
    }//still need the false of this
}

int parseHeaders(){
    int t;
    matchLexTok(lexm,tokn,"PROCEDURE");
    matchLexTok(lexm,tokn,"IDENTIF"); //POINTER MUST NOT BE INCREMENTED
    //since pointer is incremented store the string array -1 in a variable
    char *tmpLexm;
    char *tmpTokn;
    tmpLexm = lexem[arryStrt-1];
    tmpTokn = token[arryStrt-1];
    //The code between these comments above to line 159 hold the previous TOKEN and LEXEM
    int proc; //return to this and the x in parseConstLsts
    proc = GPTsearchTble("MAIN");
    if(proc < 0){
        return 0;
        //error
    }else{
        matchLexTok(lexm,tokn,";");//lexm and token are off here
        return proc;
    }
    getNextStrngArry(arryStrt);

    //next we need to find where the procedure is on the global table

}

void parseProcs(){
    if(compLexTok(lexm,tokn,"PROC")){
        return;
    }else{
        getNextStrngArry(arryStrt);

    }
}

void parseConsts(){

    if(compLexTok(lexm,tokn,"CONST")==1){
        return;
    }else{
        getNextStrngArry(arryStrt);
        parseConstLsts();
    }

}

void parseConstLsts(){
    int i = 1;
    int x;
    int size;
    int y ;
    while(i != 0){
        //error is occuring in the building of strings
        i = 0;
        matchLexTok(lexm,tokn, "IDENTIF");
        char *tmpLexm; //this might need to be globally available
        tmpLexm=lexem[arryStrt-1]; //subtract one from array to get the previous lexeme as I've advanced it
        char *tmpTokn = token[arryStrt-1];
        identifer[arryC]=tmpLexm;
        identifSize++;
        arryC++;
        //getNextStrngArry(arryStrt);
        matchLexTok(lexm,tokn,":");
        parseConstnts();
        //Here is where we start to search the global DCL table first run should return a false
        x=searchVarblTble(tmpLexm,0);
        //also need to search Type, and procedure table
        //===============Need a method to find the current size of the table so I can auto index============
        if(x != 1){
            size = GSTstructSize();
            GSTaddToTble(tmpLexm,typeToAdd,0,0,0,valToAdd,-1);
        }
        //error handler here
        matchLexTok(lexm,tokn,";");
        if(compLexTok(tmpLexm,tokn,"IDENTIF")==0){
//            getNextStrngArry(arryStrt); //incrementing the pointer seems to be skipping...
            i = 1;
        }
        if(y==1);
        //getNextStrngArry(arryStrt);//incrementing the pointer seems to be skipping...
    }
}

void parseConstnts(){ //this is where I left off...
    //this first if needs to check if tokn is part of the constants array
    int cont = 0;

    for(int i = 0; i<4;i++){
        if(!strcmp(cons[i],tokn)){
            cont = 1;
            break;
        }
    }

    if(cont=1){
        //ensure these global values are reset
        typeToAdd ="";
        valToAdd = "";
        if(!strcmp(tokn,"NUMERIC")){
            typeToAdd="INT";
        }else if(!strcmp(tokn,"DECIMAL")){
            typeToAdd="REAL";
        }else{
            typeToAdd="STRING";
        }
        //Once I find out what the token is I need to store it's value
        valToAdd = lexem[arryStrt];
        getNextStrngArry(arryStrt);

    }else{
        printf("An error has occured in your CONSTANTS");
    }
}

int parseDecDefs(char *memLoc){ // this need to be changed to return an integer value
    if(compLexTok(lexm,tokn,"DCL")==1){
        return 0;
    }else{
        getNextStrngArry(arryStrt);
        return parseDecList(memLoc);
    }
}

/*caused errors wasn't returning what was expected
char numToChar(int x){
    char chr[24];
    sprintf(chr,"%d",x);
    return chr;
}
*/

int parseDecList(char *memLoc){

    int offset =0;
    int i = 1;
    char numToStrn[24];

    while(i!=0){
        i = 0;
        matchLexTok(lexm,tokn,"IDENTIF"); //DO NOT INCREMENT use tmp strings
        char *tmpTokn; //This will store the previous strings
        char *tmpLexm; //This will store the previous strings
        tmpTokn = token[arryStrt-1];
        tmpLexm = lexem[arryStrt-1];
        matchLexTok(lexm,tokn,":");
        char *decType;
        decType = parseDecTypes(); //the types are BOOL, INT, REAL, STRING... all should be stored in the types Table
        int tbleIndx = GTTsearchTble(decType);
        int tbleSize = GTTsizeBits(decType); //should return 8 but returns 4
        if(tbleIndx<0){
            //error
        }
        if (memLoc =="global"){
            //GSTaddToTble(tmpTokn,tmpLexm,)
        }else{
            offset += tbleSize;
            sprintf(numToStrn,"%d", offset);
            LSTaddToTble(tmpLexm, decType,numToStrn,"VAL",-1); //possible issue with longer integers offset must be char
        }
        matchLexTok(lexm,tokn,";");
        if(compLexTok(lexm,tokn,"IDENTIF")==0){ //issue with lexem not being incremeneted after all declarations
            i = 1;
        }
    }
    return offset;

    /*if(compLexTok(lexm,tokn, "IDENTIF")==1){
        return;
    }else{
        //this returns the number of unique identifiers we need to concern ourselves with
        if(!strcmp(tokn,"IDENTIF")){
            identifSize++;
            identifer[arryC]=lexm;
            arryC++;
        }
        getNextStrngArry(arryStrt);
        matchLexTok(lexm,tokn,":");
        matchLexTok(lexm,tokn,"INT");
        matchLexTok(lexm,tokn,";");
        parseDecList();
    }*/
}

char *parseDecTypes(){
    char *strnDecType;
    //CONSIDER ANOTHER METHOD MAYBE INT
    strnDecType = tokenType(tokn);
    if((!strcmp(strnDecType,"INT"))|| (!strcmp(strnDecType,"BOOL"))|| (!strcmp(strnDecType,"REAL")) || (strcmp(strnDecType,"STRING"))){
        strnDecType = tokenType(tokn);
    }else if(compLexTok(lexm,tokn,"IDENTIF")){
        strnDecType = lexem[arryStrt];
        int tblLoc = GSTsearchTble(strnDecType);
        if(tblLoc <0){
            //error
        }
    }else{
        //error
    }
    getNextStrngArry(arryStrt);
    return strnDecType;
}

void parseProc(){
    matchLexTok(lexm,tokn,"BEGIN");
    printBeginAsm();
    parseStatmntsLst();
    matchLexTok(lexm,tokn, "END");
    printEndAsm();
}

void parseStatmntsLst(){

    while(compLexTok(lexm,tokn,"END")==1){
        parseIndvStatmnt();
    }
    //Below is original PAL code for reference
    /*if(compLexTok(lexm,tokn,"END")==0){
        return;
    }else{
        parseIndvStatmnt();
        parseStatmntsLst();
    }*/
}

void parseIndvStatmnt(){

    char *lbl="";

    lbl = parseLbl(lbl);
    if(lbl != ""){
        //emitLbl(lbl);
    }
    parseExctblStats(); //not written yet

    //Below is original PAL code for reference
    /*if(compLexTok(lexm,tokn,"WRITELN")==0){
        parseWrtLne();
    }else if(compLexTok(lexm,tokn,"IDENTIF")==0){
        parseAssngmnt();
    }else{
        printf("Error in IndvStatement\n");
    }*/
}

char *parseLbl(char *lbl){ //possibly consider using a global variable for this file called retLbl?
    char *loclLbl ="";

    if(compLexTok(lexm,tokn,"#")==1){//double check might need to be 1
        return "";
    }
    getNextStrngArry(arryStrt);
    matchLexTok(tokn,lexm,"NUMERIC");//DO NOT INCREMEMT POINTER
    char *tmpTokn;
    char *tmpLexm;
    tmpTokn = token[arryStrt-1];
    tmpLexm = lexem[arryStrt-1];
    //THIS MIGHT BE ANOTHER AREA TO REFERENCE THE CONST PARSER... INCREMENTING POINTER GETS WEIRD...
    getNextStrngArry(arryStrt);
    int tbleLocl = LLTsearchTble(tmpLexm);
    if(tbleLocl>=0){
       loclLbl = LLTStrngRet(tbleLocl,2);
    } else{
        //loclLbl = getLabe();
        //LLTaddToTable("""""");
        //CODE IN THIS ELSE NEEDS TO BE FINISHED
    }
}

void parseExctblStats(){
    if((compLexTok(lexm,tokn,"READ")==0) || (compLexTok(lexm,tokn,"READLN")==0)){
        parseReads();
    }else if((compLexTok(lexm,tokn,"WRITE")==0) || (compLexTok(lexm,tokn,"WRITELN")==0)){
        parseWrites();
    }else if(compLexTok(lexm,tokn,"IDENITF")==0){

    }else if(compLexTok(lexm,tokn,"CALL")==0){

    }else if(compLexTok(lexm,tokn,"RETURN")==0){

    }else if(compLexTok(lexm,tokn,"GOTO")==0){

    }else if(compLexTok(lexm,tokn,";")==0){

    }else if(compLexTok(lexm,tokn,"DO")==0){

    }else if(compLexTok(lexm,tokn,"IF")==0){

    }else if(compLexTok(lexm,tokn,"WHILE")==0){

    }else if(compLexTok(lexm,tokn,"CASE")==0){

    }else if(compLexTok(lexm,tokn,"FOR")==0){

    }else if(compLexTok(lexm,tokn,"NEXT")==0){

    }else if(compLexTok(lexm,tokn,"BREAK")==0){

    }else{
        //error
    }
}

void parseReads(){
    int flag;
    if(compLexTok(lexm,tokn,"READ")){
        flag = 0;
    }else{
        flag = 1;
    }
    getNextStrngArry(arryStrt);
    parseArgs("READ");
    if(flag=1){
        //emitReadLine()
    }
    matchLexTok(lexm,tokn,";");

}

void parseArgs(char *args){
    if(compLexTok(tokn,lexm,"(")==1){
        return;
    }
    getNextStrngArry(arryStrt);
    //need a parameters variable
    char *parmtrs="";
    if((!strcmp(args,"READ") || (!strcmp(args,"WRITE")))){
        parmtrs == "";
    }else{
        //I need the parameters figured out PLType& read tables.h it's a VECTOR in C++ what can I replace that with?
    }
    parseArgsList(args, parmtrs);
    matchLexTok(lexm,tokn,")");
}

void parseArgsList(char *args, char *parmtrs){
    int i = 1;

    if((!strcmp(args,"READ")) || (!strcmp(args,"WRITE"))){
        while(i!=0){
            i = 0;

            if(args == "READ") {
                char *ident;
                char *retType = parseLocalVal(ident);
                //if(isNamedConstant(ident){ this is an error
                    //error
                //}
                //emitRead(retType);
            }
            else{
                char *retType = parseExprsn();
                //emitWrite(retType);
            }
            if(compLexTok(lexm,tokn,",")==0){
                getNextStrngArry(arryStrt);
                i = 1;
                getNextStrngArry(arryStrt);
            }
        }
    }else {//this is proce

    }
}



void parseWrites(){
    int flag = 1; //1 is up 0 is down

    if(compLexTok(lexm,tokn,"WRITE")==0){//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!+____________#############################
        flag=0;
    }
    getNextStrngArry(arryStrt);
    parseArgs("WRITE");
    if(flag == 1){
        //emiteWriteLn();
    }
    matchLexTok(lexm,tokn,";");
}

void parseWrtLne(){
    int strngLen;
    char strLeng[40];
    char *tmpLexm;
    tmpLexm=lexem[arryStrt];
    matchLexTok(lexm,tokn,"WRITELN");
    matchLexTok(lexm,tokn,"(");
    parseExprsn();
    //matchLexTok(lexm,tokn,"IDENTIF");
    matchLexTok(lexm,tokn,")");
    strngLen=strlen(tmpLexm);
    sprintf(strLeng,"%d",strLeng);
    printWritLn(strLeng,tmpLexm);
    matchLexTok(lexm,tokn,";");
}

void parseAssngmnt(){
    //char is getting moved forward because this is called again this temp must be unique
    char *tmpLexm;
    tmpLexm=lexem[arryStrt];
    matchLexTok(lexm,tokn,"IDENTIF");
    matchLexTok(lexm,tokn,":=");
    parseExprsn();
    printAssngment(tmpLexm);
    matchLexTok(lexm,tokn,";");
}

char *parseExprsn(){

    char *type = parseComprs();
    return type;

    /*parseTerms();
    parseAddExprsn();*/
}

char *parseComprs(){
    char *loclType = parseExprsn2(); //parseExprsn2 is paresSimpleExpression
    if(isComparsonOp(lexm)>0){  //(isMultiOp(lexm)>0){
        return loclType;
    }
}

char *parseExprsn2(){//this is SimpleExpression
    int addOp = 0;

    int flag = parseUnOps();
    char *loclType = parseTerms();
    if(flag = 1){
        //emitNeg(loclType);
        addOp=isAddOp(lexm);
    }while(addOp >0){
        int opType = isAddOp(lexm);
        char op;
        if(opType == 1){
            op = "+";
        }else if (opType ==2){
            op = "-";
        }
        getNextStrngArry(arryStrt);
        //LINE 1145-1148 NEED REVIEW
    }
    //while the currentValue is an ADDOP
//++++++++++++++++++++++++++++++++++++++++++PICK UP HERE+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

}

int parseUnOps(){ //checks to see if operator is a + or -
    int flag = 0;
    if(compLexTok(lexm,tokn,"+")==0){
        getNextStrngArry(arryStrt);
    }else if(compLexTok(lexm,tokn,"-")==0){
        flag = 1;
        getNextStrngArry(arryStrt);
    }else{
        ;
    }
    return flag;
}

void parseAddExprsn(){
    char *tmpLexm;
    tmpLexm=lexem[arryStrt];
    if(strcmp(tokn,"+") && (strcmp(tokn,"-"))){
        return;
    }else{
        getNextStrngArry(arryStrt);
        parseTerms();
        printAdditionLines(tmpLexm);
        parseAddExprsn();
    }
}

char *parseTerms(){
    char *loclType = parseFactor(); //this call needs to return a string
    //while the current tokenType is a mulop loop MULTI OPERATORS ARE FOUND in MAIN at 1380
    //THEY ARE {%, *, /, AND}
    //THE WHILE ISN'T RUN First time
    int opType = isMultiOp(lexm);
    while(opType>0){
        char *tmpLexm;
        char *tmpTokn;
        char *op = lexm;
        getNextStrngArry(arryStrt);
        char *btype = parseFactor();
        char *ctype = "10";
        //printMultiOps(op,ctype);
    }
    return loclType; //returns to parseExpression2
    //parseAddTerms();
}

void parseAddTerms(){
    char *tmpLexm;
    tmpLexm=lexem[arryStrt];
    if(strcmp(tokn,"*") && (strcmp(tokn,"/"))){
        return;
    }else{
        getNextStrngArry(arryStrt);
        parseTerms();
        printMultiLines(tmpLexm);
        parseAddTerms();
    }
}
//ERROR IS OCCURING IN HERE============================================================================================
char *parseFactor(){

    char *localIdent="";
    char *tmpLexm="";

    tmpLexm=lexem[arryStrt];
    printf("The current lexem is %s\n",tmpLexm);
    if(compLexTok(lexm,tokn,"IDENTIF")==0) {
        //Search the local tables
        int varLoc = LSTsearchTble(lexm);
        if (varLoc >=0) {
            char *localType = parseLocalVal(localIdent);
            //if(localType ==) is an ATOMIC TYPE
            //emitRval(LocalType)
            //}else{
            //emitRval("STRUCT");
            //}
            return localType;
        }
        //Search the global table
        varLoc = GSTsearchTble(lexm);
        if (varLoc >= 0) {
            char *localType = parseLocalVal(localIdent);
            //if(localType ==) is an ATOMIC TYPE
            //emitRval(LocalType)
            //}else{
            //emitRval("STRUCT");
            //}
            return localType;
        }
        //Search the procedure table
        varLoc = GPTsearchTble(lexm);
        if (varLoc >= 0) {
            char *localType = parseFuncs(varLoc);//need to write
            return localType;
        }


        if (compLexTok(lexm, tokn, "IDENTIF") == 0) {
            printIdentifs(tmpLexm);
            getNextStrngArry(arryStrt);
        } else if (compLexTok(lexm, tokn, "NUMERIC") == 0) {
            printNums(tmpLexm);
            getNextStrngArry(arryStrt);
        } else {
            matchLexTok(lexm, tokn, "(");
            parseExprsn();
            matchLexTok(lexm, tokn, ")");
        }
        //error
    }else if (compLexTok(lexm,tokn,"(")==0){
        getNextStrngArry(arryStrt);
        char *exprType = parseExprsn();
        matchLexTok(lexm,tokn,")");
        return exprType;
    } else if(compLexTok(lexm,tokn,"NOT")==1){
        getNextStrngArry(arryStrt);
        char *factType = parseFactor();
        //emitNOT assembly;
        return factType;
    }else{
        char *tmpLabel;
        char *tmpType;
        char *tmpVal;
        parseConstnts();//LEFT OFF HERE=============================================================================================
        int loctn = GLTsearchTble(typeToAdd);//,valToAdd readd?
    }
}

char *parseFuncs(char *locs){
    int i = 0;
    return "Hey";
}

char *parseLocalVal(char *localIdent){
    char *tmpLexm;
    char *tmpTokn;
    matchLexTok(lexm,tokn,"IDENTIF");//DO NOT INCREMENT POINTER;
    tmpLexm=lexem[arryStrt-1];
    char *ltype;
    char *addr;
    int ptr;

    VarbleInfo(tmpLexm,"",addr,0);
    if (ltype==""){
        //error
    }
    //emitLval(addr,ptr);
    char *qualifType = parseQuals(ltype);
    return qualifType;
}



char *parseQuals(char *ltype){
    char *qualif = ltype;
    int i = 1;


    if((compLexTok(lexm,tokn,"[")|| compLexTok(lexm,tokn,"."))){
        i = 1;
    }
    while(i!=0){
        i =0;
        int local = GSTsearchTble(lexm); //FIRST PACE lexm should be STRING MIGHT NEED TO LOOK AT TYPE MORE

    }
    return qualif;

}
//=====================================================================================================================
//=====================================Function to ignore tabs=========================================================
void skipTabs(char c){
    do{
        c=fgetc(filePntr);
    }while (c=='\t');
    fseek(filePntr,-1,SEEK_CUR);
}
//=====================================================================================================================
//====================================Function to test if file is empty================================================
int isEmpty(FILE *fp){
    if(ftell(fp)==0){
        return 1; //1 is error
    }else{
        return 0; //0 is correct
    }
}
//=====================================================================================================================
//=====================================================================================================================
//=====================================Function compares the TOKEN and LEXEME==========================================
//--------This function checks to token against a passed pointer to an array. Since I create a .txt file initially I--
//--------have already handled the lexeme. The *lex variable is passed but currently unused as it doesn't seem--------
//--------necessary. This function will increment the array to the next string in the list.---------------------------
void matchLexTok(char *lex, char *tok, char *valComp){
    if(!strcmp(tok,valComp)){
        getNextStrngArry(arryStrt);
    }else{
        printf("Error in MustMatch %s & %s\n",tokn, lexm);
        exit;
    }
}
//=====================================================================================================================
//=====================================Function checks to see if TOKEN and LEXEME match================================
//--------This function seems un-ncessary. Consider how I can modify the matchLexTok. This returns a 1 for false, and--
//--------0 for true.
int compLexTok(char *lex, char *tok, char *valComp){
    if(!strcmp(tok,valComp)){
        return (0);
    }else{
        return (1);
    }
}
//=====================================================================================================================
//=====================================Function gets the next string===================================================
void getNextStrngArry(int curntArr){
    tokn = token[curntArr+1];
    lexm = lexem[curntArr + 1];
    arryStrt = curntArr + 1;
}//===================================The below functions communicate with the tables.c===============================
int searchVarblTble(char *val, int tble){ //pass the lexeme and search the tables
    int isGlobal;
    isGlobal =GSTsearchTble(val);
    if(isGlobal!=NULL){
        isGlobal =GSTsearchTble(val);
        return isGlobal;
    }else{
        return isGlobal;
    }
}
//====================================================================================================================
int allgnMent(int off){
    int intl = off /16;
    int rem = off % 16;

    if(rem>0){
        intl ++;
    }
    int fnlSize = 16* intl;
    return(fnlSize);

};
//==============================IS LEXEM AN ADD OR OPP TYPE===========================================================
//THEY ARE {%, *, /, AND} are OP types
//IF the return of this function is greater than 0 it is an ADD OP. 1 indicates + 2 indicates -
int isAddOp(char *ops){
    if(!strcmp(ops,"+")){
        return 1;
    }else if(!strcmp(ops,"-")){
        return 2;
    }else{
        return 0;
    }
}

//if the return of this funciton is greater than 0 then it is MULTIOP. 1 indicates * 2 indicates / 3 indicates %
//4 indicates AND
int isMultiOp(char *ops){
    if(!strcmp(ops,"*")){
        return 1;
    } else if(strcmp(ops,"/")){
        return 2;
    }else if(strcmp(ops,"%")){
        return 3;
    }else if(!strcmp(ops,"AND")){
        return 4;
    }
}

int isComparsonOp(char *op){
    int count =0;
    int i = 0;
    for(i = 0; i<6;i++){ //why unreachble?
        if(!strcmp(op,TwoDimOp[i])){
            count++;
        }
        return count;
    }
}

void VarbleInfo(char *lclIdent, char* lclType, char *addr, char *ptr){
    int tableLoc;
    tableLoc=LSTsearchTble(lclIdent);
    if(tableLoc>=0){
        //stuff
    }
    tableLoc=GSTsearchTble(lclIdent);
    if(tableLoc>=0){
        int key = GSTsearchTble(lclIdent);
        lclTy = GSTStrngRet(key,2);
        strcat(lcladdr,lclIdent);//errors here when concatenating _ and PROMPT1 into _PROMPT1 LEFT OFF HERE 4/29/18
        strcat(lcladdr,"_");
        pntr=0;
        return;
    }
    lclTy="";
    lcladdr[LIMIT]="";
    pntr=0;
    return;
}