# CPU Architecture

-   Registers

    -   General Purpose Registers = 16 _i.e._, `R0 R1 ... R15`
    -   Special Purpose Register Accumulator `AX`

-   Size of registers = 16 bits

-   CPU Architecture = Von Neumann (16-bit)

-   Memory type = Byte Addressable

-   Memory size = 2<sup>16</sup> = 65536 bytes

-   Memory layout is Big Endian

## Instruction set

| Instruction       | Result                       |
| ----------------- | ---------------------------- |
| `ADD R1, R2, R3`  | `R1 = R2 + R3 `              |
| `SUB R1, R2, R3 ` | `R1 = R2 - R3 `              |
| `MUL R1, R2, R3 ` | `R1 = R2 * R3 `              |
| `ADD R1, R0, #5 ` | `assigns R1 = 5 `            |
| `LD R1, R2[R1] `  | `R1 = [R1+R2] `              |
| `ST R2[R1], R1 `  | `[R1+R2] = R1 `              |
| `LAX #512 `       | `assigns AX = 512 `          |
| `STX R2 `         | `R2 = AX `                   |
| `JMP L1 `         | `Jump to L1 label `          |
| `BEQZ R1, L1 `    | `Jump to L1 label if R1 = 0` |
| `HLT `            | `Halt the computation `      |

## Opcode

| Opcode | bin    |
| ------ | ------ |
| `ADD`  | `0000` |
| `ADDI` | `0001` |
| `SUB`  | `0010` |
| `SUBI` | `0011` |
| `MUL`  | `0100` |
| `MULI` | `0101` |
| `LD`   | `0110` |
| `ST`   | `0111` |
| `JMP`  | `1000` |
| `BEQZ` | `1001` |
| `HLT`  | `1010` |
| `LAX`  | `1011` |
| `STX`  | `1100` |
