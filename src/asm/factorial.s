; R1 stores the number to find factorial of
LAX #10
STX R1

; R2 stores the output of factorial function
ADD R2 R0 #1
factorial:
BEQZ R1 exit
MUL R2 R2 R1
SUB R1 R1 #1
JMP factorial

exit:
HLT
