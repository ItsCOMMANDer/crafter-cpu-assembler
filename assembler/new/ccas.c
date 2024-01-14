#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include <math.h>

enum instruction_token {
    INSTRUCTIONTOKEN_IMMIDIATE = 0,
    INSTRUCTIONTOKEN_REGISTER = 1,
    INSTRUCTIONTOKEN_ADRESS = 2,
    INSTRUCTIONTOKEN_POINTER = 3,
    INSTRUCTIONTOKEN_INVALID = -1, 
};

struct instruction_info {
    char* instruction_name;
    uint8_t opcode;
    uint8_t amountOfParams;
    enum instruction_token params[3];
    bool valid;
};

struct stringArray {
    char** strings;
    int amountOfStrings;
};



bool isDigit(char c) {return (c >= '0' && c <= '9');}
bool isNumber(char* c) {
    if(c == NULL) return false;
    for(int i = 0; i < strlen(c); i++) {if(!isDigit(c[i])) return false;}
    return true;
}


const char instructions[32][32] = {
    "03ADD2RR",
    "03SUB2RR",
    "03SHR1R",
    "03CMP2RR",
    "03ADC2RR",
    "03SBC2RR",
    "04NAND2RR",
    "03XOR2RR",
    "04ADDI2RI",
    "04SUBI2RI",
    "00Unused0",
    "04CMPI2RI",
    "04ADCI2RI",
    "04SBCI2RI",
    "05NANDI2IR",
    "04XORI2IR",
    "03JMP1A",
    "02JZ1A",
    "02JN1A",
    "02JP1A",
    "04LDIR2RI",
    "04LDRR2RR",
    "05LDRMR2RP",
    "05LDMRR2PR",
    "00Unused0",
    "00Unused0",
    "04PUSH1R",
    "03POP1R",
    "03SCF0",
    "03RCF0",
    "04CALL1A",
    "03NOP0",
};

char* getParamType( char* dest, enum instruction_token token) {
    switch (token) {
        case INSTRUCTIONTOKEN_IMMIDIATE:
            strncpy(dest, "INSTRUCTIONTOKEN_IMMIDIATE", 26);
            break;
        case INSTRUCTIONTOKEN_REGISTER:
            strncpy(dest, "INSTRUCTIONTOKEN_REGISTER", 25);
            break;
        case INSTRUCTIONTOKEN_ADRESS:
            strncpy(dest, "INSTRUCTIONTOKEN_ADRESS", 23);
            break;
        case INSTRUCTIONTOKEN_POINTER:
            strncpy(dest, "INSTRUCTIONTOKEN_POINTER", 24);
            break;
        default:
            strncpy(dest, "INSTRUCTIONTOKEN_INVALID", 24);
            break;
    }
    return dest;
}


struct instruction_info getInstructionInfo(uint8_t index) {
    struct instruction_info ret = {0};
    ret.valid = true;
    ret.opcode = index;
    int asmLength;
    int skip = 2;
    if(isDigit(instructions[index][0]) && isDigit(instructions[index][1])) {
        char tmpChr[3] = { instructions[index][0], instructions[index][1], '\0'};
        asmLength = atoi(tmpChr);
    }
    else {
        ret.valid = false;
        return ret;        
    }
    if(asmLength <= 0) {
        ret.valid = false;
        return ret;        
    }
    char* asmString = "";
    strncpy(asmString, instructions[index] + skip, asmLength);
    skip+=asmLength;
    int asmStringLength = atoi(asmString);
    strncpy(asmString, instructions[index] + skip, asmStringLength);
    skip+=asmStringLength;
    ret.instruction_name = calloc(sizeof(char), asmStringLength + 1);
    strncpy(ret.instruction_name, asmString, asmStringLength);
    int amountOfParams = instructions[index][skip] - '0';
    ret.amountOfParams = amountOfParams;
    skip++;
    enum instruction_token params[3] = {0}; 
    for(int j = 0; j < amountOfParams; j++) {
        switch(instructions[index][skip++]) {
            case 'I':
                params[j] = INSTRUCTIONTOKEN_IMMIDIATE;
                break;
            case 'R':
                params[j] = INSTRUCTIONTOKEN_REGISTER;
                break;
            case 'A':
                params[j] = INSTRUCTIONTOKEN_ADRESS;
                break;
            case 'P':
                params[j] = INSTRUCTIONTOKEN_POINTER;
                break;
            default:
                params[j] = INSTRUCTIONTOKEN_INVALID;
                break;
        }
    }
    
    memcpy(ret.params, params, sizeof(params));
    return ret;
}

uint16_t getOpCode(char* insturction_name) {
    for(int i = 0; i < 32; i++) {
        struct instruction_info info = getInstructionInfo(i);
        if(_stricmp(insturction_name, info.instruction_name) == 0) {
            free(info.instruction_name);
            return info.opcode;
        }
        free(info.instruction_name);
    }
    return -1;
}

struct stringArray getParams(char* assembly) {
    struct stringArray res = {0};
    bool newParamHasBegan = false;
    int charsPerParam = 0;
    int paramIndex = 0;
    int i;
    int brackets = 0;


    for(i = 0; i < strlen(assembly); i++) {
        if(assembly[i] == '[') brackets++;
        if(assembly[i] == ']') brackets--;
        if(assembly[i] != ',' && assembly[i] != ' ' && newParamHasBegan == false) {
            newParamHasBegan = true;
        }
        if((assembly[i] == ',' || assembly[i] == ' ') && newParamHasBegan == true && brackets == 0) {
            newParamHasBegan = false;
            res.amountOfStrings++;
        }
    }
    res.amountOfStrings++;
    char** paramPointer = (char**)calloc(res.amountOfStrings, sizeof(char*));
    brackets = 0;
    
    newParamHasBegan = false;

    for(i = 0; i < strlen(assembly); i++) {
        if(assembly[i] == '[') brackets++;
        if(assembly[i] == ']') brackets--;
        if(assembly[i] != ',' && assembly[i] != ' ' && newParamHasBegan == false) newParamHasBegan = true;
        
        if((assembly[i] == ',' || assembly[i] == ' ') && newParamHasBegan == true && brackets == 0) {
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

    res.strings = paramPointer;
    return res;
}



void printInstructionData(void);

int main(int argc, char** argv) {

    char* string = "Int [80, f] , ff f";

    struct stringArray t = getParams(string);

    printf("%d\n", t.amountOfStrings);
    printf("%s\n", t.strings[0]);
    printf("%s\n", t.strings[1]);
    printf("%s\n", t.strings[2]);
    printf("%s\n", t.strings[3]);

    return 0;

    if(argc <= 1) return -1;
    FILE* source_file = fopen(argv[1], "rb");
    if(source_file == NULL) {
        printf("Error opening file.\n");
        return -1;
    }

    fseek(source_file, 0, SEEK_END);
    long long fileLength = ftell(source_file);
    if (fileLength == 0) {
        printf("File is empty.\n");
        fclose(source_file);
        return -1;
    }

    char *buffer = calloc(fileLength + 1, sizeof(char));
    fseek(source_file, 0, SEEK_SET);
    fread(buffer, sizeof(char), fileLength, source_file);
    fclose(source_file);

    uint64_t lineCount = 1;
    int maxCharCount = 0;
    int charCount = 0;

    for(uint64_t i = 0; i < fileLength + 1; i++) {
        if(buffer[i] == '\n' || buffer[i] == '\0') {
            if(buffer[i] != '\0') lineCount++;
            maxCharCount = max(maxCharCount,charCount);
            charCount = 0;
        } else {
            charCount++;
        }
    }

    printf("\nLine Count: %llu\nLongest Line: %d\n", lineCount, maxCharCount);
    
    char** code = calloc(lineCount, sizeof(char*));
    
    int index = 0;
    int offset = 0;

    for (int i = 0; i < lineCount; i++) {
        code[i] = calloc(maxCharCount + 1, sizeof(char));
        sscanf(buffer + offset, "%[^\n]%n", code[i], &index); //idk how the "%[^\n]%n" tbh
        offset += index + 1;
    }

    //CONTINUE
    for(int i = 0; i < lineCount; i++) {printf("%s\n", code[i]);}

    for(int i = 0; i < lineCount; i++) {
        free(code[i]);
    }
    free(code);
    
    return 42;
}


void printInstructionData(void) {
    for(int i = 0; i < 32; i++) {
        struct instruction_info ins = getInstructionInfo(i);
        if(!ins.valid) {
            printf("Unused/Invalid formatting\n");
            continue;
        }
        printf("AMS: %5s\tOpcode: %2d\tAmount of params: %2d\t", ins.instruction_name, ins.opcode, ins.amountOfParams);
        free(ins.instruction_name);
        for(int j = 0; j < ins.amountOfParams; j++) {
            switch(ins.params[j]) {
                case INSTRUCTIONTOKEN_ADRESS:
                    printf("A");
                    break;
                case INSTRUCTIONTOKEN_IMMIDIATE:
                    printf("I");
                    break;
                case INSTRUCTIONTOKEN_POINTER:
                    printf("P");
                    break;
                case INSTRUCTIONTOKEN_REGISTER:
                    printf("R");
                    break;
            }
            if(j < ins.amountOfParams - 1) printf(":");
        }
        printf("\n");
    }
}