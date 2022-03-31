asm_src=$(wildcard asm/*.s)
assembler_src=$(wildcard assembler/*.cpp assembler/*.hpp)
simulator_src=$(wildcard simulator/*.cpp simulator/*.hpp)
utils_src=utils*.cpp utils*.hpp

assembler_out_src=${asm_src:%.s=%.bin}
simulator_out_src=${assembler_out_src:%.bin=%.bin.log}

assembler=assembler/assembler
simulator=simulator/simulator

all: ${assembler} ${simulator} ${assembler_out_src} ${simulator_out_src}

${assembler}: ${assembler_src} ${utils_src}
	+make -C assembler

${simulator}: ${simulator_src} ${utils_src}
	+make -C simulator

%.bin: %.s ${assembler}
	${assembler} $< > /dev/null

%.bin.log: %.bin ${simulator}
	${simulator} $< > $@

clean:
	rm -f ${assembler_out_src} ${simulator_out_src}
	+make -C assembler clean
	+make -C simulator clean
