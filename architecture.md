# CPU Architecture

-   Number of registers = 16 _i.e._, `R0 R1 ... R15`

-   Size of registers = 16 bits

-   CPU Architecture = Von Neumann (16-bit)

-   Memory type = Byte Addressable

-   Memory size = 2<sup>16</sup> = 65536 bytes

## Instruction set

| Instruction       | Result                        |
| ----------------- | ----------------------------- |
| `ADD R1, R2, R3`  | `R1 = R2 + R3 `               |
| `SUB R1, R2, R3 ` | `R1 = R2 - R3 `               |
| `MUL R1, R2, R3 ` | `R1 = R2 * R3 `               |
| `ADD R1, R0, #5 ` | `assigns R1 = 5 `             |
| `LD R1, A[R1] `   | `R1= [R1+A] `                 |
| `ST A[R1], R1 `   | `[R1+A] = R1 `                |
| `JMP L1 `         | `Jump to L1 label `           |
| `BEQZ R1, L1 `    | `Jump to L1 label if R1 = 0` |
| `HLT `            | `Halt the computation `       |
