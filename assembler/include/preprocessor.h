#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

void remove_trailing_whitespaces(char* source_file_name, char* destenation_file_name) {
    FILE *source_file = fopen(source_file_name, "rb");
    if (source_file == NULL) {
        perror("Error opening file ");
        return;
    }

    FILE *output_file = fopen(destenation_file_name, "wb");
    if (output_file == NULL) {
        perror("Error opening output file ");
        fclose(source_file);
        return;
    }

    int readChar = 0;
    bool charReached = false;
    int jumpBack = 0;
    int jumpBackFromBeginning = 0;
    

    while((readChar = fgetc(source_file)) != EOF) {
        if((char)readChar != ' ' && (char)readChar != '\t') {jumpBack = 0;}
        else jumpBack--;
        
        if((char)readChar == '\n') {
            fseek(output_file, jumpBack, SEEK_CUR);
            charReached = false;
        }

        if(!charReached && (char)readChar != ' ' && (char)readChar != '\t') {
            charReached = true;
        }
        if(charReached) fputc(readChar, output_file);
    }

    fclose(output_file);
    fclose(source_file);
}

void remove_comments(char* source_file_name, char* destenation_file_name) {
    FILE *source_file = fopen(source_file_name, "rb");
    if (source_file == NULL) {
        perror("Error opening file ");
        return;
    }

    FILE *output_file = fopen(destenation_file_name, "wb");
    if (output_file == NULL) {
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
        
        if(!multiLineComment && !singleLineComment) fputc(readChar, output_file);
    }

    fclose(output_file);
    fclose(source_file);
}

#endif