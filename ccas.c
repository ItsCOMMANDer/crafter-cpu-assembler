#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

enum instruction_set {
    ADD = 0b00000,
    SUB = 0b00001,
    ADC = 0b00010,
    SBC = 0b00011,
    NAND = 0b00100,
    XOR = 0b00101,
    CMP = 0b00110,
    SHL = 0b00111,
    SHR = 0b01000,
    INC = 0b01001,
    DEC = 0b01010,
    JMP = 0b01011,
    JZ = 0b01100,
    JN = 0b01101,
    LDIR = 0b01110,
    LDRR = 0b01111,
    LDRMR = 0b10000,
    LDMRR = 0b10001,
    LDRMRR = 0b10010,
    LDMRRR = 0b10011,
    PUSH = 0b10100,
    POP = 0b10101,
    UNDEFINED0 = 0b10110,
    UNDEFINED1 = 0b10111,
    UNDEFINED2 = 0b11000,
    UNDEFINED3 = 0b11001,
    UNDEFINED4 = 0b11010,
    UNDEFINED5 = 0b11011,
    UNDEFINED6 = 0b11100,
    UNDEFINED7 = 0b11101,
    UNDEFINED8 = 0b11110,
    NOP = 0b11111,
};

int preprocess(char* source_file_name, char* output_file_name) {
    FILE *source_file = fopen(source_file_name, "r");
    if(source_file == NULL) {
        perror("Error opening file ");
        return -1;
    }
    FILE *output_file = fopen(output_file_name, "w");
    if(output_file == NULL) {
        perror("Error opening output file ");
        fclose(source_file);
        return -1;
    }
    _Bool singleLineComment = 0;
    _Bool multilineComment = 0;
    _Bool charAfterNewLine = 0;
    int numberOfCharsBeforeNewLine = 0;
    int readChar;

    readChar = fgetc(source_file);
    while(readChar != EOF) {
        if((char)readChar == '/') {
            if(fgetc(source_file) == '/') {
                singleLineComment = 1;
                fseek(source_file, -1, SEEK_CUR);
            } else fseek(source_file, -1, SEEK_CUR);

            if(fgetc(source_file) == '*') {
                multilineComment = 1;
                fseek(source_file, -1, SEEK_CUR);
            } else fseek(source_file, -1, SEEK_CUR);
        }

        if((char)readChar != ' ' && (char)readChar != '\t' && (char)readChar != '\n' && singleLineComment == 0 && multilineComment == 0) {
            charAfterNewLine = 1;
            numberOfCharsBeforeNewLine = 0;
        } else if(((char)readChar == ' ' || (char)readChar == '\t') && singleLineComment == 0 && multilineComment == 0) {
            numberOfCharsBeforeNewLine--;
        }

        if((char)readChar == '\n' && singleLineComment == 0 && multilineComment == 0) {
            fseek(output_file, numberOfCharsBeforeNewLine, SEEK_CUR);
            numberOfCharsBeforeNewLine = 0;
            singleLineComment = 0;
        }

        if(singleLineComment == 0 && multilineComment == 0 && charAfterNewLine == 1) {
            fputc((char)readChar, output_file);
            if((char)readChar == '\n') charAfterNewLine = 0;
        }

        if(readChar == '*') {
            if(fgetc(source_file) == '/') {
                multilineComment = 0;
                fseek(source_file, 1, SEEK_CUR);
            } else fseek(source_file, -1, SEEK_CUR);
        }
        readChar = fgetc(source_file);
    }
    fclose(output_file);
    output_file = fopen(output_file_name, "r");
    int charAmount = 0;
    while((readChar = getc(output_file)) != EOF) charAmount++;
    charAmount+=numberOfCharsBeforeNewLine;
    char* outputFileResize = calloc(charAmount, sizeof(char));
    if(outputFileResize == NULL) {
        perror("Memory allocation failed");
        fclose(output_file);
        fclose(source_file);
        return -1;
    }

    fseek(output_file, 0, SEEK_SET);

    for(int i = 0; i < charAmount; i++) {
        outputFileResize[i] = getc(output_file);
    }
    fclose(output_file);

    output_file = fopen(output_file_name, "w");
    for(int i = 0; i < charAmount; i++) {
        fputc(outputFileResize[i], output_file);
    }

    free(outputFileResize);
    fclose(output_file);
    fclose(source_file);
}

int main(int argc, char** argv) {
    char *input_file_name = "tests\\simple.s";
    char *output_file_name = "tests\\simpleOut.s";

    preprocess(input_file_name, output_file_name);
    // Open source file
    FILE* source_file = fopen(output_file_name, "r");
    if(source_file == NULL) {
        perror("Error opening file ");
        return -1;
    }

    // Count number of lines
    long lines = 1;
    char readCharacter;
    while ((readCharacter = fgetc(source_file)) != EOF) {
        if (readCharacter == '\n') {
            lines++;
        }
    }

    // Counts numbers of characters per line
    int *characterAmount = (int*)calloc(lines, sizeof(int));
    if (characterAmount == NULL) {
        perror("Memory allocation failed");
        fclose(source_file);
        return -1;
    }

    int i = 0;
    fseek(source_file, 0, SEEK_SET);
    while ((readCharacter = fgetc(source_file)) != EOF) {
        if (readCharacter == '\n') {
            i++;
        } else {
            characterAmount[i]++;
        }
    }

    // Cpoies code into String array (2d char array)
    char** code = (char**)malloc(lines * sizeof(char*));

    if(code == NULL) {
        perror("Memory allocation for code array Failed ");
        return 1;
    }
    int charIndex = 0;
    int readChar;
    fseek(source_file, 0, SEEK_SET);
    for(i = 0; i < lines; i++) {
        code[i] = (char*)calloc(characterAmount[i] + 1, sizeof(char));

        if(code[i] == NULL) {
            perror("Memory allocation Failed ");
            return 1;
        }

        while((readChar = getc(source_file)) != '\n' && readChar != EOF) {
            code[i][charIndex] = readChar;
            charIndex++;
        }
        charIndex = 0;
        code[i][characterAmount[i] + 1] = 0;
    }

    
    int amountOfLabels = 0;    
    for(int i = 0; i < lines; i++) {
        if(code[i][strlen(code[i]) - 1] == ':') amountOfLabels++;
    }
    
    char** label = calloc(amountOfLabels, sizeof(char*));
    short* labelAddresses = calloc(amountOfLabels, sizeof(short));
    int labelIndex = 0;
    for(int i = 0; i < lines; i++) {
        if(code[i][strlen(code[i]) - 1] == ':') {
            label[labelIndex] = calloc(strlen(code[i]), sizeof(char));
            for(int j = 0; j < strlen(code[i]) - 1; j++) {
                label[labelIndex][j] = code[i][j];
                label[labelIndex][strlen(code[i])] = 0;
            }
            labelAddresses[labelIndex] = i - labelIndex;
            labelIndex++;
        }
    }

    for(int i = 0; i < labelIndex; i++) {
        printf("Label \"%s\" at address %d\n", label[i], labelAddresses[i]);
    }

    // clean up
    for(int i = 0; i < amountOfLabels; i++) {
        free(label[i]);
    }
    free(label);
    free(labelAddresses);

    for (i = 0; i < lines; i++) {
        free(code[i]);
    }

    free(code);
    free(characterAmount);
    fclose(source_file);
    return 0;
}