#pragma once
#include <string>
#include <bitset>

using namespace std;

class Execute {
private:

public:
	Execute();
	int ExecuteALU(int opCode, int op1, int op2);
	~Execute();
};

