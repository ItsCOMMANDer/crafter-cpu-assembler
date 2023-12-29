#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>



void preprocess(char* source_file_name, char* destenation_file_name) {
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
    int charCount = 0;
    bool singleLineComment = false;
    bool multiLineComment = false;
    bool charEncounterd = false;
    while((readChar = fgetc(source_file)) != EOF) {
        if((char)readChar != ' ' && (char)readChar != '\t') {charEncounterd = true;}
        switch((char)readChar) {
            case '\n':
                singleLineComment = false;
                if(getc(source_file) == '\n') {
                    fseek(source_file, -1, SEEK_CUR);
                    continue;
                }
                fseek(source_file, -1, SEEK_CUR);
                charCount = 0;
                break;
            case '/':
                int tmpChr = fgetc(source_file);
                if(tmpChr == '/') {
                    singleLineComment = true;
                    continue;
                }
                if(tmpChr == '*') {
                    multiLineComment = true;
                    continue;
                }
                fseek(source_file, -1, SEEK_CUR);
                break;
            case '*':
                if(fgetc(source_file) == '/') {
                    multiLineComment = false;
                    continue;
                }
                fseek(source_file, -1, SEEK_CUR);
                break;
        }

        charCount--;
        
        if(!multiLineComment && !singleLineComment && charEncounterd) fputc(readChar, outputFile);
        if((char)readChar == '\n') charEncounterd = false;
    }
    

    fclose(outputFile);
    fclose(source_file);
}

#endif