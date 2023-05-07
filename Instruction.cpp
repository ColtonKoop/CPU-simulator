#include "Instruction.h"

Instruction::Instruction() {

}

void Instruction::decodeInstruction(unsigned int instruction) {
	this->instructionUnsignedInt = instruction;
	convertInstructionToBinaryString();

	instructionType = bitset<2>(instructionBinaryString.substr(31-31,2)).to_ulong();

	if (instructionType == 0) { //R type
		opCode = bitset<4>(instructionBinaryString.substr(31-29, 4)).to_ulong();
		dest = bitset<5>(instructionBinaryString.substr(31-25, 5)).to_ulong();
		op1 = bitset<5>(instructionBinaryString.substr(31-20, 5)).to_ulong();
		op2 = bitset<5>(instructionBinaryString.substr(31-15, 5)).to_ulong();
	}
	else if (instructionType == 1) { //I type
		opCode = bitset<4>(instructionBinaryString.substr(31-29, 4)).to_ulong();
		dest = bitset<5>(instructionBinaryString.substr(31-25, 5)).to_ulong();
		op1 = bitset<5>(instructionBinaryString.substr(31-20, 5)).to_ulong();
		immediate = bitset<16>(instructionBinaryString.substr(31-15, 16)).to_ulong();
	}
	else if (instructionType == 2) { //J type
		address = bitset<30>(instructionBinaryString.substr(31-29, 30)).to_ulong();
	}
	else { // == 3 P type
		opCode = bitset<4>(instructionBinaryString.substr(31-29, 4)).to_ulong();
		dest = bitset<5>(instructionBinaryString.substr(31-25, 5)).to_ulong();
	}
}

int Instruction::getUnsignedInstruction() {
	return instructionUnsignedInt;
}

int Instruction::getInstructionType() {
	return instructionType;
}

int Instruction::getOpCode() {
	return opCode;
}

int Instruction::getOp1() {
	return op1;
}

int Instruction::getOp2() {
	return op2;
}

int Instruction::getDest() {
	return dest;
}

int Instruction::getImmediate() {
	return immediate;
}

int Instruction::getAddress() {
	return address;
}

void Instruction::convertInstructionToBinaryString() {
	bitset<32> tempBitset(instructionUnsignedInt);
	instructionBinaryString = tempBitset.to_string();
}

Instruction::~Instruction() {

}