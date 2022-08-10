# CENOS: The Modern CPU Simulator

CENOS is a Central Processing Unit (CPU) Simulator written in C++ which attempts to simulate Out-Of-Order (O3) execution using the ['Scoreboarding Algorithm'](https://en.wikipedia.org/wiki/Scoreboarding) equipped with a 2-bit Branch Predictor. The simulator works on a tailored [assembly](architecture.md) inspired by the [RISCV](https://riscv.org/risc-v-isa/) Instruction Set Architecture (ISA).

CENOS was written as a part of the project in the course *'CSN506: Advanced Computer Architecture'* taught by Professor [Debiprasanna Sahoo](https://cse.iitr.ac.in/~CSE/Debiprasanna_Sahoo) during the Spring Semester 2022.

## About

Since CENOS simulates O3 execution using scoreboarding, the presence of a multi-stage pipeline is indisputable. We utilised the [popular 5-stage pipeline](https://en.wikipedia.org/wiki/Classic_RISC_pipeline), extending and modifying it later for scoreboarding:

 1. Instruction Fetch (IF)
 2. Instruction Decode (ID)
	 1. Instruction Isssue (II)
	 2. Operand Fetch (OF)
	 3. Execute (EX)
3. Retire

The branch predictor is incorporated in the ID stage. It would be redundant to expound on the working of the simulator since it replicates the flow stated in common literature*.

## Using CENOS

One can run any assembly code of their choice as long as it adheres to the CENOS ASM standard stated in [architecture.md](architecture.md). To run the simulator:

-   Change the directory to `src`

    ```bash
    cd src
    ```

-   Write a sample program in aforementioned ASM standard. For demonstration purposes, say we store the following program named `factorial.s` in the `asm` directory:

    ```asm
    ; This is a sample program to find factorial of number stored in R1
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
    ```

-   Compile the assembler

    ```bash
    cd assembler
    make
    cd ..
    ```

-   Run the assembler giving the program as input:

    ```bash
    ./assembler/assembler asm/factorial.s
    ```

    It outputs `asm/factorial.bin` which looks like:

    ```bin
    1011000000001010
    1100000000000001
    0001001000000001
    1001000100001000
    0100001000100001
    0011000100010001
    1000111111111010
    1010000000000000
    ```

-   Now, compile the simulator:

    ```bash
    cd simulator
    make
    cd ..
    ```

-   Run the simulator giving the generated binary file as input. It generates the a file `asm/factorial.bin.log`:

    ```bash
    ./simulator/simulator asm/factorial.bin
    ```
    The log file is the 'cherry on top' of the simulator in the sense that it contains a highly verbose cycle-by-cycle run-through of the binary, exactly how an actual CPU would do it. It also displays the values held by each register at a given cycle as well as the values of scoreboarding registers and the branch prediction status:
	```
	CPU Cycle: 20
	Branch prediction correct, moving on. Index= 3Branch taken= 0
	Retire:
	Branch taken: 0 alu output= 5
	Retire instruction idx: 3
	Execute:
	

	Operand Fetch: FU(0)
	FU: 0 busy: 1
	Op: SUBI isfetched: 1 isExecuted: 0
	Src reg fj: 10 fk: -1 fl: -1
	imm: 1
	Dest reg fi: 1
	FU prod qj: -1 qk: -1 ql: -1
	Src reg ready rj: 0 rk: 0 rl: 0
	aluout: 0 idx: 5
	  

	Issue:
	FU: 3 busy: 1
	Op: BEQZ isfetched: 0 isExecuted: 0
	Src reg fj: 1 fk: -1 fl: -1
	imm: 8
	Dest reg fi: -1
	FU prod qj: 0 qk: -1 ql: -1
	Src reg ready rj: 0 rk: 1 rl: 1
	aluout: 0 idx: 3
		  

	Registers:
	PC: 4
	General Purpose Registers
	R00: 0000 R01: 0010 R02: 0001 R03: 0000
	R04: 0000 R05: 0000 R06: 0000 R07: 0000
	R08: 0000 R09: 0000 R10: 0000 R11: 0000
	R12: 0000 R13: 0000 R14: 0000 R15: 0000
	R16: 0010
	```


## Directory Structure

    src
    ├── asm             # Assembly source code
    ├── assembler       # Assemlber source code
    ├── Makefile        # Makefile
    ├── simulator       # Simulator source code


---
*We will intentionally refrain from explaining the simulator flow since we are aware that due to the "open-source" nature of the project, it will be blatantly copied by naive students who do not want to put any sort of effort into their coursework. Therefore, from an academic standpoint, it is appreciated if someone could put the least effort of understanding the code before copying it.
