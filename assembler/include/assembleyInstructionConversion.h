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
#define INSTRUCTION_COUNT 32

#define REG0_SHIFT_BITS 8
#define REG1_SHIFT_BITS 5
#define REG2_SHIFT_BITS 2

typedef enum instruction_set {
    OPCODE_ADD = 0b00000000000000,                 //instruction 0
    OPCODE_SUB = 0b0000100000000000,               //instruction 1
    OPCODE_UNUSED0 = 0b0001000000000000,           //instruction 2
    OPCODE_CMP = 0b0001100000000000,               //instruction 3
    OPCODE_ADC = 0b0010000000000000,               //instruction 4
    OPCODE_SBC = 0b0010100000000000,               //instruction 5
    OPCODE_NAND = 0b0011000000000000,              //instruction 6
    OPCODE_XOR = 0b0011100000000000,               //instruction 7
    OPCODE_UNUSED1 = 0b0100000000000000,           //instruction 8
    OPCODE_SHR = 0b0100100000000000,               //instruction 9
    OPCODE_LSH = 0b0101000000000000,               //instruction 10
    OPCODE_UNUSED2 = 0b0101100000000000,           //instruction 11
    OPCODE_ADDI = 0b0110000000000000,              //instruction 12
    OPCODE_SUBI = 0b0110100000000000,              //instruction 13
    OPCODE_CMPI = 0b0111000000000000,              //instruction 14
    OPCODE_JMP = 0b0111100000000000,               //instruction 15
    OPCODE_JZ = 0b1000000000000000,                //instruction 16
    OPCODE_JN = 0b1000100000000000,                //instruction 17
    OPCODE_JP = 0b1001000000000000,                //instruction 18
    OPCODE_LDIR = 0b1001100000000000,              //instruction 19
    OPCODE_LDRR = 0b1010000000000000,              //instruction 20
    OPCODE_LDRMR = 0b1010100000000000,             //instruction 21
    OPCODE_LDMRR = 0b1011000000000000,             //instruction 22
    OPCODE_LDRMRR = 0b1011100000000000,            //instruction 23
    OPCODE_LDMRRR = 0b1100000000000000,            //instruction 24
    OPCODE_PUSH = 0b1100100000000000,              //instruction 25
    OPCODE_POP = 0b1101000000000000,               //instruction 26
    OPCODE_SCF = 0b1101100000000000,               //instruction 27
    OPCODE_RCF = 0b1110000000000000,               //instruction 28
    OPCODE_CALL = 0b1110100000000000,              //instruction 29
    OPCODE_RET = 0b1111000000000000,               //instruction 30
    OPCODE_NOP = 0b1111100000000000,               //instruction 31
    NAI = -1 // not an instrution
} OPCODE_t;


typedef enum instructionToken {
    INSTRUCTION_TOKEN_REGISTER = 0,
    INSTRUCTION_TOKEN_IMMIDIETE = 1,
    INSTRUCTION_TOKEN_ADDRESS = 2,
    INSTRUCTION_TOKEN_NONE = 3,
} instructionToken_t;

const instructionToken_t instructionPattern[32][3] = {
    {INSTRUCTION_TOKEN_REGISTER, INSTRUCTION_TOKEN_REGISTER, INSTRUCTION_TOKEN_NONE},              //instruction 0
    {INSTRUCTION_TOKEN_REGISTER, INSTRUCTION_TOKEN_REGISTER, INSTRUCTION_TOKEN_NONE},              //instruction 1
    {INSTRUCTION_TOKEN_NONE, INSTRUCTION_TOKEN_NONE, INSTRUCTION_TOKEN_NONE},                      //instruction 2
    {INSTRUCTION_TOKEN_REGISTER, INSTRUCTION_TOKEN_REGISTER, INSTRUCTION_TOKEN_NONE},              //instruction 3
    {INSTRUCTION_TOKEN_REGISTER, INSTRUCTION_TOKEN_REGISTER, INSTRUCTION_TOKEN_NONE},              //instruction 4
    {INSTRUCTION_TOKEN_REGISTER, INSTRUCTION_TOKEN_REGISTER, INSTRUCTION_TOKEN_NONE},              //instruction 5
    {INSTRUCTION_TOKEN_REGISTER, INSTRUCTION_TOKEN_REGISTER, INSTRUCTION_TOKEN_NONE},              //instruction 6
    {INSTRUCTION_TOKEN_REGISTER, INSTRUCTION_TOKEN_REGISTER, INSTRUCTION_TOKEN_NONE},              //instruction 7
    {INSTRUCTION_TOKEN_NONE, INSTRUCTION_TOKEN_NONE, INSTRUCTION_TOKEN_NONE},                      //instruction 8
    {INSTRUCTION_TOKEN_REGISTER, INSTRUCTION_TOKEN_NONE, INSTRUCTION_TOKEN_NONE},                  //instruction 9
    {INSTRUCTION_TOKEN_NONE, INSTRUCTION_TOKEN_NONE, INSTRUCTION_TOKEN_NONE},                      //instruction 10
    {INSTRUCTION_TOKEN_NONE, INSTRUCTION_TOKEN_NONE, INSTRUCTION_TOKEN_NONE},                      //instruction 11
    {INSTRUCTION_TOKEN_REGISTER, INSTRUCTION_TOKEN_IMMIDIETE, INSTRUCTION_TOKEN_NONE},             //instruction 12
    {INSTRUCTION_TOKEN_REGISTER, INSTRUCTION_TOKEN_IMMIDIETE, INSTRUCTION_TOKEN_NONE},             //instruction 13
    {INSTRUCTION_TOKEN_REGISTER, INSTRUCTION_TOKEN_IMMIDIETE, INSTRUCTION_TOKEN_NONE},             //instruction 14
    {INSTRUCTION_TOKEN_ADDRESS, INSTRUCTION_TOKEN_NONE, INSTRUCTION_TOKEN_NONE},                   //instruction 15
    {INSTRUCTION_TOKEN_ADDRESS, INSTRUCTION_TOKEN_NONE, INSTRUCTION_TOKEN_NONE},                   //instruction 16
    {INSTRUCTION_TOKEN_ADDRESS, INSTRUCTION_TOKEN_NONE, INSTRUCTION_TOKEN_NONE},                   //instruction 17
    {INSTRUCTION_TOKEN_ADDRESS, INSTRUCTION_TOKEN_NONE, INSTRUCTION_TOKEN_NONE},                   //instruction 18
    {INSTRUCTION_TOKEN_REGISTER, INSTRUCTION_TOKEN_IMMIDIETE, INSTRUCTION_TOKEN_NONE},             //instruction 19
    {INSTRUCTION_TOKEN_REGISTER, INSTRUCTION_TOKEN_REGISTER, INSTRUCTION_TOKEN_NONE},              //instruction 20
    {INSTRUCTION_TOKEN_REGISTER, INSTRUCTION_TOKEN_REGISTER, INSTRUCTION_TOKEN_NONE},              //instruction 21
    {INSTRUCTION_TOKEN_REGISTER, INSTRUCTION_TOKEN_REGISTER, INSTRUCTION_TOKEN_NONE},              //instruction 22
    {INSTRUCTION_TOKEN_REGISTER, INSTRUCTION_TOKEN_REGISTER, INSTRUCTION_TOKEN_REGISTER},          //instruction 23
    {INSTRUCTION_TOKEN_REGISTER, INSTRUCTION_TOKEN_REGISTER, INSTRUCTION_TOKEN_REGISTER},          //instruction 24
    {INSTRUCTION_TOKEN_REGISTER, INSTRUCTION_TOKEN_NONE, INSTRUCTION_TOKEN_NONE},                  //instruction 25
    {INSTRUCTION_TOKEN_REGISTER, INSTRUCTION_TOKEN_NONE, INSTRUCTION_TOKEN_NONE},                  //instruction 26
    {INSTRUCTION_TOKEN_NONE, INSTRUCTION_TOKEN_NONE, INSTRUCTION_TOKEN_NONE},                      //instruction 27
    {INSTRUCTION_TOKEN_NONE, INSTRUCTION_TOKEN_NONE, INSTRUCTION_TOKEN_NONE},                      //instruction 28
    {INSTRUCTION_TOKEN_ADDRESS, INSTRUCTION_TOKEN_NONE, INSTRUCTION_TOKEN_NONE},                   //instruction 29
    {INSTRUCTION_TOKEN_NONE, INSTRUCTION_TOKEN_NONE, INSTRUCTION_TOKEN_NONE},                      //instruction 30
    {INSTRUCTION_TOKEN_NONE, INSTRUCTION_TOKEN_NONE, INSTRUCTION_TOKEN_NONE}                       //instruction 31
};

const int amountOfParamsForInstruction[32] = {
    2,   //instruction 1
    2,   //instruction 2
    0,   //instruction 3
    2,   //instruction 4
    2,   //instruction 5
    2,   //instruction 6
    2,   //instruction 7
    2,   //instruction 8
    0,   //instruction 9
    1,   //instruction 10
    0,   //instruction 11
    0,   //instruction 12
    2,   //instruction 13
    2,   //instruction 14
    2,   //instruction 15
    1,   //instruction 16
    1,   //instruction 17
    1,   //instruction 18
    1,   //instruction 19
    2,   //instruction 20
    2,   //instruction 21
    2,   //instruction 22
    2,   //instruction 23
    3,   //instruction 24
    3,   //instruction 25
    1,   //instruction 26
    1,   //instruction 27
    0,   //instruction 28
    0,   //instruction 29
    1,   //instruction 30
    0,   //instruction 31
    0,   //instruction 32
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
    OPCODE_ADD,
    OPCODE_SUB,
    OPCODE_UNUSED0,
    OPCODE_CMP,
    OPCODE_ADC,
    OPCODE_SBC,
    OPCODE_NAND,
    OPCODE_XOR,
    OPCODE_UNUSED1,
    OPCODE_SHR,
    OPCODE_LSH,
    OPCODE_UNUSED2,
    OPCODE_ADDI,
    OPCODE_SUBI,
    OPCODE_CMPI,
    OPCODE_JMP,
    OPCODE_JZ,
    OPCODE_JN,
    OPCODE_JP,
    OPCODE_LDIR,
    OPCODE_LDRR,
    OPCODE_LDRMR,
    OPCODE_LDMRR,
    OPCODE_LDRMRR,
    OPCODE_LDMRRR,
    OPCODE_PUSH,
    OPCODE_POP,
    OPCODE_SCF,
    OPCODE_RCF,
    OPCODE_CALL,
    OPCODE_RET,
    OPCODE_NOP
};

typedef enum registerIndex {
    REGISTER_A = 0, 
    REGISTER_B = 1,
    REGISTER_C = 2,
    REGISTER_H = 3,
    REGISTER_L = 4,
    REGISTER_PC = 5,
    REGISTER_SP = 6,
    REGISTER_BP = 7,
    NAR = -1
} registerIndex_t;

uint16_t addInstruction(uint16_t reg0, uint16_t reg1, uint16_t unused0) {return (OPCODE_ADD | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS));}
uint16_t subInstruction(uint16_t reg0, uint16_t reg1, uint16_t unused0) {return (OPCODE_SUB | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS));}
uint16_t u0Instruction(uint16_t unused0, uint16_t unused1, uint16_t unused2) {return OPCODE_NOP;}
uint16_t cmpInstruction(uint16_t reg0, uint16_t reg1, uint16_t unused0) {return (OPCODE_CMP | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS));}
uint16_t adcInstruction(uint16_t reg0, uint16_t reg1, uint16_t unused0) {return (OPCODE_ADC | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS));}
uint16_t sbcInstruction(uint16_t reg0, uint16_t reg1, uint16_t unused0) {return (OPCODE_SBC | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS));}
uint16_t nandInstruction(uint16_t reg0, uint16_t reg1, uint16_t unused0) {return (OPCODE_NAND | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS));}
uint16_t xorInstruction(uint16_t reg0, uint16_t reg1, uint16_t unused0) {return (OPCODE_XOR | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS));}
uint16_t u1Instruction(uint16_t unused0, uint16_t unused1, uint16_t unused2) {return OPCODE_NOP;}
uint16_t shrInstruction(uint16_t reg0, uint16_t unused0, uint16_t unused1) {return (OPCODE_SHR | (reg0 << REG0_SHIFT_BITS));}
uint16_t u2Instruction(uint16_t unused0, uint16_t unused1, uint16_t unused2) {return OPCODE_NOP;}
uint16_t u3Instruction(uint16_t unused0, uint16_t unused1, uint16_t unused2) {return OPCODE_NOP;}
uint16_t addiInstruction(uint16_t reg0, uint16_t immidiete, uint16_t unused) {return OPCODE_ADDI | (reg0 << REG0_SHIFT_BITS) | (immidiete && 0b11111111);}
uint16_t subiInstruction(uint16_t reg0, uint16_t immidiete, uint16_t unused) {return OPCODE_SUBI | (reg0 << REG0_SHIFT_BITS) | (immidiete && 0b11111111);}
uint16_t cmpiInstruction(uint16_t reg0, uint16_t immidiete, uint16_t unused) {return OPCODE_CMPI | (reg0 << REG0_SHIFT_BITS) | (immidiete && 0b11111111);}
uint16_t jmpInstruction(uint16_t address, uint16_t unused0, uint16_t unused1) {return (OPCODE_JMP | (address & 0b0000011111111111));}
uint16_t jzInstruction(uint16_t address, uint16_t unused0, uint16_t unused1) {return (OPCODE_JZ | (address & 0b0000011111111111));}
uint16_t jnInstruction(uint16_t address, uint16_t unused0, uint16_t unused1) {return (OPCODE_JN | (address & 0b0000011111111111));}
uint16_t jpInstruction(uint16_t address, uint16_t unused0, uint16_t unused1) {return (OPCODE_JP | (address & 0b0000011111111111));}
uint16_t ldirInstruction(uint16_t reg0, uint16_t immidiete, uint16_t unused0) {return (OPCODE_LDIR | (reg0 << REG0_SHIFT_BITS) | ((immidiete & 0b11111111) << REG1_SHIFT_BITS));}
uint16_t ldrrInstruction(uint16_t reg0, uint16_t reg1, uint16_t unused0) {return (OPCODE_LDRR | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS));}
uint16_t ldrmrInstruction(uint16_t reg0, uint16_t reg1, uint16_t unused0) {return (OPCODE_LDRR | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS));}
uint16_t ldmrrInstruction(uint16_t reg0, uint16_t reg1, uint16_t unused0) {return (OPCODE_LDRR | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS));}
uint16_t ldrmrrInstruction(uint16_t reg0, uint16_t reg1, uint16_t reg2) {return (OPCODE_LDRMRR | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS) | (reg2 << REG2_SHIFT_BITS));}
uint16_t ldmrrrInstruction(uint16_t reg0, uint16_t reg1, uint16_t reg2) {return (OPCODE_LDMRRR | (reg0 << REG0_SHIFT_BITS) | (reg1 << REG1_SHIFT_BITS) | (reg2 << REG2_SHIFT_BITS));}
uint16_t pushInstruction(uint16_t reg0, uint16_t unused0, uint16_t unused1) {return (OPCODE_PUSH | (reg0 << REG0_SHIFT_BITS));}
uint16_t popInstruction(uint16_t reg0, uint16_t unused0, uint16_t unused1) {return (OPCODE_POP | (reg0 << REG0_SHIFT_BITS));}
uint16_t scfInstruction(uint16_t unused0, uint16_t unused1, uint16_t unused2) {return OPCODE_SCF;}
uint16_t rcfInstruction(uint16_t unused0, uint16_t unused1, uint16_t unused2) {return OPCODE_RCF;}
uint16_t callInstruction(uint16_t address, uint16_t unused0, uint16_t unused1) {return OPCODE_CALL | (address & 0b11111111111);}
uint16_t retInstruction(uint16_t unused0, uint16_t unused1, uint16_t unused2) {return OPCODE_RET;}
uint16_t nopInstruction(uint16_t unused0, uint16_t unused1, uint16_t unused2) {return OPCODE_NOP;}

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
    const registerIndex_t registerIndexes[] = {
        REGISTER_A,
        REGISTER_B,
        REGISTER_C,
        REGISTER_H,
        REGISTER_L,
        REGISTER_PC,
        REGISTER_SP,
        REGISTER_BP
    };
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
    for(int i = 0; i < INSTRUCTION_COUNT; i++) {
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