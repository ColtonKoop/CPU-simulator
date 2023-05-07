#pragma once
#include "Instruction.h"
#include "HelperFunctions.h"
#include "Memory.h"
#include <deque>
#include <tuple>

using namespace std;

class IQueue {
private://instruction type, opCode, op1Valid, op1, op2valid, op2, immediate, destination, address, rob_id
	deque<tuple<int, int, bool, int, bool, int, int, int, int, int>> IQ; //this may need to be changed to a deque or a vector to iterate through
public:
	IQueue();
	tuple<int, int, bool, int, bool, int, int, int, int, int> pushIntoIQ(int instructionType, int opCode, bool op1Valid, int op1, bool op2Valid, int op2, int immediate, int destination, int address, int ROB_ID);
	int getFrontInstructType();
	int getFrontIQopCode();
	bool getFrontIQop1Valid();
	int getFrontIQop1();
	bool getFrontIQop2Valid();
	int getFrontIQop2();
	int getFrontIQimmediate();
	int getFrontIQdestination();
	int getFrontAddress();
	int getFrontIQROB_ID();
	void popIQ();
	bool isEmpty();
	int size();
	void setFrontIQop1Valid(bool validity);
	void setFrontIQop2Valid(bool validity);
	deque<tuple<int, int, bool, int, bool, int, int, int, int, int>>& getIQ();
	~IQueue();
};

