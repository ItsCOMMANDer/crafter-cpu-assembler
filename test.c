#include <stdio.h>
#include <stdbool.h>

int main(int argc, char** argv) {
    FILE *source_file = fopen(argv[1], "r");
    if(source_file == NULL) {
        perror("Error opening file ");
        return -1;
    }
    FILE *output_file = fopen(argv[2], "w");
    if(output_file == NULL) {
        perror("Error opening output file ");
        fclose(source_file);
        return -1;
    }
    _Bool singleLineComment = 0;
    _Bool multilineComment = 0;
    char readChar;
    while((readChar = fgetc(source_file)) != EOF) {
        if(readChar == '/') {
            if(fgetc(source_file) == '/') {
                singleLineComment = true;
                fseek(source_file, -1, SEEK_CUR);
            } else fseek(source_file, -1, SEEK_CUR);
            
            if(fgetc(source_file) == '*') {
                multilineComment = true;
                fseek(source_file, -1, SEEK_CUR);
            } else fseek(source_file, -1, SEEK_CUR);
        }

        if(readChar == '\n') {
            singleLineComment = false;
            if(fgetc(source_file) == '\n') {
                fseek(source_file, 2, SEEK_CUR);
            } else {
                fseek(source_file, -1, SEEK_CUR);
            }
        }
        
        if(singleLineComment == 0 && multilineComment == 0 && readChar != '\t') {
            fputc(readChar, output_file);
        }

        if(readChar == '*') {
            if(fgetc(source_file) == '/') {
                multilineComment = false;
                fseek(source_file, 1, SEEK_CUR);
            } else fseek(source_file, -1, SEEK_CUR);
        }
    }
    fclose(source_file);
    fclose(output_file);
    return 0;
}