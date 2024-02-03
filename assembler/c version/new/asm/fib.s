ldir a, 0
ldir b, 1
ldir l, 0
ldir h, 100
ldir bp, 255
cmp l, h
jz 13
ldrr c, a
add c, b
ldrr b, c
addi l, 1
ldmrr [ bp ], c
jmp 5
jmp 13