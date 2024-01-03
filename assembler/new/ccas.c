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

struct instruction_info{
    char* instruction_name;
    uint8_t opcode;
    uint8_t amountOfParams;
    enum instruction_token params[3];
    bool valid;
};




bool isDigit(char c) {return (c >= '0' && c <= '9');}
bool isNumber(char* c) {
    if(c == NULL) return false;
    for(int i = 0; i < strlen(c); i++) {if(!isDigit(c[i])) return false;}
    return true;
}


const char instructions[32][32] = {
    "13ADD2RR",
    "13SUB2RR",
    "13SHR1R",
    "13CMP2RR",
    "13ADC2RR",
    "13SBC2RR",
    "14NAND2RR",
    "13XOR2RR",
    "14ADDI2RI",
    "14SUBI2RI",
    "0Unused0",
    "14CMPI2RI",
    "14ADCI2RI",
    "14SBCI2RI",
    "15NANDI2IR",
    "14XORI2IR",
    "13JMP1A",
    "12JZ1A",
    "12JN1A",
    "12JP1A",
    "14LDIR2RI",
    "14LDRR2RR",
    "15LDRMR2RP",
    "15LDMRR2PR",
    "0Unused0",
    "0Unused0",
    "14PUSH1R",
    "13POP1R",
    "13SCF0",
    "13RCF0",
    "14CALL1A",
    "13NOP0",
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
    int skip = 1;
    if(isDigit(instructions[index][0])) asmLength = instructions[index][0] - '0';
    else {
        ret.valid = false;
        ret.instruction_name = calloc(1, sizeof(char));
        return ret;
    }
    if(asmLength <= 0) {
        ret.valid = false;
        ret.instruction_name = calloc(1, sizeof(char));
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

int main(int argc, char** argv) {
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

    free(buffer);
    return 42;
}



/*
int test(void) {
    for(int i = 0; i < 32; i++) {
        int asmLength;
        int skip = 1;
        if(isDigit(instructions[i][0])) asmLength = instructions[i][0] - '0';
        else {
            printf("Format Error.\n");
            return -1;
        }
        if(asmLength <= 0) {
            printf("Unused\n");
            continue;
        }
        char* asmString = "";
        strncpy(asmString, instructions[i] + skip, asmLength);
        skip+=asmLength;
        int asmStringLength = atoi(asmString);
        strncpy(asmString, instructions[i] + skip, asmStringLength);
        skip+=asmStringLength;
        printf("AMS: %6s\t", asmString); // instruction name
        memset(asmString, 0, asmStringLength);
        int amountOfParams = instructions[i][skip] - '0';
        skip++;
        printf("Amount of params: %d\t", amountOfParams);
        enum instruction_token params[3] = {0}; 
        for(int j = 0; j < amountOfParams; j++) {
            printf("%c:", instructions[i][skip]);
            switch(instructions[i][skip++]) {
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
        printf("\n");
    }
}
*/