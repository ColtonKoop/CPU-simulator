#include "Memory.h"
#include "Memory.h"

Memory::Memory() {

}

void Memory::setMemory(vector<unsigned int> tempVector) {
	vector<unsigned int>::iterator it;

	for (it = tempVector.begin(); it != tempVector.end(); it++) {
		instructionMemory.push_back(*it);
	}
}

void Memory::setMemory(int position, unsigned int data) {
	instructionMemory.at(position) = data;
}

int Memory::getSize() {
	return instructionMemory.size();
}

unsigned int Memory::getInstruction(int whichInstruction) {
	return instructionMemory.at(whichInstruction);
}

void Memory::clearMemory() {
	instructionMemory.clear();
}

Memory::~Memory() {

}
