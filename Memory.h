#pragma once
#include <vector>
#include <string>

using namespace std;

class Memory {
private:
	vector<unsigned int> instructionMemory;
public:
	Memory();
	void setMemory(vector<unsigned int>);
	void setMemory(int position, unsigned int data);
	int getSize();
	unsigned int getInstruction(int position);
	void clearMemory();
	~Memory();
};

