#pragma once
#include <deque>
#include <tuple>
#include "Instruction.h"
#include "HelperFunctions.h"

using namespace std;

class ReorderBuffer {
private: //valid, instruction, ROB_ID
	deque<tuple<bool, unsigned int, int>> ROB;
public:
	ReorderBuffer();
	void pushIntoROB(bool valid, unsigned int instruction, int ROB_ID);
	bool isEmpty();
	void popROB();
	void setValid(int ROB_ID);
	bool getFrontROBValid();
	int getFrontROBID();
	~ReorderBuffer();
};

