

        LDIR BP, 255                       
                    LDRR SP, BP 
                     LDIR A, 1                          
    LDIR B, 1  
        LDIR C, 0     
        LDRR C, B
   ADD C, A
 
      PUSH C
      LDRR B, C
JMP 6