# CPU Simulator

This project is about simulating out-of-order CPU execution using scoreboarding algorithm on our own assembly with 2-bit branch prediction.

To understand our assembly instruction refer to [architecture.md](architecture.md).

# Flow of project

Instructions to assemble and run the simulator -

-   Change the directory to src

    ```bash
    cd src
    ```

-   Write a sample program in assembly language in asm directory and save it as factorial.s

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

-   Run the assembler giving factorial.s as input

    ```bash
    ./assembler/assembler asm/factorial.s
    ```

    It'll output asm/factorial.bin which looks like this

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

-   Compile the simulator

    ```bash
    cd simulator
    make
    cd ..
    ```

-   Run the simulator giving factorial.bin as input, it'll output asm/factorial.bin.log

    ```bash
    ./simulator/simulator asm/factorial.bin
    ```

# Directory structure

    src
    ├── asm             # Assembly source code
    ├── assembler       # Assemlber source code
    ├── Makefile        # Makefile
    ├── simulator       # Simulator source code
