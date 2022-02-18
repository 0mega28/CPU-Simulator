#!/bin/bash

actual_dir=$(dirname "$0")

asm_sources="$actual_dir/asm"
assembler_sources="$actual_dir/assembler"
simulator_sources="$actual_dir/simulator"

(
	cd "$asm_sources" || exit
	find . \( -name "*.bin" -or -name "*.log" \) -delete
)

(
	cd "$assembler_sources" || exit
	make clean
)

(
	cd "$simulator_sources" || exit
	make clean
)

echo DONE
