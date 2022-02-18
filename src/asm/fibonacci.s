; Calculate and store first n (stored in R1) Fibonacci numbers at memory location m (stored in R2)
LAX #10
STX R1

LAX #0
STX R2

ADD R3 R0 R0
ADD R4 R0 #1

ST R0[R2] R3
ADD R2 R2 #1
ST R0[R2] R4
ADD R2 R2 #1

fibonacci:
BEQZ R1 exit
ADD R5 R3 R4
ADD R3 R0 R4
ADD R4 R0 R5
ST R0[R2] R4
ADD R2 R2 #1
SUB R1 R1 #1
JMP fibonacci

exit:
HLT