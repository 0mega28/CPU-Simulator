; Calculate and store first n (stored in R1) Fibonacci numbers at memory location m (stored in R2)
ADD R1 R0 #10

ADD R2 R0 #512

ADD R3 R0 R0
ADD R4 R0 #1

ST R0[R2] R3
ADD R2 R2 #2
ST R0[R2] R4
ADD R2 R2 #2

fibonacci:
BEQZ R1 exit
ADD R5 R3 R4
ADD R3 R0 R4
ADD R4 R0 R5
ST R0[R2] R4
ADD R2 R2 #2
JMP fibonacci

exit:
HLT