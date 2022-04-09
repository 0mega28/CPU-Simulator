ADD R6 R0 #9
; R6 = 4
ADD R2 R0 #5
; R2 = 5
MUL R1 R2 R6
; R1 = 4 * 5 = 20
SUB R8 R6 R2
; R8 = 4 - 5 = -1
MUL R10 R1 R6
; R10 = 20 * 4 = 80
ADD R6 R8 R2
; R6 = -1 + 5 = 4
HLT