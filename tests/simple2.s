START:
MOV r0, 1     
MOV r1, 0     
LCULATE:
ADD r2, r0, r1  
MOV r1, r0      
MOV r2, r1      
MOV out, r0     
PRINT out       
SUBI r3, 1      
BNEZ r3, CALCULATE  
HALT            