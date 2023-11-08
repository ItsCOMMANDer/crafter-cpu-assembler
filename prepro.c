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
    _Bool charAfterNewLine = 0;
    int readChar;
    int numberOfCharsBeforeNewLine = 0;
    readChar = fgetc(source_file);
    while(readChar != EOF) {
        if((char)readChar != ' ' && (char)readChar != '\t') {
            charAfterNewLine = 1;
            numberOfCharsBeforeNewLine = 0;
        } else numberOfCharsBeforeNewLine++;

        if((char)readChar == '/') {
            if(fgetc(source_file) == '/') {
                singleLineComment = true;
                fseek(source_file, -1, SEEK_CUR);
            } else fseek(source_file, -1, SEEK_CUR);

            if(fgetc(source_file) == '*') {
                multilineComment = true;
                fseek(source_file, -1, SEEK_CUR);
            } else fseek(source_file, -1, SEEK_CUR);
        }

        if((char)readChar == '\n') {
            printf("nbnl: %d\n", numberOfCharsBeforeNewLine);
            fseek(output_file, -numberOfCharsBeforeNewLine, SEEK_CUR);
            numberOfCharsBeforeNewLine = 0;

            charAfterNewLine = 0;
            singleLineComment = false;
            if(fgetc(source_file) == '\n') {
                fputc('\n', output_file);
                fseek(source_file, 2, SEEK_CUR);
            } else {
                fputc('\n', output_file);
                fseek(source_file, -1, SEEK_CUR);
            }
        }

        if(singleLineComment == 0 && multilineComment == 0 && charAfterNewLine == 1) {
            fputc((char)readChar, output_file);
        }

        if(readChar == '*') {
            if(fgetc(source_file) == '/') {
                multilineComment = false;
                fseek(source_file, 1, SEEK_CUR);
            } else fseek(source_file, -1, SEEK_CUR);
        }
        readChar = fgetc(source_file);
    }
    fclose(source_file);
    fclose(output_file);
    return 0;
}
