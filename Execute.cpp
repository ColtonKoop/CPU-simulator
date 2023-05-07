#include "Execute.h"

Execute::Execute() {

}

int  Execute::ExecuteALU(int opCode, int op1, int op2) { // consider expanding this to include reg and dest more closely integrated with registers
	
	if (opCode == 0) { //add
		return op1 + op2;
	}
	else if (opCode == 1) { //subtract
		return op1 - op2;
	}
	else if (opCode == 2) { //multiply
		return op1 * op2;
	}
	else if (opCode == 3) { //divide
		return op1 / op2;
	}
	else if (opCode == 4) { //modulo
		return op1 % op2;
	}
	else if (opCode == 5 || opCode == 6) {
		//BEQ/BNE sub??
	}
	else { //opCode = 7 or 8
		//BEQ/BNE MOV??
	}
}

Execute::~Execute() {

}
