
#include "token.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "scanner.h"
#include "parser.h"
#include "assmblr.h"


int main(int argc, char *argv[]) {
    FILE *filePtr;
    /*---------Begin process cmd-line args and Redirection--------*/

    switch (argc) {
        case 1: // No parameters, use stdin
            // printf("NO argument provided\n");
            filePtr = stdin;
            break;

        case 2: // One parameter, use .lan file supplied
            if ((filePtr = fopen(strcat(argv[1], ".klump"), "r")) == NULL) {
                printf("Cannot open input file.\n");
                exit(1);
            }
            break;

        default:
            printf("Unable to open file .klump extension is implicit.\n");
            exit(0);
    }

    /*---------End process cmd-line args and Redirection----------*/

    /*---------Begin check if file empty-----------*/

    fseek(filePtr, 0, SEEK_END);
    if (ftell(filePtr) == 0) {
        printf("File is empty.\n");
        exit(1);
    } else {
        rewind(filePtr);
    }

    /*---------/End check if file empty-------------*/

    /*---------Begin check for invalid characters and max length-------------*/

    char c;
    int numLine = 1;

    int charCount = 0;
    char tempStr[MAX]; // ! Remember: C doesn't do array out-of-bound checking!
    int i;

    int isValid = 1; // true
    while ((c = fgetc(filePtr)) != EOF) {
        if (c == '\n')
            numLine++;

        // Exclude comment line starting with '//' and ending with new line
        if (c == '/') {
            if (fgetc(filePtr) == '/') {
                while ((c = fgetc(filePtr)) != '\n') {}
                numLine++;
            }
        }

        if (isalnum(c)) {
            tempStr[charCount] = c; // array out-of-bound checking not happen here
            charCount++;
            if (charCount > MAX) {
                printf("Word '%s' on line number %d is too long. \n", tempStr, numLine);
                exit(1);
            }
        } else if (isspace(c) || isExAcceptableChar(c)) {
            charCount = 0;
        } else {
            printf("Invalid character '%c' at line %d. \n", c, numLine);
            isValid = 0; // false
        }

    }

    if (isValid == 0) {
        printf("Something wrong with the input file. \n");
        exit(1);
    }

    rewind(filePtr);

    /*---------/End check for invalid characters and max length-------------*/

    /*---------Begin Job-------------*/
    // At this time, the file is good. Now let scanner.c do the work
    TokenType tokenType = UNDEF;

    while ((tokenType = getTokenType(filePtr,1)) != EOT) {

        getTokenType();
    }
    /*---------/End Job-------------*/
    fclose(filePtr);
    createNewAsmFle("assmblr.asm");

    //fclose(filePtr);
    startParser();
    return 0;
}