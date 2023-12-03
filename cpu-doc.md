# CPU design

## Basic overview

### Registers
Idx | Reg | Bits | Function |
----|-----|------|----------|
0   | A   | 8    | General purpose/accumulator
1   | B   | 8    | General purpose
2   | C   | 8    | General purpose
3   | H   | 8    | High bits of 16 bit general purpose reg HL
4   | L   | 8    | Low bits of HL
5   | PC  | 16   | Program counter
6   | SP  | 8    | Stack pointer
7   | BP  | 8    | Base pointer


### Instruction set

#### Guidelines for laying out instructions
The instructions are laid out in a specific manner to make instruction decoding easier.
- if the MSB is 0 then the instruction is an ALU instruction
    - If the 2nd MSB is 0, then the instruction operates on two registers, while if it's 1 then it operates on a register and an immediate.
        - Some instructions (SHR) don't use the 2nd argument
    - The remaining 3 least significant bits represent the ALU operation performed
- If the first 3 most significant bits are 100 then the instruction is a jump instruction. in that case the last two bits are the condition on which to jump.

General guidelines
- Keep related instructions in blocks, e.g. in the range 10000-10111 not 10010-11110, so that it's easy to tell what instructions are what just by looking at one or two bits 

#### ISA table
Op    | Name   | Usage                               | Description                                                                |
------|--------|-------------------------------------|----------------------------------------------------------------------------|
00000 | ADD    | ADD  <REG 1>, <REG 2>               | Adds <REG 2> to <REG 1>
00001 | SUB    | SUB  <REG 1>, <REG 2>               | Subtracts <REG 2> from <REG 1>
00010 | SHR    | SHR  <REG>                          | Logical bit shift right <REG> by one
00011 | CMP    | CMP  <REG 1>, <REG 2>               | Subtracts <REG 2> from <REG 1>, sets flags, and discards the result
00100 | ADC    | ADC  <REG 1>, <REG 2>               | Adds <REG 2> and carry flag to <REG 1>
00101 | SBC    | SBC  <REG 1>, <REG 2>               | Subtracts <REG 2> and the carry flag from <REG 1>
00110 | NAND   | NAND <REG 1>, <REG 2>               | NANDs <REG 1> with <REG 2> 
00111 | XOR    | XOR  <REG 1>, <REG 2>               | XORs <REG 1> with <REG 2>
01000 | ADDI   | ADD  <REG>, <Immediate>             | Adds <Immediate> to <REG>
01001 | SUBI   | SUB  <REG>, <Immediate>             | Subtracts <Immediate> from <REG>
01010 | unused (SHRI doesn't make sense)
01011 | CMPI   | CMP   <REG>, <Immediate>            | Subtracts <Immediate> from <REG 1>, sets flags, and discards the result
01100 | ADCI   | ADCI  <REG>, <Immediate>            | Adds <Immediate> plus the carry flag to <REG> 
01101 | SBCI   | SBCI  <REG>, <Immediate>            | Subtracts <Immediate> plus the carry flat from <REG>
01110 | NANDI  | NANDI <REG>, <Immediate>            | NANDs <REG> with <Immediate>
01111 | XORI   | XORI  <REG>, <Immediate>            | XORs <REG> with <Immediate>
10000 | JMP    | JMP  <Address>                      | Jumps to <Address>
10001 | JZ     | JZ   <Address>                      | Jumps to <Address> if zero flag is set
10010 | JN     | JN   <Address>                      | Jumps to <Address> if negative flag is set
10011 | JP     | JP   <Address>                      | Jumps to <Address> if positive flag is set
10100 | LDIR   | LD   <REG>, <Immediate>             | Loads <Immediate> into <REG>
10101 | LDRR   | LD   <REG 1>, <REG 2>               | Loads <REG 2> in to <REG 1>
10110 | LDRMR  | LD   <REG 1>, [<REG 2>]             | Loads the value of memory at location <REG 2> in to <REG 1>
10111 | LDMRR  | LD   [<REG 1>], <REG 2>             | Loads the value of <REG 2> into the bytepointer <REG 1>
11000 | LDRMRR*| LD   <REG 1>, [<REG 2> + <REG 3>]   | Loads the value of memory at the sum of <REG 2> and <REG 3> in to <REG 1>
11001 | LDMRRR*| LD   [<REG 1> + <REG 2>], <REG 3>   | Loads the value of <REG 1> into memory at the sum of <REG 2> and <REG 3> 
11010 | PUSH   | PUSH <REG>                          | Pushes <REG> on to the stack
11011 | POP    | POP  <REG>                          | Pops <REG> off the stack
11100 | SCF    | SCF                                 | Sets the carry flag
11101 | RCF    | RCF                                 | Resets the carry flag
11110 | CALL   | CALL <Address>                      | Push PC, jump to <Address>
<N/A> | RET    | RET                                 | Alias for pop PC (Not implemented in hardware)
11111 | NOP    | NOP                                 | does literally nothing

(\[<REG>\] means "value at memory address <REG>")
*don't actually build these but they're here in case

#### Possible ways to reduce complexity
- hard wire register A to some functions, e.g. first or second operand of ALU instructions, or destination of ALU instructions
- get rid of the more complex addressing modes (LDRMRR, LDMRRR)

### Instruction Encoding

5 bits for opcode \\
then either: \\
    3 bits for source register and 3 bits for destination \\
or \\ 
    11 bits for relative jump address \\
or \\
    3 bits for register, 3 bits unused and 8 bits for immediate value

```
00000 xxx xxx xxxxx -> NOP
00001 001 010 xxxxx -> ADD A, B
  |    |   |  
  |    |   \-- Reg B
  |    \------ Reg A
  \----------- ADD opcode
```

```
01001 111 11111001
  |    |   |
  |    \---\-- -7 words, two's compliment
  \----------- JZ opcode
```

```
01010 001 xxx 01010101 -> LD b, 85
  |    |       |      
  |    |       |
  |    |       \------- numeber 85
  |    \--------------- Register B
  \-------------------- LDIR opcode
```

## TODO
- Figure out how HL as a pair could be used
