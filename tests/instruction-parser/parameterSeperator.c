#include "../../include/assembleyInstructionConversion.h"
#include <stdio.h>
#include <string.h>

int main() {
    char* code = calloc(512, sizeof(char));
    strncpy(code, "ADD A, B", 512);

    asmParamsResult_t res = getParametersFromAsembly(code);
    
    printf("Parameters for \"%s\":\n", code);
    
    for(int i = 0; i < res.amountOfParams; i++) {
        printf("\tParam %i: \"%s\"\n", i, res.params[i]);
    }
    return 0;
}