#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

void remove_leading_and_trailing_spaces(char* source_file_name, char* destenation_file_name) {
    // checks for correct parameters and if file opening was successful
    FILE *sourceFile = fopen(source_file_name, "rb");
    if (sourceFile == NULL) {
        perror("Error opening file ");
        return;
    }

    FILE *outputFile = fopen(destenation_file_name, "wb");
    if (outputFile == NULL) {
        perror("Error opening output file ");
        fclose(sourceFile);
        return;
    }
    // declare variabels
    int readChar = 0; // character read by fgetc
    int jmpBack = 0;  // amount of characters to jump back (in the outputfile) at the end of a lien
    bool charReached = false; // 1 if a non tab/space character has been encounterd in a new line/amount of chars since a non space/tab char

    while ((readChar = fgetc(sourceFile)) != EOF) {
        if((char)readChar == ' ' || (char)readChar == '\t') {
            jmpBack--; //a space/tab has been encounterd, decrement
        }
        if ((char)readChar != ' ' && (char)readChar != '\t') {
            charReached = 1; // a non tab/space char has been encounterd
            if((char)readChar != '\n') {jmpBack = 0;} // resets jumpback
        }
        if ((char)readChar == '\n') {
            charReached = 0;                    // reset if a non sp ace/tab char has been encounterd because its a new lien
            fseek(outputFile, jmpBack, SEEK_CUR); // jumo back in the output file
            printf("%djmp\n", jmpBack); // debug
            fputc('\n', outputFile); // put an newline in outputfile
        }
        if (charReached == 1) {
            fputc(readChar, outputFile);  // if character has been encounterd, add read char to outputfile
        }
    }
    

    fclose(outputFile);
    fclose(sourceFile);
}

void remove_comments(char* source_file_name, char* destenation_file_name) {
    FILE *source_file = fopen(source_file_name, "rb");
    if (source_file == NULL) {
        perror("Error opening file ");
        return;
    }

    FILE *outputFile = fopen(destenation_file_name, "wb");
    if (outputFile == NULL) {
        perror("Error opening output file ");
        fclose(source_file);
        return;
    }

    int readChar = 0;
    bool singleLineComment = 0;
    bool multiLineComment = 0;

    while((readChar = fgetc(source_file)) != EOF) {
        switch((char)readChar) {
            case '\n':
                singleLineComment = 0;
                if(getc(source_file) == '\n') {
                    fseek(source_file, -1, SEEK_CUR);
                    continue;
                }
                fseek(source_file, -1, SEEK_CUR);
                break;
            case '/':
                int tmpChr = fgetc(source_file);
                if(tmpChr == '/') {
                    singleLineComment = 1;
                    continue;
                }
                if(tmpChr == '*') {
                    multiLineComment = 1;
                    continue;
                }
                fseek(source_file, -1, SEEK_CUR);
                break;
            case '*':
                if(fgetc(source_file) == '/') {
                    multiLineComment = 0;
                    continue;
                }
                fseek(source_file, -1, SEEK_CUR);
                break;
        }
        
        if(!multiLineComment && !singleLineComment) fputc(readChar, outputFile);
    }

    fclose(outputFile);
    fclose(source_file);
}

#endif