START:
    MOV r0, 1     /* Initialize r0 with 1 */
    MOV r1, 0     /* Initialize r1 with 0 */

CALCULATE:
    ADD r2, r0, r1  /* r2 = r0 + r1 */
    MOV r1, r0      /* r0 = r1 */
    MOV r2, r1      /* r1 = r2 */

    MOV out, r0     /* Store r0 (current Fibonacci number) in out */

    PRINT out       /* Print the current Fibonacci number*/








    SUBI r3, 1      /* Decrement a counter (r3)
    BNEZ r3, CALCULATE  /* Jump back to CALCULATE if r3 is not zero       o*/

    HALT            /* Halt the program*/