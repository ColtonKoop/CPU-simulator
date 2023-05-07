#pragma once
#include <vector>
#include <string>
#include <bitset>

using namespace std;

class Instruction {
private:
	unsigned int instructionUnsignedInt;
	string instructionBinaryString;
	int instructionType = -1;
	int opCode = -1;
	int op1 = -1; //operands
	int op2 = -1;
	int dest = -1;
	int immediate = -1;
	int address = -1;
	void convertInstructionToBinaryString();
public:
	Instruction();
	void decodeInstruction(unsigned int);
	int getUnsignedInstruction();
	int getInstructionType();
	int getOpCode();
	int getOp1();
	int getOp2();
	int getDest();
	int getImmediate();
	int getAddress();
	~Instruction();
};

