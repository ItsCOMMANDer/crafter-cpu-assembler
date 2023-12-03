#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "include/assembleyInstructionConversion.h"

int main(int argc, char** argv) {
    char *input_file_name = "asmIN.s";
    char *output_file_name = "asmOUT.s";

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

    int lineIndex = 0;
    fseek(source_file, 0, SEEK_SET);
    while ((readCharacter = fgetc(source_file)) != EOF) {
        if (readCharacter == '\n') {
            lineIndex++;
        } else {
            characterAmount[lineIndex]++;
        }
    }

    // Cpoies code into String array (2d char array)
    char** code = (char**)malloc(lines * sizeof(char*));

    if(code == NULL) {
        perror("Memory allocation for code array Failed ");
        fclose(source_file);
        return 1;
    }

    int charIndex = 0;
    int readChar;
    fseek(source_file, 0, SEEK_SET);
    for(int i = 0; i < lines; i++) {
        code[i] = (char*)calloc(characterAmount[i] + 1, sizeof(char));

        if(code[i] == NULL) {
            perror("Memory allocation Failed ");
            for(int k = 0; k < i; k++) {
                free(code[k]);
            }
            free(code);
            fclose(source_file);
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
    uint16_t* labelAddresses = calloc(amountOfLabels, sizeof(uint16_t));
    uint16_t* labelLine = calloc(amountOfLabels, sizeof(uint16_t));
    int labelIndex = 0;
    for(int i = 0; i < lines; i++) {
        if(code[i][strlen(code[i]) - 1] == ':') {
            label[labelIndex] = calloc(strlen(code[i]), sizeof(char));
            for(int j = 0; j < strlen(code[i]) - 1; j++) {
                label[labelIndex][j] = code[i][j];
                label[labelIndex][strlen(code[i])] = 0;
                labelLine[labelIndex] = i;
            }
            labelAddresses[labelIndex] = i - labelIndex;
            labelIndex++;
        }
    }

    uint16_t *machine_code = calloc(lines - amountOfLabels, sizeof(uint16_t));
    if(machine_code == NULL) goto cleanup;
    int instructionIndex = 0;
    
    uint16_t args[3] = {0};

    asmParamsResult_t instruction;

    int currentLabelIndex = 0;

    //for(i)
    //TODO: continue



    for(int i = 0; i < instruction.amountOfParams; i++) {
        free(instruction.params[i]);
    }
    free(instruction.params);

    FILE *output_file = fopen(output_file_name, "wb");

    fwrite(machine_code, sizeof(uint16_t), lines - amountOfLabels, output_file);

    fclose(output_file);

    // clean up
    cleanup:
    for(int i = 0; i < amountOfLabels; i++) {
        free(label[i]);
    }
    free(label);
    free(labelAddresses);

    for (int i = 0; i < lines; i++) {
        free(code[i]);
    }

    free(code);
    free(characterAmount);
    fclose(source_file);
    return 0;
}