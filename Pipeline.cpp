#include "Pipeline.h"

Pipeline::Pipeline() {

}

Pipeline::~Pipeline() {

}

void Pipeline::fetch(HelperFunctions& help, Memory mem, Instruction& instruct, IQueue& IQ, ReorderBuffer& ROB, RegisterFile& reg) {
	if (help.programCounter < mem.getSize()) {
		instruct.decodeInstruction(mem.getInstruction(help.programCounter)); //decode the instruction at program counter position in memory
		IQ.pushIntoIQ(instruct.getInstructionType(), instruct.getOpCode(), false, instruct.getOp1(), false, instruct.getOp2(), instruct.getImmediate(), instruct.getDest(), instruct.getAddress(), help.ROB_ID_counter);

		//checking validity of operands upon insertion to the IQ and setting writeback registers to false as they're about to be written to
		if (get<insTyp>(IQ.getIQ().back()) == 0) { //R type
			if (get<opCod>(IQ.getIQ().back()) == 7) { //move operation
				get<op1V>(IQ.getIQ().back()) = true;
				if (reg.getRegisterValid(get<op1>(IQ.getIQ().back())) == true) {
					get<op2V>(IQ.getIQ().back()) = true;
				}
				reg.setRegisterValid(get<des>(IQ.getIQ().back()), false);
			}
			else if (get<opCod>(IQ.getIQ().back()) == 8) { //move operation with a print
				get<op1V>(IQ.getIQ().back()) = true;
				if (reg.getRegisterValid(get<op1>(IQ.getIQ().back())) == true) {
					get<op2V>(IQ.getIQ().back()) = true;
				}
				reg.setRegisterValid(get<des>(IQ.getIQ().back()), false);
			}
			else { //regular R type instruction
				if (reg.getRegisterValid(get<op1>(IQ.getIQ().back())) == true) {
					get<op1V>(IQ.getIQ().back()) = true;
				}
				if (reg.getRegisterValid(get<op2>(IQ.getIQ().back())) == true) {
					get<op2V>(IQ.getIQ().back()) = true;
				}
				reg.setRegisterValid(get<des>(IQ.getIQ().back()), false);
			}
		}
		else if (get<insTyp>(IQ.getIQ().back()) == 1) { //I type
			if (get<opCod>(IQ.getIQ().back()) == 5) { //Branch if equal operation
				if (reg.getRegisterValid(get<des>(IQ.getIQ().back())) == true) {
					get<op1V>(IQ.getIQ().back()) = true;
				}
				if (reg.getRegisterValid(get<op1>(IQ.getIQ().back())) == true) {
					get<op2V>(IQ.getIQ().back()) = true;
				}
			}
			else if (get<opCod>(IQ.getIQ().back()) == 6) { //branch if not equal operation
				if (reg.getRegisterValid(get<des>(IQ.getIQ().back())) == true) {
					get<op1V>(IQ.getIQ().back()) = true;
				}
				if (reg.getRegisterValid(get<op1>(IQ.getIQ().back())) == true) {
					get<op2V>(IQ.getIQ().back()) = true;
				}
			}
			else { //regular I-type instruction
				if (reg.getRegisterValid(get<op1>(IQ.getIQ().back())) == true) {
					get<op1V>(IQ.getIQ().back()) = true;
					get<op2V>(IQ.getIQ().back()) = true;
				}
				reg.setRegisterValid(get<des>(IQ.getIQ().back()), false);
			}
		}
		else if (get<insTyp>(IQ.getIQ().back()) == 2) { //J type
			get<op1V>(IQ.getIQ().back()) = true;
			get<op2V>(IQ.getIQ().back()) = true;
		}
		else { // == 3, P type
			get<op1V>(IQ.getIQ().back()) = true;
			get<op2V>(IQ.getIQ().back()) = true;
			reg.setRegisterValid(get<des>(IQ.getIQ().back()), false);
		}

		ROB.pushIntoROB(false, instruct.getUnsignedInstruction(), help.ROB_ID_counter);
	}
}

void Pipeline::execute(string whichCSV, HelperFunctions& help, Execute& ex, IQueue& IQ, RegisterFile& Reg) {
	if (IQ.getFrontIQop1Valid() == true && IQ.getFrontIQop2Valid() == true) {
		if (IQ.getFrontInstructType() == 0) { //R type
			if (IQ.getFrontIQopCode() == 7) { //move operation
				Reg.setRegister(IQ.getFrontIQdestination(), Reg.getRegisterData(IQ.getFrontIQop1()), true);
				markWritebackDependentOperandsValid(IQ, IQ.getFrontIQdestination());
			}
			else if (IQ.getFrontIQopCode() == 8) { //move operation with a print
				Reg.setRegister(IQ.getFrontIQdestination(), Reg.getRegisterData(IQ.getFrontIQop1()), true);
				markWritebackDependentOperandsValid(IQ, IQ.getFrontIQdestination());
				cout << "Final result is: " << Reg.getRegisterData(IQ.getFrontIQdestination()) << endl;
			}
			else { //regular R type instruction
				Reg.setRegister(IQ.getFrontIQdestination(), ex.ExecuteALU(IQ.getFrontIQopCode(), Reg.getRegisterData(IQ.getFrontIQop1()), Reg.getRegisterData(IQ.getFrontIQop2())), true);
				markWritebackDependentOperandsValid(IQ, IQ.getFrontIQdestination());
			}
		}
		else if (IQ.getFrontInstructType() == 1) { //I type
			if (IQ.getFrontIQopCode() == 5) { //Branch if equal operation
				if (ex.ExecuteALU(1 /*subtraction*/, Reg.getRegisterData(IQ.getFrontIQdestination()), Reg.getRegisterData(IQ.getFrontIQop1())) == 0) { //equal
					help.programCounter = IQ.getFrontIQimmediate();
				}
				else { /*not equal--program counter taken care of*/ }
			}
			else if (IQ.getFrontIQopCode() == 6) { //branch if not equal operation
				if (ex.ExecuteALU(1 /*subtraction*/, Reg.getRegisterData(IQ.getFrontIQdestination()), Reg.getRegisterData(IQ.getFrontIQop1())) == 0) { /*equal--program counter taken care of*/ }
				else { //not equal
					help.programCounter = IQ.getFrontIQimmediate();
				}
			}
			else { //regular I-type instruction
				Reg.setRegister(IQ.getFrontIQdestination(), ex.ExecuteALU(IQ.getFrontIQopCode(), Reg.getRegisterData(IQ.getFrontIQop1()), IQ.getFrontIQimmediate()), true);
				markWritebackDependentOperandsValid(IQ, IQ.getFrontIQdestination());
			}
		}
		else if (IQ.getFrontInstructType() == 2) { //J type
			help.programCounter = IQ.getFrontAddress();
		}
		else { // == 3, P type
			string tempInput;
			int convertedInput;
			bool validInput = false;
			cout << "Enter parameter for program: ";

			//validate input
			while (validInput == false) {
				try {
					getline(cin, tempInput);
					cout << endl;
					convertedInput = stoul(tempInput);
					if (whichCSV == "factorial.csv") {
						if (convertedInput < 1 || convertedInput > 12) {
							throw "Try a number between 1 and 12: ";
						}
					}
					else if (whichCSV == "fibonacci.csv") {
						if (convertedInput < 1 || convertedInput > 46) {
							throw "Try a number between 1 and 46: ";
						}
					}
					else { //myProgram
						if (convertedInput < 1 || convertedInput > 65535) {
							throw "Try a number between 1 and 65535: ";
						}
					}
					validInput = true;
				}
				catch (char const* e) {
					cout << e;
				}
				catch (...) {
					cout << "Invalid parameter; try again: ";
				}
			}
			Reg.setRegister(2, convertedInput, true);
			markWritebackDependentOperandsValid(IQ, 2);
		}	
	}
}

void Pipeline::markWritebackDependentOperandsValid(IQueue& IQ, int writebackRegisterPosition) {
	if (IQ.size() > 1) { //Theres instructions left in the IQ to be executed after the current one
		deque<tuple<int, int, bool, int, bool, int, int, int, int, int>>::iterator it = IQ.getIQ().begin();
		it++;
		for (; it != IQ.getIQ().end(); it++) {
			if (get<insTyp>(*it) == 0) { //R type
				if (get<opCod>(*it) == 7) { //move operation
					if (get<op1>(*it) == IQ.getFrontIQdestination()) { //compare op1 to writeback
						get<op2V>(*it) = true; //op2 valid is true
					}
					if (get<des>(*it) == IQ.getFrontIQdestination()) {
						break;
					}
				}
				else if (get<opCod>(*it) == 8) { //move operation with a print
					if (get<op1>(*it) == IQ.getFrontIQdestination()) { //compare op1 to writeback
						get<op2V>(*it) = true; //op2 valid is true
					}
					if (get<des>(*it) == IQ.getFrontIQdestination()) {
						break;
					}
				}
				else { //regular R type instruction
					if (get<op1>(*it) == IQ.getFrontIQdestination()) { //compare op1 to writeback
						get<op1V>(*it) = true; //op1 valid is true
					}
					if (get<op2>(*it) == IQ.getFrontIQdestination()) { //compare op2 to writeback
						get<op2V>(*it) = true; //op2 valid is true
					}
					if (get<des>(*it) == IQ.getFrontIQdestination()) {
						break;
					}
				}
			}
			else if (get<insTyp>(*it) == 1) { //I type
				if (get<opCod>(*it) == 5) { //Branch if equal operation
					if (get<des>(*it) == IQ.getFrontIQdestination()) { //compare dest to writeback
						get<op1V>(*it) = true; //op1 valid is true
					}
					if (get<op1>(*it) == IQ.getFrontIQdestination()) { //compare op1 to writeback
						get<op2V>(*it) = true; //op2 valid is true
					}
					if (get<des>(*it) == IQ.getFrontIQdestination()) {
						break;
					}
				}
				else if (get<opCod>(*it) == 6) { //branch if not equal operation
					if (get<des>(*it) == IQ.getFrontIQdestination()) { //compare dest to writeback
						get<op1V>(*it) = true; //op1 valid is true
					}
					if (get<op1>(*it) == IQ.getFrontIQdestination()) { //compare op1 to writeback
						get<op2V>(*it) = true; //op2 valid is true
					}
					if (get<des>(*it) == IQ.getFrontIQdestination()) {
						break;
					}
				}
				else { //regular I-type instruction
					get<op2V>(*it) = true; //op 2 flag always valid since immediate doesn't read from any register
					if (get<op1>(*it) == IQ.getFrontIQdestination()) { //compare op1 to writeback
						get<op1V>(*it) = true; //op1 valid is true
					}
					if (get<des>(*it) == IQ.getFrontIQdestination()) {
						break;
					}
				}
			}
			else if (get<insTyp>(*it) == 2) { //J type
				//doesn't read or write any registers
			}
			else { // == 3, P type
				if (get<des>(*it) == IQ.getFrontIQdestination()) {
					break;
				}
			}

		}
	}
}

void Pipeline::commit(HelperFunctions& help, ReorderBuffer& ROB) {
	if (!ROB.isEmpty()) {
		if (ROB.getFrontROBValid() == true) {
			ROB.popROB();
		}
	}
}
