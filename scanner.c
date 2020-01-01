//
// Created by jrinder on 1/3/18.
//

#include <stdio.h>
#include <strings.h>
#include <ctype.h>
#include <memory.h>
#include <stdlib.h>

#include "token.h"
#include "scanner.h"
#include "systemdefs.h"


//===================================0 indicates scanner, 1 indicates parser===========================================
TokenType getTokenType(FILE *filePntr, int scanrParser) {
    int lneNum = 1;
    char chr;
    char lookAheadChr;
    char tmpHldr;
    FILE *fpScn;

    //=============destroys the .scan file. Consider putting this in a function as the .asm does this too.=============
    fpScn= fopen("scnr.scan","w");
    fclose(fpScn);
    //=================================================================================================================
//-------------------------------------Loop through Chars and scan to determine types----------------------------------
    restart:
    while((chr=fgetc(filePntr))!=EOF){
        //-----------------------------Check to see if end of line if so increment next line---------------------------
        if (testChar(chr,'\n')==0|| testChar(chr,NULL)==0){
            lneNum++;
        }

        //-----------------------------Check to see if a comment has been made-----------------------------------------
        //=============================COMMENT CODE====================================================================
        if(chr=='{'){ //if a comment is being made we need to find the end of the comment
            int endCmt = 1; //1 indicates false

            while (endCmt==1){
                if(lookAheadChr=getNxtPntdVal(filePntr)=='}'){
                    endCmt=0;//This will end the loop though I don't think it's technically necessary... left in as it
                    //it seems wise.

                    goto restart;//once we reach the last '}' we can go back to the top of the loop and get the next
                    //char. This lets us ignore the text in the bracket and the ending bracket. We don't rewind the
                    //pointer so that we can start at the next char after '}

                }else{
                    endCmt==1;
                }
            }
        //=============================================================================================================


            //Scratch goes between

            //Scratch goes between
        }
        if((testChar(chr,'/'))==0){
            if(getNxtPntdVal(filePntr)=='/'){
                while(getNxtPntdVal(filePntr)!='\n'){
                    lneNum++;
                }
            }else{
                fseek(filePntr,-1,SEEK_CUR);
            }
        }
        //-----------------------------Determine what char is alpha, num, etc------------------------------------------
        int chrType = 0;

        chrType = charType(chr);

        if (chrType==1){
            chr=mkeUprCse(chr);
            build2dArry(words,intlArryi,intlArryj, chr, filePntr, chrType); //builds the 2d array (forming words)
            wordLineNums[intlArryi] = lneNum;
            if(isKeyWord(words[intlArryi])==0){
                token[tokeni]=words[intlArryi];
                tokeni++;
            }else{
                token[tokeni]="IDENTIF";
                tokeni++;
            }
            //printf("The Word is %s",words[wordi]);
            intlArryi++;intlArryj = 0;
            fseek(filePntr, -1, SEEK_CUR);
        }else if(chrType==2){
            //re-think this. There has to be a more effective way. But this does work
            tmpHldr = build2dArryNum(nums,intlArryi,intlArryj,chr,filePntr);
            //need to check if char after number is a period and if so treat it as decimal.
            if(tmpHldr=='N') {
                token[tokeni] = "NUMERIC";
            }else{
                token[tokeni] = "DECIMAL";
            }
            tokeni++;
            wordLineNums[intlArryi] = lneNum;
            intlArryi++;intlArryj = 0;
            fseek(filePntr, -1, SEEK_CUR);
        }else if(chrType==3){
            build2dArryOps(words,intlArryi,intlArryj,chr,filePntr);
            token[tokeni]=words[intlArryi];
            tokeni++;
            intlArryi++;intlArryj = 0;
            fseek(filePntr, -1, SEEK_CUR);
        }else if(chrType==4){
            buildString(words,intlArryi,intlArryj,chr,filePntr,chrType,'\'');
            token[tokeni]="CSTRING";
            tokeni++;
            intlArryi++;intlArryj=0;
        }
    }
    printTokens();
//in order to create an array in C memory allocaiton is needed. to simplify this I should need no more than the total
// lines multiplied by 2. This will be the size of my array
    counts = counts*2;

    return EOT;
}
//=====================================================================================================================
//-------------------Functions below support the above function--------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
//===================This function will eventually write this data to an output file===================================
void printTokens(){
    int i;
    for(i = 0; i<tokeni; i++){
        if(!strcmp(token[i],words[i])){
            writeLnes(token[i],words[i]);
            //printf("%s\t\t%s\n",token[i],words[i]);
        }else if(!strcmp(token[i],"NUMERIC")){
            writeLnes(token[i],nums[i]);
            //printf("%s\t\t%s\n",token[i],nums[i]);
        }else if(!strcmp(token[i],"DECIMAL")){
            writeLnes(token[i],nums[i]);
        }else{
            writeLnes(token[i],words[i]);
        }
    }
}
//=====================================================================================================================
//*********************************************************************************************************************
//====================This function tests to see if the string is a keyword============================================
int isKeyWord(char *strn){
    int i;
    int results = 1; //1 is false if not a keyWord then 1 otherwise 0

    for(i = 0; i<42;i++){
        if(!strcmp(keywords[i],strn)) {
            results = 0;
        }
    }
    return results;
}
int is2dOperator(char *strn){
    int i;
    int result = 1; //0 indicates a 2d operator. 1 indicates 1 d operator

    for(i=0; i<4; i++){
        if(!strcmp(TwoDimOps[i],strn)){
            result=0;
        }
    }
    return result;
}
int isPunct(char c){
    int i;
    int resulst=1;

    for(i = 0 ; i<10;i++){
        if(c == punct[i]){
            resulst=0;
        }
    }
    return resulst;

}
//=====================================================================================================================
//*********************************************************************************************************************
//=====================this function determines if the string is a constant like a string of text chars/numbers========
int isConstant(char *strn){

}
//=====================================================================================================================
//*********************************************************************************************************************
//=====================This function determines if the char is an operator possibly break down to dif types ops========
int isOperator(char c){
    int i;
    int results = 1;

    for(i = 0; i< 6;i++){
        if(c == otherOperators[i]){
            results = 0;
        }
    }
    return results;
}
//=====================================================================================================================
//*********************************************************************************************************************
//====================This function will write data to a file may not need=============================================
void writeLnes(char * txt1, char * txt2){
    FILE * flPntr;
    char stngHldr[LIMIT]="";
    strcat(stngHldr,txt1);
    strcat(stngHldr, "\t\t\t");
    strcat(stngHldr,txt2);
    strcat(stngHldr,"\n");
    flPntr = fopen("scnr.scan","a");
    fputs(stngHldr,flPntr);
    fclose(flPntr);
    counts= counts+1;
}
//=====================================================================================================================
//*********************************************************************************************************************
//=====================this function builds a 2d array of continuous chars (ints included)=============================
void build2dArry(char arry[LIMIT][MAX],int itemi, int itemj, char c, FILE * fPtr, int charsType) {
    char prevChar;
    char nextChar;
    int cont = 0;
    long offset;

    offset = ftell(fPtr);

    prevChar = c;

    arry[itemi][itemj++] = c;

    //prevChar = rewnd(c,fPtr);
    //c = fgetc(fPtr);

    //What is the logic I need to ensure consts are stored with chars or numbers?
    //1.The previous char must be char
    //2.The next char mut NOT be a '\n'
    //Outcome: In order to ensure ; : ) don't get caught up in this array I need to test the previous char to make sure
    //it's a char (type 1) and that it's not a punctuation.

    while ((charType(c = fgetc(fPtr))==charsType || charType(prevChar)==1) && (isPunct(c)==1) && (isOperator(c)==1) && c !='\n' && c!=32 && c!='\t'){ //&& c != '\t'){ //||charType(c = fgetc(fPtr))==2
        //prevChar = fgetc(fPtr);
        arry[itemi][itemj++]= mkeUprCse(c);
        //arry[itemi][itemj++]= mkeUprCse(prevChar);
        prevChar = c;
        offset = ftell(fPtr);
        fseek(fPtr,offset,SEEK_SET);


        //c = fgetc(fPtr);
    }
    arry[itemi][itemj] = '\0';
}
//=====================This function builds 2d array of numbers added back to resolve decimal issue==================
char build2dArryNum(char arry[LIMIT][MAX],int itemi, int itemj, char c, FILE * fPtr) {
    char retVar='N';
    arry[itemi][itemj++] = c;
    while (charType(c = fgetc(fPtr))==2|| c=='.'){ //issue looping through chars. Can't make it based on char type as this
        //will result in skipping decimals
        if(c=='.') {
            retVar = 'D';
        }
        arry[itemi][itemj++]= c;
    }
    arry[itemi][itemj] = '\0';
    return retVar;
}
//=====================this function builds a CSTRING possible to use build2dArray instead?===========================
void buildString(char arry[LIMIT][MAX],int itemi, int itemj, char c, FILE * fPtr, int charsType, char endr) {
    c=mkeUprCse(c);
    arry[itemi][itemj++]=c;
    int endStrn=1;
    while(endStrn==1){
        if(c=fgetc(fPtr)=='\''){
            endStrn=0;
            fseek(fPtr, -1, SEEK_CUR);
            c=fgetc(fPtr);
            c=mkeUprCse(c);
            arry[itemi][itemj++]=c;
        }else{
            fseek(fPtr, -1, SEEK_CUR);
            c=fgetc(fPtr);
            c=mkeUprCse(c);
            arry[itemi][itemj++]=c;
        }
    }
    arry[itemi][itemj]='\0';
}
/*============================THE FUNCTIONS BETWEEN THIS ARE NOT NECESSARY REFERENCE ONLY=============================

*/
void build2dArryOps(char arry[LIMIT][MAX],int itemi, int itemj, char c, FILE * fPtr) {
    arry[itemi][itemj++] = c;
    int count = 0;
    int size = 1;
    int twDpunc=1; //0 true 1 false;
    char oldChr=c;

    //First determine if this char is on the list of chars that could be 2dim
    size = sizeOfPunctArry(c);

    while (charType(c = fgetc(fPtr))==3 && (twDpunc = isPunctPartOf2D(c, oldChr))==0){

        twDpunc = isPunctPartOf2D(c,oldChr);

            arry[itemi][itemj++] = c;
            //-----TEST TO SEE IF VALID 2D array----
            count++;
    }
    arry[itemi][itemj] = '\0';
}

int sizeOfPunctArry(char c){
    for(int i=0;i<4;i++){
        if(c==indvTwoDimOps[i]){
            return 2;
        }else {
            return 1;
        }
    }
}

int isPunctPartOf2D(char c, char c2) {
    int results =1;
    char strn[LIMIT][MAX];
    int x=0,y=0;
    strn[x][y]="";
    strn[x][y++]="";

    for (int i = 0; i < 4; i++) {
        if (c == indvTwoDimOps[i]) {
            strn[0][0]=c2;
            strn[0][1]=c;
            results=is2dOperator(strn[x]);
        }

    }
    return results;
}
//=====================================================================================================================
//=====================================================================================================================
//*********************************************************************************************************************
//=====================Gets value from pointer in file consider removing===============================================
char getNxtPntdVal(FILE *fPntr) {
    char holder;
    holder = fgetc(fPntr);
    return holder;
}
//=====================================================================================================================
//*********************************************************************************************************************
//=====================Test 2 char's against one another tests end of line comments, etc===============================
int testChar(char c, char c2) {
    if (c==c2){
        return 0;
    }else{
        return 1;
    }
}
//=====================================================================================================================
//*********************************************************************************************************************
//=====================Makes a char upper case possibly remove as this may be done elsewhere===========================
char mkeUprCse(char c) {
    c = putchar(toupper(c));
    return c;
}
//=====================================================================================================================
//*********************************************************************************************************************
//=====================Determines what the char type is. ispunct will require deeper dive==============================
int charType(char c) {
    int typ = 0;
    if(isalpha(c)){
        typ = 1;
    }else if(isdigit(c)){
        typ = 2;
    }else if(ispunct(c) && c!='\''){
        typ = 3;
    }else if(c=='\''){
        typ = 4; //4 indicates a CSTRING. We will need to find a way to use the 2d array builder to build a CSTRING
    }else if(isspace(c) && isalpha(c-1)){
        typ = 1;
    }
    return (typ);
}
//=====================================================================================================================
//*********************************************************************************************************************
//=====================================================================================================================
int isExAcceptableChar(char c) {
    if (c == '.' || c == '(' || c == ')' || c == ',' || c =='{' || c == '}' ||
        c == ';' || c == '[' || c == ']' ||
        c == ':' || c == '+' || c == '-' || c == '*' || c == '/' || c == '%' ||
        c == '=' || c == '<' || c == '>' || c == '!' || c == '"' || c == '#' || c=='\''
        /* || c == '#' */ ) {

        return 1;
    } else
        return 0;
}

//===========================================Function to rewind pointer to a previous char=============================
char rewnd(char curnt, FILE *fp){
    char c;
    c = getc(fp);
    //ungetc()
}

