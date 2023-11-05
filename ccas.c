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

int preprocess(char* source_file_name, char output_file_name) {
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
    bool singleLineComment = 0;
    bool multilineComment = 0;
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
        if(readChar == '\n') singleLineComment = false;
        if(readChar == '*') {
            if(fgetc(source_file) == '/') {
                multilineComment = false;
                fseek(source_file, -1, SEEK_CUR);
            } else fseek(source_file, -1, SEEK_CUR);
        }

        if((singleLineComment | multilineComment) == 0 ) {
            fputc(readChar, output_file);
        }
    }
    fclose(source_file);
    fclose(output_file);
    return 0;
}

int main(int argc, char** argv) {

    // Open source file
    FILE* source_file = fopen(argv[1], "r");
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
    
    fseek(source_file, 0, SEEK_SET);
    for(i = 0; i < lines; i++) {
        code[i] = (char*)calloc(characterAmount[i] + 1, sizeof(char));
        fgets(code[i], characterAmount[i] + 2, source_file);
        if(code[i] == NULL) {
            perror("Memory allocation Failed ");
            return 1;
        }
    }

    // Search and count for labels
    int amountOfLabels = 0;
    for(i = 0; i < lines; i++) {
        bool slashEncounterd = false;
        bool colonEncounterd = false;
        for(int j = 0; j < characterAmount[i]; j++) {
            if(code[i][j] == ':') {
                colonEncounterd = true;
                break;
            }
            if(slashEncounterd && code[i][j] == '/') {
                slashEncounterd = false;
                break;
            }
            if(code[i][j] == '/') slashEncounterd = true;
        }
        if(colonEncounterd) amountOfLabels++;
    }

    char** label = (char**)malloc(amountOfLabels * sizeof(char*));
    int labelIndex = 0;
    if(label == NULL) {
        perror("Memory allocation for label array Failed ");
        return 1;
    }

    // Copy labels into String array (2d char array)
    for(i = 0; i < lines; i++) {
        bool slashEncounterd = false;
        bool colonEncounterd = false;
        for(int j = 0; j < characterAmount[i]; j++) {
            if(code[i][j] == ':') {
                colonEncounterd = true;
                break;
            }
            if(slashEncounterd && code[i][j] == '/') {
                slashEncounterd = false;
                break;
            }
            if(code[i][j] == '/') slashEncounterd = true;
        }
        if(colonEncounterd) {
            int j;
            int amountOfCharsInLabel = 0;
            bool containsOnlyNumber = true;
            for(j = 0; j < characterAmount[i]; j++) {
                if(code[i][j] == ' ') {
                    printf("Error in %s:%d:%d\n\tLabels can't contain Spaces.\n", argv[1], i, j + 1);
                    for(int p = 0; p < labelIndex; p++) {
                        free(label[p]);
                    }
                    free(label);
                    for (int p = 0; p < lines; p++) {
                        free(code[p]);
                    }
                    free(code);
                    free(characterAmount);
                    fclose(source_file);
                    return 2;
                }
                if(code[i][j] == ':') break;
                else amountOfCharsInLabel++;
                switch(code[i][j]) {
                    case '0':break;
                    case '1':break;
                    case '2':break;
                    case '3':break;
                    case '4':break;
                    case '5':break;
                    case '6':break;
                    case '7':break;
                    case '8':break;
                    case '9':break;
                    case '-':break;
                    default:
                        containsOnlyNumber = false;
                        break;
                }
            }
            if(containsOnlyNumber) {
                printf("Error in label in %s:%d\n\tLabel cannot be named a number.\n", argv[1], i);
                for(int p = 0; p < labelIndex; p++) {
                        free(label[p]);
                    }
                    free(label);
                    for (int p = 0; p < lines; p++) {
                        free(code[p]);
                    }
                    free(code);
                    free(characterAmount);
                    fclose(source_file);
                    return 2;
            }
            label[labelIndex] = (char*)calloc(amountOfCharsInLabel + 1, sizeof(char));
            for(j = 0; j < amountOfCharsInLabel; j++) {
                label[labelIndex][j] = code[i][j];
            }
            labelIndex++;

        }
    }

    short *binary_code = calloc(lines - amountOfLabels, sizeof(short));
    printf("%d code?\n", lines - amountOfLabels);


    // clean up
    for (i = 0; i < lines; i++) {
        free(code[i]);
    }

    for (i = 0; i < amountOfLabels; i++) {
        free(label[i]);
    }

    free(label);
    free(code);
    free(binary_code);
    free(characterAmount);
    fclose(source_file);
    return 0;
}