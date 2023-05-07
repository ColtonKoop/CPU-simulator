#pragma once
#include <vector>
#include <string>
#include <bitset>
#include <tuple>

using namespace std;

class RegisterFile {
private:
	vector<tuple<int, bool>> registers; //16 registers
public:
	RegisterFile();
	void setRegister(int position, int data, bool validity);
	void setRegisterData(int position, int data);
	void setRegisterValid(int position, bool validity);
	tuple<int, bool> getRegister(int whichRegister);
	int getRegisterData(int whichRegister);
	bool getRegisterValid(int whichRegister);
	void clearRegisters();
	void initializeRegisters();
	~RegisterFile();
};

