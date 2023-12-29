#include "../../include/assembleyInstructionConversion.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>


int main() {
    char *code = calloc(512, sizeof(char));
    //strncpy(code, "LdIr a, 255", 512);
    strncpy(code, "PUSH PC", 512);
    
    asmParamsResult_t instruction = getParametersFromAsembly(code);
    
    instructionToken_t instruction_tok = getOpcode(instruction.params[0]);
    if(instruction_tok == NAI) {
        printf("Instruction \"%s\" doesnt exist.\n", instruction.params[0]);
        return -1;
    }

    printf("Instruction token = %d\n", instruction_tok);

    uint16_t args[3] = {0};

    for(int i = 0; i < instruction.amountOfParams-1; i++) {
        printf("Patterntype for argument %d (\"%s\") is %d \n", i, instruction.params[i+1], (int)instructionPattern[instruction_tok][i]);
        switch(instructionPattern[instruction_tok][i]) {
            case INSTRUCTION_TOKEN_REGISTER:
                if(getRegisterIndex(instruction.params[i+1]) != NAR) {
                    printf("Register at arg %d validated.\n", i);
                    args[i] = getRegisterIndex(instruction.params[i+1]);
                }
                else {printf("Register at arg %d not validated, because register doesnt exists\n", i); return -1;}
                break;

            case INSTRUCTION_TOKEN_IMMIDIETE:
                if(isNumber(instruction.params[i+1])) {
                    if(atoi(instruction.params[i+1]) <= 0b11111111) {
                        printf("Immidiete at arg %d validated.\n", i);
                        args[i] = (uint16_t)atoi(instruction.params[i+1]);
                    } else {printf("Immidiete at arg %d not validated, because number is to big (exeeds 8bit intiger limit).\n", i); return -1;}
                } else {printf("Immidiete at arg %d not validated, because arguments isnt a number.\n", i); return -1;}
                break;

            case INSTRUCTION_TOKEN_ADDRESS:
                if(isNumber(instruction.params[i+1])) {
                    if(atoi(instruction.params[i+1]) <= 0b11111111111) {
                        printf("Offset at arg %d validated.", i);
                        args[i] = atoi(instruction.params[i+1]);
                    } else {printf("Offset at arg %d not validated, because number is to big (exeeds 8bit intiger limit).\n", i); return -1;}
                } else {printf("Offset at arg %d not validated, because arguments isnt a number.\n", i); return -1;}
                break;
        }
    }
    printf("Parameter values for instruction No. %d are: %d, %d, %d\n", (int)instruction_tok, args[0], args[1], args[2]);
    printf("Finished instruction binary: 0x%X\n", assembleInstructions[(int)instruction_tok](args[0], args[1], args[2]));
    
    return 0;
}