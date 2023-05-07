#include "Execute.h"
#include "FetchUnit.h"
#include "HelperFunctions.h"
#include "Instruction.h"
#include "IQueue.h"
#include "Memory.h"
#include "Pipeline.h"
#include "RegisterFile.h"
#include "ReorderBuffer.h"
#include "Statistics.h"

#define insTyp 0
#define opCod 1
#define op1V 2
#define op1 3
#define op2V 4
#define op2 5
#define imm 6
#define des 7
#define addr 8
#define robid 9

#include <string>
#include <iostream>
#include <bitset>

using namespace std;

void runProgram(string, Execute&, FetchUnit&, HelperFunctions&, Instruction&, IQueue&, Memory&, Pipeline&, RegisterFile&, ReorderBuffer&, Statistics&, bool&);

int main() {
	Execute exObj;
	FetchUnit FUObj;
	HelperFunctions helpObj;
	Instruction instructionObj;
	IQueue IQObj;
	Memory memObj;
	Pipeline pipelineObj;
	RegisterFile regObj;
	ReorderBuffer ROBObj;
	Statistics statObj;

	bool exit = false;
	bool debugFlag = false;
	string choice;

	cout << "Welcome to the CPU Simulator!" << endl;
	cout << "Select a Program to run:" << endl;

	while (!exit) {
		cout << endl << "(I)nst mem, F(a)ctorial, Fi(b)onacci, (M)y Program, E(x)it: ";

		getline(cin, choice);
		cout << endl;

		switch ((char)choice[0]) {
		case 'd':
		case 'D':
			if (debugFlag ==  true) {
				debugFlag = false;
				cout << "Debug mode disabled!" << endl;
			}
			else {
				debugFlag = true;
				cout << "Debug mode enabled!" << endl;
			}
			break;
		case 'i':
		case 'I':
			runProgram("inst_mem.csv", exObj, FUObj, helpObj, instructionObj, IQObj, memObj, pipelineObj, regObj, ROBObj, statObj, debugFlag);
			break;
		case 'a':
		case 'A':
			runProgram("factorial.csv", exObj, FUObj, helpObj, instructionObj, IQObj, memObj, pipelineObj, regObj, ROBObj, statObj, debugFlag);
			break;
		case 'b':
		case 'B':
			runProgram("fibonacci.csv", exObj, FUObj, helpObj, instructionObj, IQObj, memObj, pipelineObj, regObj, ROBObj, statObj, debugFlag);
			break;
		case 'm':
		case 'M':
			runProgram("myProgram.csv", exObj, FUObj, helpObj, instructionObj, IQObj, memObj, pipelineObj, regObj, ROBObj, statObj, debugFlag);
			break;
		case 'x':
		case 'X':
			exit = true;
			break;
		default:
			cout << "Invalid choice; please try again.\n\n";
			break;
		}
	}

	cout << "Thank you; goodbye.\n";

	return 0;
}

void runProgram(string whichCSV, Execute& exObj, FetchUnit& FUObj, HelperFunctions& helpObj, Instruction& instructionObj, IQueue& IQObj, Memory& memObj, Pipeline& pipelineObj, RegisterFile& regObj, ReorderBuffer& ROBObj, Statistics& statObj, bool& debugFlag) {
	bool stallForBranch = false;
	bool ROBcorrupt = false;
	string debugMessage = "";
	string tempInput = "";
	
	FUObj.loadCSVToMemory(whichCSV, memObj);

	while (!ROBObj.isEmpty() || helpObj.programCounter == 0) {	

		if (helpObj.programCounter != 0) {
			debugMessage.append("------------ COMMIT ------------\n");
		}

		//Commit
		for (int i = 0; i < helpObj.COMMIT_WIDTH; i++) {
			if (!ROBObj.isEmpty()) {
				if (helpObj.commitCounter == ROBObj.getFrontROBID()) {
					if (ROBObj.getFrontROBValid() == true) {
						statObj.ROBIDExitsPipeline(ROBObj.getFrontROBID());
						debugMessage.append("commiting ROB ID " + to_string(ROBObj.getFrontROBID()) + "\n");
						pipelineObj.commit(helpObj, ROBObj);
						helpObj.commitCounter++;
					}
				}
				else {
					cout << "ROB corruption detected; terminating program" << endl;
					ROBcorrupt = true;
					break;
				}
			}
		}

		if (ROBcorrupt) {
			break;
		}

		if (debugFlag && helpObj.programCounter != 0) {
			debugMessage.append("Enter anything to advance (x to exit debug): ");
			cout << debugMessage;
			getline(cin, tempInput);
			tempInput = tempInput[0];
			cout << endl;
			if (tempInput == "x" || tempInput == "X") {
				debugFlag = false;
			}
		}
		debugMessage = "";
		tempInput = "";
		statObj.incrementClockCycle();

		//Fetch
		debugMessage.append("------------ FETCH ------------\n");
		for (int i = 0; i < helpObj.FETCH_WIDTH; i++) {
			if (stallForBranch == false) {
				if (helpObj.programCounter < memObj.getSize()) {
					pipelineObj.fetch(helpObj, memObj, instructionObj, IQObj, ROBObj, regObj);
					debugMessage.append(bitset<32>(memObj.getInstruction(helpObj.programCounter)).to_string() + "\n");
					statObj.ROBIDEntersPipeline(helpObj.ROB_ID_counter);
				}
				helpObj.programCounter++;
				helpObj.ROB_ID_counter++;
			}
			if (!IQObj.isEmpty()) {
				if (get<1>(IQObj.getIQ().back()) == 5 || get<1>(IQObj.getIQ().back()) == 6) { //branch operation detected
					stallForBranch = true;
				}
			}
		}

		if (!IQObj.isEmpty()) {
			debugMessage.append("------------ INSTRUCTION QUEUE ------------\n");
			deque<tuple<int, int, bool, int, bool, int, int, int, int, int>>::iterator it;
			//instruction type, opCode, op1Valid, op1, op2valid, op2, immediate, destination, address, rob_id
			debugMessage.append("insTyp opCod op1V op1 op2V op2 imm dest add robid\n");
			for (it = IQObj.getIQ().begin(); it != IQObj.getIQ().end(); it++) {
				debugMessage.append(to_string(get<0>(*it)) + "      " + to_string(get<1>(*it)) + "     " + to_string(get<2>(*it)) + "    " + to_string(get<3>(*it)) + "   " + to_string(get<4>(*it)) + "    " + to_string(get<5>(*it)) + "   " + to_string(get<6>(*it)) + "   " + to_string(get<7>(*it)) + "    " + to_string(get<8>(*it)) + "   " + to_string(get<9>(*it)) + "\n");
			}
		}

		statObj.incrementClockCycle();

		//Execute
		debugMessage.append("------------ EXECUTE ------------\n");
		for (int i = 0; i < helpObj.ISSUE_WIDTH; i++) {
			if (!IQObj.isEmpty()) {
				debugMessage.append("checking status: src1 V=" + to_string(IQObj.getFrontIQop1Valid()) + " src2 V=" + to_string(IQObj.getFrontIQop2Valid()) + "\n");
				if (IQObj.getFrontIQop1Valid() == true || IQObj.getFrontIQop2Valid() == true) {
					debugMessage.append("Instruction " + to_string(IQObj.getFrontIQROB_ID()) + " ready for execution\n");
					pipelineObj.execute(whichCSV, helpObj, exObj, IQObj, regObj);
					debugMessage.append("Instruction " + to_string(IQObj.getFrontIQROB_ID()) + " executed\n");
					statObj.incrementThroughputCounter();

					if (get<1>(IQObj.getIQ().front()) == 5 || get<1>(IQObj.getIQ().front()) == 6) { //branch operation was just executed
						stallForBranch = false;
					}

					ROBObj.setValid(IQObj.getFrontIQROB_ID());
					debugMessage.append("ROB " + to_string(IQObj.getFrontIQROB_ID()) + " marked\n\n");

					IQObj.popIQ();
				}
				else {
					break;
				}
			}
		}

		statObj.throughputDoneThisCycle();
		statObj.incrementClockCycle();
	}

	statObj.printStatistics();

	//reset for next program
	statObj.clearStatistics();
	memObj.clearMemory();
	regObj.clearRegisters();
	helpObj.programCounter = 0;
	helpObj.ROB_ID_counter = 0;
	helpObj.commitCounter = 0;
}