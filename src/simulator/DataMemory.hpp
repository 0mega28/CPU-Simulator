#pragma once

#include <assert.h>
#include <iostream>
#include <iomanip>

class DataMemory
{
private:
	int *dataCache;
	const int dataCacheSize = 1024;

public:
	DataMemory();
	~DataMemory();

	int getData(int address);
	void setData(int address, int value);

	void dumpMemory();
};

DataMemory::DataMemory()
{
	dataCache = new int[dataCacheSize];
	for (int i = 0; i < dataCacheSize; i++)
	{
		dataCache[i] = 0;
	}
}

DataMemory::~DataMemory()
{
	delete[] dataCache;
}

int DataMemory::getData(int address)
{
	assert(address < dataCacheSize);
	return dataCache[address];
}

void DataMemory::setData(int address, int value)
{
	assert(address < dataCacheSize);
	dataCache[address] = value;
}

void DataMemory::dumpMemory()
{
	std::cout << "Data Memory: " << std::endl;

	const int numCols = 16;
	for (int i = 0; i < dataCacheSize / numCols; i++)
	{
		std::cout << std::setfill('0') << std::setw(5) << i * numCols << ":\t";
		for (int j = 0; j < numCols; j++)
		{
			std::cout << '\t' << std::setfill('0') << std::setw(3) << dataCache[i * numCols + j] << " ";
		}
		std::cout << std::endl;
	}
}
