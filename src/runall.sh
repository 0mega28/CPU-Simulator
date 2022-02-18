#!/bin/bash

actual_dir=$(dirname "$0")

asm_sources="asm"
assembler_sources="$actual_dir/assembler"
simulator_sources="$actual_dir/simulator"

(
	# build assembler
	echo "Building assembler..."
	cd "$assembler_sources" || exit
	make clean &> /dev/null
	make &> /dev/null

	# find all assembly files
	asm_files=$(find "../$asm_sources" -name "*.s")

	# assemble assembly files
	for file in $asm_files; do
		echo "Assembling $file"
		./assembler "$file" &>/dev/null &
	done

	wait
)

(
	# build simulator
	echo "Building simulator..."
	cd "$simulator_sources" || exit
	make clean &> /dev/null
	make &> /dev/null

	# find all assembled assembly files
	bin_files=$(find "../$asm_sources" -name "*.bin")

	# simulate assembled files and store ouptut in .log files
	for file in $bin_files; do
		echo "Simulating $file"
		out_file="$file".log
		./simulator "$file" &>"$out_file" &
	done

	wait
)

echo "DONE"
