#ifndef assemblyInstructionConversion_H
#define assemblyInstructionConversion_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


bool isDigit(char c) {return (c >= '0' && c <= '9') ? true : false;}
bool isNumber(char* string) {
    if(string == NULL || *string == '\0') return false;
    for(; *string != '\0'; string++) {
        if(!isDigit(*string)) return false;
    }
    return true;
}

#define AMOUNT_OF_REGISTERS 8
#define INTRUCTION_COUNT 32

#define REG0_SHIFT_BITS 8
#define REG1_SHIFT_BITS 5
#define REG2_SHIFT_BITS 2

typedef enum instruction_set {
    ADD = 0b00000000000000,          //instruction 0
    SUB = 0b0000100000000000,          //instruction 1
    UNUSED0 = 0b0001000000000000,          //instruction 2
    CMP = 0b0001100000000000,          //instruction 3
    ADC = 0b0010000000000000,          //instruction 4
    SBC = 0b0010100000000000,          //instruction 5
    NAND = 0b0011000000000000,          //instruction 6
    XOR = 0b0011100000000000,          //instruction 7
    UNUSED1 = 0b0100000000000000,          //instruction 8
    SHR = 0b0100100000000000,          //instruction 9
    LSH = 0b0101000000000000,          //instruction 10
    UNUSED2 = 0b0101100000000000,          //instruction 11
    ADDI = 0b0110000000000000,          //instruction 12
    SUBI = 0b0110100000000000,          //instruction 13
    CMPI = 0b0111000000000000,          //instruction 14
    JMP = 0b0111100000000000,          //instruction 15
    JZ = 0b1000000000000000,          //instruction 16
    JN = 0b1000100000000000,          //instruction 17
    JP = 0b1001000000000000,          //instruction 18
    LDIR = 0b1001100000000000,          //instruction 19
    LDRR = 0b1010000000000000,          //instruction 20
    LDRMR = 0b1010100000000000,          //instruction 21
    LDMRR = 0b1011000000000000,          //instruction 22
    LDRMRR = 0b1011100000000000,          //instruction 23
    LDMRRR = 0b1100000000000000,          //instruction 24
    PUSH = 0b1100100000000000,          //instruction 25
    POP = 0b1101000000000000,          //instruction 26
    SCF = 0b1101100000000000,          //instruction 27
    RCF = 0b1110000000000000,          //instruction 28
    CALL = 0b1110100000000000,          //instruction 29
    RET = 0b1111000000000000,          //instruction 30
    NOP = 0b1111100000000000,          //instruction 31
    NAI = -1 // not an instrution
} OPCODE_t;


typedef enum instructionToken {
    REGISTER = 0,
    IMMIDIETE = 1,
    OFFSET = 2,
    NONE = 3,
} instructionToken_t;

const instructionToken_t instructionPattern[32][3] = {
    {REGISTER, REGISTER, NONE},             //instruction 0
    {REGISTER, REGISTER, NONE},             //instruction 1
    {NONE, NONE, NONE},                     //instruction 2
    {REGISTER, REGISTER, NONE},             //instruction 3
    {REGISTER, REGISTER, NONE},             //instruction 4
    {REGISTER, REGISTER, NONE},             //instruction 5
    {REGISTER, REGISTER, NONE},             //instruction 6
    {REGISTER, REGISTER, NONE},             //instruction 7
    {NONE, NONE, NONE},                     //instruction 8
    {REGISTER, NONE, NONE},                 //instruction 9
    {NONE, NONE, NONE},                     //instruction 10
    {NONE, NONE, NONE},                     //instruction 11
    {REGISTER, IMMIDIETE, NONE},            //instruction 12
    {REGISTER, IMMIDIETE, NONE},            //instruction 13
    {REGISTER, IMMIDIETE, NONE},            //instruction 14
    {OFFSET, NONE, NONE},                   //instruction 15
    {OFFSET, NONE, NONE},                   //instruction 16
    {OFFSET, NONE, NONE},                   //instruction 17
    {OFFSET, NONE, NONE},                   //instruction 18
    {REGISTER, IMMIDIETE, NONE},            //instruction 19
    {REGISTER, REGISTER, NONE},             //instruction 20
    {REGISTER, REGISTER, NONE},             //instruction 21
    {REGISTER, REGISTER, NONE},             //instruction 22
    {REGISTER, REGISTER, REGISTER},         //instruction 23
    {REGISTER, REGISTER, REGISTER},         //instruction 24
    {REGISTER, NONE, NONE},                 //instruction 25
    {REGISTER, NONE, NONE},                 //instruction 26
    {NONE, NONE, NONE},                     //instruction 27
    {NONE, NONE, NONE},                     //instruction 28
    {OFFSET, NONE, NONE},                   //instruction 29
    {NONE, NONE, NONE},                     //instruction 30
    {NONE, NONE, NONE}                      //instruction 31
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
};

const char asmInstructionStrings[32][16] = {
    "ADD",
    "SUB",
    "",   // unused
    "CMP",
    "ADC",
    "SBC",
    "NAND",
    "XOR",
    "",   // unused
    "SHR",
    "",   // unused
    "",   // unused
    "ADDI",
    "SUBI",
    "CMPI",
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
    "SCF",
    "RCF",
    "CALL",
    "RET",
    "NOP"
};

const OPCODE_t opcodes[] = {
    ADD,
    SUB,
    UNUSED0,
    CMP,
    ADC,
    SBC,
    NAND,
    XOR,
    UNUSED1,
    SHR,
    LSH,
    UNUSED2,
    ADDI,
    SUBI,
    CMPI,
    JMP,
    JZ,
    JN,
    JP,
    LDIR,
    LDRR,
    LDRMR,
    LDMRR,
    LDRMRR,
    LDMRRR,
    PUSH,
    POP,
    SCF,
    RCF,
    CALL,
    RET,
    NOP
};

typedef enum registerIndex {
    A = 0, 
    B = 1,
    C = 2,
    H = 3,
    L = 4,
    PC = 5,
    SP = 6,
    BP = 7,
    NAR = -1
} registerIndex_t;

uint16_t addInstruction(uint16_t reg0, uint16_t reg1, uint16_t unused0) {return (ADD | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS));}
uint16_t subInstruction(uint16_t reg0, uint16_t reg1, uint16_t unused0) {return (SUB | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS));}
uint16_t u0Instruction(uint16_t unused0, uint16_t unused1, uint16_t unused2) {return NOP;}
uint16_t cmpInstruction(uint16_t reg0, uint16_t reg1, uint16_t unused0) {return (CMP | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS));}
uint16_t adcInstruction(uint16_t reg0, uint16_t reg1, uint16_t unused0) {return (ADC | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS));}
uint16_t sbcInstruction(uint16_t reg0, uint16_t reg1, uint16_t unused0) {return (SBC | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS));}
uint16_t nandInstruction(uint16_t reg0, uint16_t reg1, uint16_t unused0) {return (NAND | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS));}
uint16_t xorInstruction(uint16_t reg0, uint16_t reg1, uint16_t unused0) {return (XOR | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS));}
uint16_t u1Instruction(uint16_t unused0, uint16_t unused1, uint16_t unused2) {return NOP;}
uint16_t shrInstruction(uint16_t reg0, uint16_t unused0, uint16_t unused1) {return (SHR | (reg0 << REG0_SHIFT_BITS));}
uint16_t u2Instruction(uint16_t unused0, uint16_t unused1, uint16_t unused2) {return NOP;}
uint16_t u3Instruction(uint16_t unused0, uint16_t unused1, uint16_t unused2) {return NOP;}
uint16_t addiInstruction(uint16_t reg0, uint16_t immidiete, uint16_t unused) {return ADDI | (reg0 << REG0_SHIFT_BITS) | (immidiete && 0b11111111);}
uint16_t subiInstruction(uint16_t reg0, uint16_t immidiete, uint16_t unused) {return SUBI | (reg0 << REG0_SHIFT_BITS) | (immidiete && 0b11111111);}
uint16_t cmpiInstruction(uint16_t reg0, uint16_t immidiete, uint16_t unused) {return CMPI | (reg0 << REG0_SHIFT_BITS) | (immidiete && 0b11111111);}
uint16_t jmpInstruction(uint16_t address, uint16_t unused0, uint16_t unused1) {return (JMP | (address & 0b0000011111111111));}
uint16_t jzInstruction(uint16_t address, uint16_t unused0, uint16_t unused1) {return (JZ | (address & 0b0000011111111111));}
uint16_t jnInstruction(uint16_t address, uint16_t unused0, uint16_t unused1) {return (JN | (address & 0b0000011111111111));}
uint16_t jpInstruction(uint16_t address, uint16_t unused0, uint16_t unused1) {return (JP | (address & 0b0000011111111111));}
uint16_t ldirInstruction(uint16_t reg0, uint16_t immidiete, uint16_t unused0) {return (LDIR | (reg0 << REG0_SHIFT_BITS) | ((immidiete & 0b11111111) << REG1_SHIFT_BITS));}
uint16_t ldrrInstruction(uint16_t reg0, uint16_t reg1, uint16_t unused0) {return (LDRR | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS));}
uint16_t ldrmrInstruction(uint16_t reg0, uint16_t reg1, uint16_t unused0) {return (LDRR | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS));}
uint16_t ldmrrInstruction(uint16_t reg0, uint16_t reg1, uint16_t unused0) {return (LDRR | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS));}
uint16_t ldrmrrInstruction(uint16_t reg0, uint16_t reg1, uint16_t reg2) {return (LDRMRR | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS) | (reg2 << REG2_SHIFT_BITS));}
uint16_t ldmrrrInstruction(uint16_t reg0, uint16_t reg1, uint16_t reg2) {return (LDMRRR | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS) | (reg2 << REG2_SHIFT_BITS));}
uint16_t pushInstruction(uint16_t reg0, uint16_t unused0, uint16_t unused1) {return (PUSH | (reg0 << REG0_SHIFT_BITS));}
uint16_t popInstruction(uint16_t reg0, uint16_t unused0, uint16_t unused1) {return (POP | (reg0 << REG0_SHIFT_BITS));}
uint16_t scfInstruction(uint16_t unused0, uint16_t unused1, uint16_t unused2) {return SCF;}
uint16_t rcfInstruction(uint16_t unused0, uint16_t unused1, uint16_t unused2) {return RCF;}
uint16_t callInstruction(uint16_t address, uint16_t unused0, uint16_t unused1) {return CALL | (address & 0b11111111111);}
uint16_t retInstruction(uint16_t unused0, uint16_t unused1, uint16_t unused2) {return RET;}
uint16_t nopInstruction(uint16_t unused0, uint16_t unused1, uint16_t unused2) {return NOP;}

const uint16_t (*assembleInstructions[])(uint16_t, uint16_t, uint16_t) = {
    addInstruction,
    subInstruction,
    u0Instruction,
    cmpInstruction,
    adcInstruction,
    sbcInstruction,
    nandInstruction,
    xorInstruction,
    u1Instruction,
    shrInstruction,
    u2Instruction,
    u3Instruction,
    addiInstruction,
    subiInstruction,
    cmpiInstruction,
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
    scfInstruction,
    rcfInstruction,
    callInstruction,
    retInstruction,
    nopInstruction
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
        if(_stricmp(regName, registerNames[i]) == 0) return registerIndexes[i];
    }
    return NAR;
}

uint16_t getOpcode(char* opcodeSting) {
    for(int i = 0; i < INTRUCTION_COUNT; i++) {
        if(_stricmp(opcodeSting, asmInstructionStrings[i]) == 0) return (int)opcodes[i] >> 11;
    }
    return NAI;
} 

typedef struct asmParamsResult{
    char** params;
    int amountOfParams;
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
    char** paramPointer = (char**)calloc(res.amountOfParams, sizeof(char*));
    
    newParamHasBegan = false;

    for(i = 0; i < strlen(assembly); i++) {
        if(assembly[i] != ',' && assembly[i] != ' ' && newParamHasBegan == false) newParamHasBegan = true;
        
        if((assembly[i] == ',' || assembly[i] == ' ') && newParamHasBegan == true) {
            newParamHasBegan = false;
            
            paramPointer[paramIndex] = (char*)calloc(charsPerParam + 1, sizeof(char));
            for(int j = 0; j < charsPerParam; j++) paramPointer[paramIndex][j] = assembly[i - (charsPerParam - j)];
            paramIndex++;
            charsPerParam = 0;
        }
        if(newParamHasBegan == true) charsPerParam++;
    }

    paramPointer[paramIndex] = (char*)calloc(charsPerParam + 1, sizeof(char));
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