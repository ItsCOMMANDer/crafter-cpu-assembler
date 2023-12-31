#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <stdbool.h>


const char instructions[][32] = {
    "ADD 2RR",
    "SUB 2RR",
    "SHR 1R",
    "CMP 2RR",
    "ADC 2RR",
    "SBC 2RR",
    "NAND 2RR",
    "XOR 2RR",
    "ADDI 2RI",
    "SUBI 2RI",
    "Unused 0",
    "CMPI 2RI",
    "ADCI 2RI" 
    "SBCI 2RI",
    "NANDI 2IR",
    "XORI 2IR",
    "JMP 1A",
    "JZ 1A",
    "JN 1A",
    "JP 1A",
    "LDIR 2RI",
    "LDRR 2RR",
    "LDRMR 2RP",
    "LDMRR 2PR",
    "Unused 0",
    "Unused 0",
    "PUSH 1R",
    "POP 1R",
    "SCF 0",
    "RCF 0",
    "CALL 1A",
    "NOP 0",
};







int main() {
    printf("Hello, World!\n");
    return 42;
}