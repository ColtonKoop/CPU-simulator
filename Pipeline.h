#pragma once
#include <string>
#include <iostream>
#include <bitset>
#include "IQueue.h"
#include "Memory.h"
#include "ReorderBuffer.h"
#include "HelperFunctions.h"
#include "RegisterFile.h"
#include "Execute.h"

#define insTyp 0
#define opCod 1
#define op1V 2
#define op1 3
#define op2V 4
#define op2 5
#define imm 6
#define des 7
#define addr 8
#define robid 9

using namespace std;

class Pipeline {
private:

public:
	Pipeline();
	~Pipeline();
	void fetch(HelperFunctions& help, Memory mem, Instruction& instruct, IQueue& IQ, ReorderBuffer& ROB, RegisterFile& Reg);
	void execute(string whichCSV, HelperFunctions& help, Execute& ex, IQueue& IQ, RegisterFile& Reg);
	void markWritebackDependentOperandsValid(IQueue& IQ, int writebackRegisterPosition);
	void commit(HelperFunctions& help, ReorderBuffer& ROB);
};

