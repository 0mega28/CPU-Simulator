#pragma once

#include <iostream>
#include <setjmp.h>

#include "Fetch.hpp"
#include "Decode.hpp"
#include "Execute.hpp"
#include "Retire.hpp"
#include "Register.hpp"
#include "InstructionMemory.hpp"
#include "DataMemory.hpp"
#include "../utils.hpp"

#define FAKE_CYCLE

class Core
{
private:
	Fetch *fetch;
	Decode *decode;
	Execute *execute;
	Retire *retire;

	InstructionMemory *instructionMemory;
	DataMemory *dataMemory;

	unsigned int CPUclock = 1;

	void _cycle();
	void _fake_cycle();

	void cycle();
public:
	Core(std::string binFile);
	~Core();

	void run();
};

Core::Core(std::string binFile)
{
	this->instructionMemory = new InstructionMemory(binFile);
	this->dataMemory = new DataMemory();

	this->fetch = new Fetch(this->instructionMemory);
	this->decode = new Decode();
	this->execute = new Execute();
	this->retire = new Retire(this->dataMemory);
}

Core::~Core()
{
	delete this->instructionMemory;
	delete this->dataMemory;

	delete this->fetch;
	delete this->decode;
	delete this->execute;
	delete this->retire;
}

void Core::_cycle()
{
	this->retire->cycle();
	this->execute->cycle();
	this->decode->cycle();
	this->fetch->cycle();
}

void Core::_fake_cycle()
{
	this->fetch->cycle();
	this->decode->cycle();
	this->execute->cycle();
	this->retire->cycle();
}

void Core::cycle()
{
	std::cout << "\nCPU Cycle: " << CPUclock << std::endl;

#ifdef FAKE_CYCLE
	this->_fake_cycle();
#else
	this->_cycle();
#endif

	RegSet::dumpRegisters();

	CPUclock++;

	std::cout << "\n-------------------------------------------------------\n";
}

void Core::run()
{
	std::cout << "Starting Simulator" << std::endl;

	this->instructionMemory->dumpMemory();

	while (!setjmp(halt_cpu))
		cycle();

	std::cout << std::endl;

	this->dataMemory->dumpMemory();
	RegSet::dumpRegisters();

	std::cout << "Simulation Complete" << std::endl;
}