#pragma once
#include "Memory.h"
#include <string>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

class FetchUnit {
private:

public:
	FetchUnit();
	unsigned int fetchInstructionFromMemory(int whichInstruction, Memory mem);
	void loadCSVToMemory(string whichCSV, Memory& mem);
	~FetchUnit();
};

