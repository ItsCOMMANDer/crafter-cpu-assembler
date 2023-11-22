#ifndef assemblyInstructionConversion_H
#define assemblyInstructionConversion_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define AMOUNT_OF_REGISTERS 8
#define INTRUCTION_COUNT 32

#define REG0_SHIFT_BITS 8
#define REG1_SHIFT_BITS 5
#define REG2_SHIFT_BITS 2

typedef enum instruction_set {
    ADD = 0b00000000000000,
    SUB = 0b0000100000000000,
    ADC = 0b0001000000000000,
    SBC = 0b0001100000000000,
    NAND = 0b0010000000000000,
    XOR = 0b0010100000000000,
    CMP = 0b0011000000000000,
    SHL = 0b0011100000000000,
    SHR = 0b0100000000000000,
    INC = 0b0100100000000000,
    DEC = 0b0101000000000000,
    JMP = 0b0101100000000000,
    JZ = 0b0110000000000000,
    JN = 0b0110100000000000,
    LDIR = 0b0111000000000000,
    LDRR = 0b0111100000000000,
    LDRMR = 0b1000000000000000,
    LDMRR = 0b1000100000000000,
    LDRMRR = 0b1001000000000000,
    LDMRRR = 0b1001100000000000,
    PUSH = 0b1010000000000000,
    POP = 0b1010100000000000,
    JP = 0b1011000000000000,
    UNDEFINED1 = 0b1011100000000000,
    UNDEFINED2 = 0b1100000000000000,
    UNDEFINED3 = 0b1100100000000000,
    UNDEFINED4 = 0b1101000000000000,
    UNDEFINED5 = 0b1101100000000000,
    UNDEFINED6 = 0b1110000000000000,
    UNDEFINED7 = 0b1110100000000000,
    UNDEFINED8 = 0b1111000000000000,
    NOP = 0b1111100000000000,
    NAI = 0b11111111111111111111111111111111
} OPCODE_t;


typedef enum instructionToken {
    REGISTER = 0,
    IMMIDIETE = 1,
    OFFSET = 2,
    NONE = 3,
} instructionToken_t;

const instructionToken_t instructionPattern[32][3] = {
    {REGISTER, REGISTER, NONE},
    {REGISTER, REGISTER, NONE},
    {REGISTER, REGISTER, NONE},
    {REGISTER, REGISTER, NONE},
    {REGISTER, REGISTER, NONE},
    {REGISTER, REGISTER, NONE},
    {REGISTER, REGISTER, NONE},
    {REGISTER, NONE, NONE},
    {REGISTER, NONE, NONE},
    {REGISTER, NONE, NONE},
    {REGISTER, NONE, NONE},
    {OFFSET, NONE, NONE},
    {OFFSET, NONE, NONE},
    {OFFSET, NONE, NONE},
    {REGISTER, IMMIDIETE, NONE},
    {REGISTER, REGISTER, NONE},
    {REGISTER, REGISTER, NONE},
    {REGISTER, REGISTER, NONE},
    {REGISTER, REGISTER, REGISTER},
    {REGISTER, REGISTER, REGISTER},
    {REGISTER, NONE, NONE},
    {REGISTER, NONE, NONE},
    {NONE, NONE, NONE},
    {NONE, NONE, NONE},
    {NONE, NONE, NONE},
    {NONE, NONE, NONE},
    {NONE, NONE, NONE},
    {NONE, NONE, NONE},
    {NONE, NONE, NONE},
    {NONE, NONE, NONE},
    {NONE, NONE, NONE},
    {NONE, NONE, NONE},
    {NONE, NONE, NONE},
};

const int amountOfParamsForInstruction[32] = {
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    2,
    2,
    2,
    2,
    2,
    2,
    3,
    3,
    1,
    1,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

typedef enum registerIndex {
    A = 0, 
    B = 1,
    C = 2,
    H = 3,
    L = 4,
    PC = 5,
    SP = 6,
    BP = 7
} registerIndex_t;

uint16_t addInstruction(registerIndex_t reg0, registerIndex_t reg1, uint16_t unused0) {return (ADD | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS));}
uint16_t subInstruction(registerIndex_t reg0, registerIndex_t reg1, uint16_t unused0) {return (SUB | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS));}
uint16_t adcInstruction(registerIndex_t reg0, registerIndex_t reg1, uint16_t unused0) {return (ADC | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS));}
uint16_t sbcInstruction(registerIndex_t reg0, registerIndex_t reg1, uint16_t unused0) {return (SBC | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS));}
uint16_t nandInstruction(registerIndex_t reg0, registerIndex_t reg1, uint16_t unused0) {return (NAND | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS));}
uint16_t xorInstruction(registerIndex_t reg0, registerIndex_t reg1, uint16_t unused0) {return (XOR | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS));}
uint16_t cmpInstruction(registerIndex_t reg0, registerIndex_t reg1, uint16_t unused0) {return (CMP | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS));}
uint16_t shlInstruction(registerIndex_t reg0, uint16_t unused0, uint16_t unused1) {return (SHR | (reg0 << REG0_SHIFT_BITS));}
uint16_t shrInstruction(registerIndex_t reg0, uint16_t unused0, uint16_t unused1) {return (SHL | (reg0 << REG0_SHIFT_BITS));}
uint16_t incInstruction(registerIndex_t reg0, uint16_t unused0, uint16_t unused1) {return (INC | (reg0 << REG0_SHIFT_BITS));}
uint16_t decInstruction(registerIndex_t reg0, uint16_t unused0, uint16_t unused1) {return (DEC | (reg0 << REG0_SHIFT_BITS));}
uint16_t jmpInstruction(int16_t address, uint16_t unused0, uint16_t unused1) {return (JMP | (address & 0b0000011111111111));}
uint16_t jzInstruction(int16_t address, uint16_t unused0, uint16_t unused1) {return (JZ | (address & 0b0000011111111111));}
uint16_t jnInstruction(int16_t address, uint16_t unused0, uint16_t unused1) {return (JN | (address & 0b0000011111111111));}
uint16_t jpInstruction(int16_t address, uint16_t unused0, uint16_t unused1) {return (JP | (address & 0b0000011111111111));}
uint16_t ldirInstruction(registerIndex_t reg0, int8_t immidiete, uint16_t unused0) {return (LDIR | (reg0 << REG0_SHIFT_BITS) | (immidiete << REG1_SHIFT_BITS));}
uint16_t ldrrInstruction(registerIndex_t reg0, registerIndex_t reg1, uint16_t unused0) {return (LDRR | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS));}
uint16_t ldrmrInstruction(registerIndex_t reg0, registerIndex_t reg1, uint16_t unused0) {return (LDRR | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS));}
uint16_t ldmrrInstruction(registerIndex_t reg0, registerIndex_t reg1, uint16_t unused0) {return (LDRR | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS));}
uint16_t ldrmrrInstruction(registerIndex_t reg0, registerIndex_t reg1, registerIndex_t reg2) {return (LDRMRR | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS) | (reg2 << REG2_SHIFT_BITS));}
uint16_t ldmrrrInstruction(registerIndex_t reg0, registerIndex_t reg1, registerIndex_t reg2) {return (LDMRRR | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS) | (reg2 << REG2_SHIFT_BITS));}
uint16_t pushInstruction(registerIndex_t reg0, uint16_t unused0, uint16_t unused1) {return (PUSH | (reg0 << REG0_SHIFT_BITS));}
uint16_t popInstruction(registerIndex_t reg0, uint16_t unused0, uint16_t unused1) {return (POP | (reg0 << REG0_SHIFT_BITS));}
uint16_t u0Instruction(uint16_t unused0, uint16_t unused1, uint16_t unused2) {}
uint16_t u1Instruction(uint16_t unused0, uint16_t unused1, uint16_t unused2) {}
uint16_t u2Instruction(uint16_t unused0, uint16_t unused1, uint16_t unused2) {}
uint16_t u3Instruction(uint16_t unused0, uint16_t unused1, uint16_t unused2) {}
uint16_t u4Instruction(uint16_t unused0, uint16_t unused1, uint16_t unused2) {}
uint16_t u5Instruction(uint16_t unused0, uint16_t unused1, uint16_t unused2) {}
uint16_t u6Instruction(uint16_t unused0, uint16_t unused1, uint16_t unused2) {}
uint16_t u7Instruction(uint16_t unused0, uint16_t unused1, uint16_t unused2) {}
uint16_t u8Instruction(uint16_t unused0, uint16_t unused1, uint16_t unused2) {}
uint16_t nopInstruction(uint16_t unused0, uint16_t unused1, uint16_t unused2) {return NOP;}

const uint16_t (*assembleInstructions[])(uint16_t, uint16_t, uint16_t) = {
    addInstruction,
    subInstruction,
    adcInstruction,
    sbcInstruction,
    nandInstruction,
    xorInstruction,
    cmpInstruction,
    shlInstruction,
    shrInstruction,
    incInstruction,
    decInstruction,
    jmpInstruction,
    jzInstruction,
    jnInstruction,
    jpInstruction,
    ldirInstruction,
    ldrrInstruction,
    ldrmrInstruction,
    ldmrrInstruction,
    ldrmrrInstruction,
    ldmrrrInstruction,
    pushInstruction,
    popInstruction,
    u0Instruction,
    u1Instruction,
    u2Instruction,
    u3Instruction,
    u4Instruction,
    u5Instruction,
    u6Instruction,
    u7Instruction,
    u8Instruction,
    nopInstruction,
};

registerIndex_t getRegisterIndex(char* regName) {
    const registerIndex_t registerIndexes[] = {A, B, C, H, L, PC, SP, BP};
    const char registerNames[][3] = {
        "A",
        "B",
        "C",
        "H",
        "L",
        "PC",
        "SP",
        "BP"
    };
    for(int i = 0; i < AMOUNT_OF_REGISTERS; i++) {
        if(strcmp(regName, registerNames[i]) == 0) return registerIndexes[i];
    }
}

OPCODE_t getOpcode(char* opcodeSting) {
    const char instructions[32][16] = {
        "ADD",
        "SUB",
        "ADC",
        "SBC",
        "NAND",
        "XOR",
        "CMP",
        "SHL",
        "SHR",
        "INC",
        "DEC",
        "JMP",
        "JZ",
        "JN",
        "JP",
        "LDIR",
        "LDRR",
        "LDRMR",
        "LDMRR",
        "LDRMRR",
        "LDMRRR",
        "PUSH",
        "POP",
        "undefined",
        "undefined",
        "undefined",
        "undefined",
        "undefined",
        "undefined",
        "undefined",
        "undefined",
        "NOP"
    };

    const OPCODE_t opcodes[] = {
        ADD,
        SUB,
        ADC,
        SBC,
        NAND,
        XOR,
        CMP,
        SHL,
        SHR,
        INC,
        DEC,
        JMP,
        JZ,
        JN,
        LDIR,
        LDRR,
        LDRMR,
        LDMRR,
        LDRMRR,
        LDMRRR,
        PUSH,
        POP,
        JP,
        UNDEFINED1,
        UNDEFINED2,
        UNDEFINED3,
        UNDEFINED4,
        UNDEFINED5,
        UNDEFINED6,
        UNDEFINED7,
        UNDEFINED8,
        NOP
    };

    for(int i = 0; i < INTRUCTION_COUNT; i++) {
        if(strcmp(opcodeSting, instructions[i]) == 0) return opcodes[i];
    }
    return NAI;
} 

typedef struct {
    char** params;
    int amountOfParams
} asmParamsResult_t;

asmParamsResult_t getParametersFromAsembly(char* assembly) {
    asmParamsResult_t res = {0};
    bool newParamHasBegan = false;
    int charsPerParam = 0;
    int paramIndex = 0;
    int i;


    for(i = 0; i < strlen(assembly); i++) {
        if(assembly[i] != ',' && assembly[i] != ' ' && newParamHasBegan == false) {
            newParamHasBegan = true;
        }
        if((assembly[i] == ',' || assembly[i] == ' ') && newParamHasBegan == true) {
            newParamHasBegan = false;
            res.amountOfParams++;
        }
    }
    res.amountOfParams++;
    char** paramPointer = calloc(res.amountOfParams, sizeof(char*));
    
    newParamHasBegan = false;

    for(i = 0; i < strlen(assembly); i++) {
        if(assembly[i] != ',' && assembly[i] != ' ' && newParamHasBegan == false) newParamHasBegan = true;
        
        if((assembly[i] == ',' || assembly[i] == ' ') && newParamHasBegan == true) {
            newParamHasBegan = false;
            
            paramPointer[paramIndex] = calloc(charsPerParam + 1, sizeof(char));
            for(int j = 0; j < charsPerParam; j++) paramPointer[paramIndex][j] = assembly[i - (charsPerParam - j)];
            paramIndex++;
            charsPerParam = 0;
        }
        if(newParamHasBegan == true) charsPerParam++;
    }

    paramPointer[paramIndex] = calloc(charsPerParam + 1, sizeof(char));
    for(int j = 0; j < charsPerParam; j++) paramPointer[paramIndex][j] = assembly[i - (charsPerParam - j)];
    newParamHasBegan = false;

    charsPerParam = 0;

    res.params = paramPointer;
    return res;
}

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
    bool singleLineComment = 0;
    bool multilineComment = 0;
    bool charAfterNewLine = 0;
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
    char* outputFileResize = (char*)calloc(charAmount, sizeof(char));
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

#endif