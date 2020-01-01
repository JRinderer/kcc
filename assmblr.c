//
// Created by jrinder on 2/20/18.
//

//#include <bits/types/FILE.h>
#include <stdio.h>
#include <memory.h>
#include "assmblr.h"
#include "token.h"
#include "parser.h"

FILE *filePntr;

//============Define constants that will not change throughout the code. These are standard assemlby pieces.===========

//=====================================================================================================================
//===============Since I'm working with file I/O I have to create a .asm file to append code to.=======================
void createNewAsmFle(char *path){

    filePntr= fopen(path,"w");
    fclose(filePntr);
}
//=====================================================================================================================
//write data to asm
/*void writeLnesAsm(char * txt1, char * txt2){
    FILE * flPntr;
    char stngHldr[LIMIT]="";
    strcat(stngHldr,txt1);
    strcat(stngHldr, "\t\t\t");
    strcat(stngHldr,txt2);
    strcat(stngHldr,"\n");
    flPntr = fopen("scnr.scan","a");
    fputs(stngHldr,flPntr);
    fclose(flPntr);
}*/

//=======================================generates a newLine \n========================================================
void nullLine(){
    FILE *fp;
    fp = fopen("assmblr.asm","a");
    fputs(" \n",fp);
    fclose(fp);
}
//=====================================================================================================================
//===========================This will generate a line of assemlby code================================================
void printLines(char *lbl, char *opCode, char *var, char *cmnts){
    FILE *fp;

    fp = fopen("assmblr.asm","a");

    char strnHlder[LIMIT]="";
    strcat(strnHlder,lbl);
    strcat(strnHlder,opCode);
    strcat(strnHlder,var);
    strcat(strnHlder,cmnts);
    strcat(strnHlder,"\n");
    fputs(strnHlder,fp);
    fclose(fp);
}
//==========================Print the beginning section for ASM file===================================================
void printBeginAsm(){
    nullLine();
    printLines(";TEXT SECTION","","","");
    nullLine();
    printLines("","global main","","");
    nullLine();
    printLines("\t\t\t","section","\t\t\t.txt","");
    nullLine();
    printLines("\t\t\t","extern","\t\t\tprintf","");
    //printLines("\t\t\t","global","\t\t\t_start","");
    nullLine();
    printLines("main:","\t\tnop","\t\t\t\t\t\t","\t\t\t\t;main PROC");
    printLines("\t\t\t","push","\t\t\trbp","");
    printLines("\t\t\t","mov","\t\t\t\trbp,rsp","");

}

void printNums(char *num){
    printLines("\t\t\t","push\t\t\t",num,"\t\t\t\t\t\t;emit numbers");

}

void printIdentifs(char *ident){
    char strnHldr[LIMIT]="";
    strcat(strnHldr,"qword ");
    strcat(strnHldr,"[");
    strcat(strnHldr,ident);
    strcat(strnHldr,"]");

    printLines("\t\t\t","push\t\t\t",strnHldr,"\t\t\t\t;print identifiers");
}

void printAssngment(char *assng){
    char strnHldr[LIMIT]="";
    strcat(strnHldr,"qword ");
    strcat(strnHldr,"[");
    strcat(strnHldr,assng);
    strcat(strnHldr,"]");

    printLines("\t\t\t","pop\t\t\t\t",strnHldr,"\t\t\t\t;print assignments");
}

void printAdditionLines(char *ops){
    printLines("\t\t\t","pop\t\t\t\t","rbx","\t\t\t\t\t\t;print addition ops");
    printLines("\t\t\t","pop\t\t\t\t","rax","");
    //if(strcmp(tokn,"+")
    if(!strcmp(ops,"+")){
        printLines("\t\t\t","add\t\t\t\t","rax,rbx","\t\t\t\t\t;adds the contents of rax and rbx");
    }else{
        printLines("\t\t\t","sub\t\t\t\t","rax,rbx","\t\t\t\t\t;subtracts the contents of rax and rbx");
    }
    printLines("\t\t\t","push\t\t\t","rax","");
}

void printMultiLines(char *ops){
    printLines("\t\t\t","pop\t\t\t\t","rbx","\t\t\t\t\t\t;print multiplcation/division");
    printLines("\t\t\t","pop\t\t\t\t","rax","");
    printLines("\t\t\t","mov\t\t\t\t","rdx,0","\t\t\t\t\t;move a 0 into rdx");
    if(!strcmp(ops,"*")){
        printLines("\t\t\t","imul\t\t\t","rax,rbx","\t\t\t\t\t;multiply the two register");
        printLines("\t\t\t","push\t\t\t","rax","");
    }else if(!strcmp(ops,"/")){
        printLines("\t\t\t","idiv\t\t\t\t","rbx","\t\t\t\t\t;multiply the two register");
        printLines("\t\t\t","push\t\t\t\t","rax","");
    }
}

void printWritLn(char *strngLngth, char *strn){
//====================Intially planed on using strictly assembly possible reuse for KLUMP==============================
/*    printLines("\t\t\t","mov\t\t\t\t","rax,1","");
    printLines("\t\t\t","mov\t\t\t\t","rdi,1","");
    printLines("\t\t\t","mov\t\t\t\trsi,",strn,"");
    printLines("\t\t\t","mov\t\t\t\trdx,",strngLngth,"");
    printLines("\t\t\t","syscall","","");
    printLines("\t\t\t","mov\t\t\t\trax",",60","");
    printLines("\t\t\t","xor\t\t\t\trdi,", "rdi","");*/
//=====================================================================================================================
    printLines("\t\t\t","mov\t\t\t\t","rdi,_INT_","");
    printLines("\t\t\t","pop\t\t\t\t","rsi","");
    printLines("\t\t\t","mov\t\t\t\t","rax,0","");
    printLines("\t\t\t","call\t\t\t","printf","");
    printLines("\t\t\t","mov\t\t\t\t","rdi,_STR_","");
    printLines("\t\t\t","mov\t\t\t\t","rsi,_CRLF_","");
    printLines("\t\t\t","mov\t\t\t\t","rax,0","");
    printLines("\t\t\t","call\t\t\t","printf","");
}

void printEndAsm(){
    printLines("\t\t\t","mov\t\t\t\t","rsp,rbp","");
    printLines("\t\t\t","pop\t\t\t\t","rbp","");
    printLines("\t\t\t","mov\t\t\t\t","rax,0","");
    printLines("\t\t\t","ret","","");
    nullLine();
    printLines(";BSS Section","","","");
    nullLine();
    printLines("\t\t\t","section\t\t\t\t",".bss","");
    nullLine();
    //
    for(int i=0; i<identifSize;i++){
        printLines(strcat(identifer[i],":"),"\t\t\t","resq\t\t\t","1");
    }
    //
    nullLine();
    printLines(";DATA Section","","","");
    nullLine();
    printLines("\t\t\t","section\t\t\t\t",".data","");
    printLines("_INT_    :\t","db\t\t\t\t\t","\"%i\",0x0","");
    printLines("_STR_    :\t","db\t\t\t\t\t","\"%s\",0x0","");
    printLines("_CRLF_   :\t","db\t\t\t\t\t","0xa,0x0","");
    nullLine();

}

void printProcdrPro(char *procName, char *offset){
    char *lbl;
    char *prcEnd;
    if(!strcmp(procName,"MAIN")){
        lbl="main";
    }else {
        strcat(lbl, "_ENTRY_");
        strcat(lbl, procName);
        strcat(lbl, "_");
    }
    //strcat(prcEnd,procName);
    printLines(lbl,":","","");
    printLines("\t\t\t","push\t\t\t\t","FRAME","");
    printLines("\t\t\t","move\t\t\t\t","FRAME, STACK","");
    printLines("\t\t\t","sub\t\t\t\t\t","STACK,",offset);


}