#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

void remove_leading_whitespaces(char* source_file_name, char* destenation_file_name) {
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

    int readChar = 0;
    int jmpBack = 0;
    bool charReached = false;

    while ((readChar = fgetc(sourceFile)) != EOF) {
        if((char)readChar == ' ' || (char)readChar == '\t') {
            jmpBack--;
        }
        if ((char)readChar != ' ' && (char)readChar != '\t') {
            charReached = 1;
            if((char)readChar != '\n') {jmpBack = 0;}
        }
        if ((char)readChar == '\n') {
            charReached = 0;
            fseek(outputFile, jmpBack, SEEK_CUR);
            printf("%djmp\n", jmpBack);
            fputc('\n', outputFile);
        }
        if (charReached == 1) {
            fputc(readChar, outputFile);
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