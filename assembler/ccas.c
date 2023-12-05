#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "include/assembleyInstructionConversion.h"

int main(int argc, char** argv) {
    if(argc < 3) {
        printf("Too little Arguments. Usage:\n\t%s \"<input file name>\" \"output file name>\"\n", argv[0]);
        return -1;
    }
    char *input_file_name = malloc(strlen(argv[1]) + 1);
    char *output_file_name = malloc(strlen(argv[2]) + 1);

    strncpy(input_file_name, argv[1], strlen(argv[1]) + 1);
    strncpy(output_file_name, argv[2], strlen(argv[2]) + 1);


    preprocess(input_file_name, output_file_name);
    // Open source file
    FILE* source_file = fopen(output_file_name, "r");
    if(source_file == NULL) {
        perror("Error opening file ");
        return -1;
    }

    // Count number of lines
    long amountOfLines = 1;
    char readCharacter;
    while ((readCharacter = fgetc(source_file)) != EOF) {
        if (readCharacter == '\n') {
            amountOfLines++;
        }
    }

    // Counts numbers of characters per line
    int *characterAmount = (int*)calloc(amountOfLines, sizeof(int));
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
    char** code = (char**)malloc(amountOfLines * sizeof(char*));

    if(code == NULL) {
        perror("Memory allocation for code array Failed ");
        fclose(source_file);
        return 1;
    }

    int charIndex = 0;
    int readChar;
    fseek(source_file, 0, SEEK_SET);
    for(int i = 0; i < amountOfLines; i++) {
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

    fclose(source_file);

    
    int amountOfLabels = 0;    
    for(int i = 0; i < amountOfLines; i++) {
        if(code[i][strlen(code[i]) - 1] == ':') amountOfLabels++;
    }
    
    char** label = calloc(amountOfLabels, sizeof(char*));
    uint16_t* labelAddresses = calloc(amountOfLabels, sizeof(uint16_t));
    uint16_t* labelLine = calloc(amountOfLabels, sizeof(uint16_t));
    bool labelsExist = false;

    int labelIndex = 0;
    for(int i = 0; i < amountOfLines; i++) {
        if(code[i][strlen(code[i]) - 1] == ':') {
            label[labelIndex] = calloc(strlen(code[i]), sizeof(char));
            for(int j = 0; j < strlen(code[i]) - 1; j++) {
                label[labelIndex][j] = code[i][j];
                label[labelIndex][strlen(code[i])] = 0;
                labelLine[labelIndex] = i;
                labelsExist = true;
            }
            labelAddresses[labelIndex] = i - labelIndex;
            labelIndex++;
        }
    }

    uint16_t *machine_code = calloc(amountOfLines - amountOfLabels, sizeof(uint16_t));
    if(machine_code == NULL) goto cleanup;
    int instructionIndex = 0;
    
    uint16_t args[3] = {0};

    asmParamsResult_t instruction;
    instructionToken_t instruction_tok;

    int currentLabelIndex = 0;

    for(lineIndex = 0; lineIndex < amountOfLines; lineIndex++) {
        printf("At line %d\n", lineIndex);
        if(!labelsExist && currentLabelIndex < amountOfLabels && lineIndex == labelLine[currentLabelIndex]) {
            currentLabelIndex++;
            printf("gkjhgh");
            continue;
        }

        instruction = getParametersFromAsembly(code[lineIndex]);
        instruction_tok = getOpcode(instruction.params[0]);
        printf("Instruction token = %d\ninstruction: %s\n", instruction_tok, instruction.params[0]);

        if(instruction_tok == NAI) {
            printf("Instruction \"%s\" doesnt exist.\n", instruction.params[0]);
            goto cleanup;
            return -1;
        }
        
        for(int paramIndex = 0; paramIndex < instruction.amountOfParams; paramIndex++) {
            switch(instructionPattern[instruction_tok][paramIndex]) {
                case INSTRUCTION_TOKEN_REGISTER:
                    if(getRegisterIndex(instruction.params[paramIndex + 1]) != NAR) {
                        printf("Register at instruction %d at arg %d validated.\n", lineIndex, paramIndex);
                        args[paramIndex] = getRegisterIndex(instruction.params[paramIndex + 1]);
                    }
                    else {printf("Register at instruction %d at arg %d not validated, because register doesnt exists\n", lineIndex, paramIndex); return -1;}
                    break;

                case INSTRUCTION_TOKEN_IMMIDIETE:
                    if(isNumber(instruction.params[paramIndex + 1])) {
                        if(atoi(instruction.params[paramIndex + 1]) <= 0b11111111) {
                            printf("Immidiete at instruction %d at arg %d validated.\n", lineIndex, paramIndex);
                            args[paramIndex] = (uint16_t)atoi(instruction.params[paramIndex + 1]);
                        } else {printf("Immidiete at instruction %d at arg %d not validated, because number is to big (exeeds 8bit intiger limit).\n", lineIndex, paramIndex); return -1;}
                    } else {printf("Immidiete at instruction %d at arg %d not validated, because arguments isnt a number.\n", lineIndex, paramIndex); return -1;}
                    break;

                case INSTRUCTION_TOKEN_ADDRESS:
                    if(isNumber(instruction.params[paramIndex + 1])) {
                        if(atoi(instruction.params[paramIndex + 1]) <= 0b11111111111) {
                            printf("Offset at instruction %d at arg %d validated.", lineIndex, paramIndex);
                            args[paramIndex] = atoi(instruction.params[paramIndex + 1]);
                        } else {printf("Offset at instruction %d at arg %d not validated, because number is to big (exeeds 8bit intiger limit).\n", lineIndex, paramIndex); return -1;}
                    } else {printf("Offset at instruction %d at arg %d not validated, because arguments isnt a number.\n", lineIndex, paramIndex); return -1;}
                    break;
            }
        }

        machine_code[instructionIndex] = assembleInstructions[(int)instruction_tok](args[0], args[1], args[2]);
        printf("Finished instruction binary: 0x%X\n", machine_code[instructionIndex]);
        instructionIndex++;

        for(int index = 0; index < instruction.amountOfParams; index++) {
            free(instruction.params[index]);
        }
        free(instruction.params);
    }
    printf("Done!!!");


    for(int i = 0; i < instruction.amountOfParams; i++) {
        free(instruction.params[i]);
    }
    free(instruction.params);

    FILE *output_file = fopen(output_file_name, "wb");

    fwrite(machine_code, sizeof(uint16_t), amountOfLines - amountOfLabels, output_file);

    fclose(output_file);

    // clean up
    cleanup:
    free(input_file_name);
    free(output_file_name);

    free(machine_code);

    for(int i = 0; i < amountOfLabels; i++) {
        free(label[i]);
    }
    free(label);
    free(labelAddresses);

    for (int i = 0; i < amountOfLines; i++) {
        free(code[i]);
    }
    free(code);
    free(characterAmount);
    return 0;
}