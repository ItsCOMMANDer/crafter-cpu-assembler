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

void freeStringArray(struct stringArray array) {
    if(array.strings == NULL) return;
    for(int i = 0; i < array.amountOfStrings; i++) {
        if(array.strings[i] == NULL) continue;
        free(array.strings[i]);
    }
}



int64_t max(int64_t a, int64_t b) {return a > b ? a : b;} //how is this not a function in math.h?
int64_t min(int64_t a, int64_t b) {return a < b ? a : b;} //how is this not a function in math.h too?

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

char* getParamType(char* dest, enum instruction_token token) {
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

    char* asmString = calloc(asmLength + 1, sizeof(char));
    strncpy(asmString, instructions[index] + skip, asmLength);
    ret.instruction_name = calloc(sizeof(char), asmLength);
    strncpy(ret.instruction_name, asmString, asmLength);
    free(asmString);

    skip+=asmLength;
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

uint16_t getOpCode(const char* insturction_name) {
    for(int i = 0; i < 32; i++) {
        struct instruction_info info = getInstructionInfo(i);
        if(_stricmp(insturction_name, info.instruction_name) == 0) {
            free(info.instruction_name);
            return info.opcode;
        }
        free(info.instruction_name);
    }
    return (uint16_t)-1;
}

struct stringArray getParams(const char* assembly) {
    struct stringArray res = {0};
    bool newParamHasBegan = false;
    int charsPerParam = 0;
    int paramIndex = 0;
    int i;
    int brackets = 0;

    int skip = 0;

    size_t asmLength = strlen(assembly);
    size_t asmParamLength = 0;

    for(int i = 0; i < asmLength; i++) {
        if(assembly[i] != ' ' && assembly[i] != '\t' && assembly[i] != ',') {newParamHasBegan = true;}

        if(newParamHasBegan) {
            if(assembly[i] == '[') {brackets++;}
            if(assembly[i] == ']') {brackets--;}

            if((assembly[i] == ' ' || assembly[i] == '\t' || assembly[i] == ',' || assembly[i] == '\0') && brackets == 0) {
                char** previousStringArray = res.strings;

                res.strings = calloc(res.amountOfStrings + 1, sizeof(char*));

                memcpy(res.strings, previousStringArray, res.amountOfStrings * sizeof(char*));

                res.strings[res.amountOfStrings] = calloc(asmParamLength, sizeof(char));
                
                strncpy(res.strings[res.amountOfStrings], assembly + skip, asmParamLength);

                skip += asmParamLength + 1;
                asmParamLength = 0;
                newParamHasBegan = false;
                res.amountOfStrings++;
            } else { asmParamLength++;}
        } else {skip++;}
    }

    char** previousStringArray = res.strings;

    res.strings = calloc(res.amountOfStrings + 1, sizeof(char*));

    memcpy(res.strings, previousStringArray, res.amountOfStrings * sizeof(char*));

    res.strings[res.amountOfStrings] = calloc(asmParamLength + 1, sizeof(char));
    
    strncpy(res.strings[res.amountOfStrings], assembly + skip, asmParamLength);

    res.amountOfStrings++;

    return res;
}



void printInstructionData(void);

int main(int argc, char** argv) {

    if(argc <= 1) return -1;
    FILE* source_file = fopen(argv[1], "rb");
    if(source_file == NULL) {
        printf("Error opening file.\n");
        return -1;
    }

    fseek(source_file, 0, SEEK_END);
    uint64_t fileLength = ftell(source_file);
    fseek(source_file, 0, SEEK_SET);
    if (fileLength == 0) {
        printf("File is empty.\n");
        fclose(source_file);
        return -1;
    }

    char* codeBuffer = calloc(fileLength, sizeof(char));

    fread(codeBuffer, fileLength, sizeof(char), source_file);

    struct stringArray code = { 0 };

    uint64_t characterInLine = 0;
    uint64_t skip = 0;

    for(uint64_t i = 0; i < fileLength; i++) {
        if(codeBuffer[i] == '\n') {
            //
            char** previousStringArray = code.strings;

            code.strings = calloc(code.amountOfStrings + 1, sizeof(char*));

            memcpy(code.strings, previousStringArray, code.amountOfStrings * sizeof(char*));

            code.strings[code.amountOfStrings] = calloc(characterInLine, sizeof(char));

            strncpy(code.strings[code.amountOfStrings], codeBuffer + skip, characterInLine - 1);

            skip += characterInLine + 1;
            characterInLine = 0;
            code.amountOfStrings++;

        } else {
            characterInLine++;
        }
    }

    char** previousStringArray = code.strings;

    code.strings = calloc(code.amountOfStrings + 1, sizeof(char*));

    memcpy(code.strings, previousStringArray, code.amountOfStrings * sizeof(char*));

    code.strings[code.amountOfStrings] = calloc(characterInLine + 1, sizeof(char));

    strncpy(code.strings[code.amountOfStrings], codeBuffer + skip, characterInLine);

    code.amountOfStrings++;


    
    fclose(source_file);
    free(codeBuffer);

    for(int i = 0; i < code.amountOfStrings; i++) {
        struct stringArray params = getParams(code.strings[i]);
        
        uint16_t opcode = getOpCode(params.strings[0]);

        printf("Instruction \"%s\" has opcode of %d\n", params.strings[0], opcode);

        freeStringArray(params);
    }

    freeStringArray(code);

    return 42;
}


void printInstructionData(void) {
    for(int i = 0; i < 32; i++) {
        struct instruction_info ins = getInstructionInfo(i);
        if(!ins.valid) {
            printf("Unused/Invalid formatting\n");
            continue;
        }
        printf("ASM: '%5s'\tOpcode: %2d\tAmount of params: %2d\t", ins.instruction_name, ins.opcode, ins.amountOfParams);
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