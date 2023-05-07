#include "FetchUnit.h"

FetchUnit::FetchUnit() {

}

unsigned int FetchUnit::fetchInstructionFromMemory(int whichInstruction, Memory mem) {
	return mem.getInstruction(whichInstruction);
}

void FetchUnit::loadCSVToMemory(string whichCSV, Memory& mem) {
	string datum;
	vector<unsigned int> tempVector;

	ifstream CSV(whichCSV);

	if (CSV.is_open()) {
		try {
			while (!CSV.eof()) {
				getline(CSV, datum, ',');
				//needed to remove these 3 strange characters at the beginning of the CSVs
				datum.erase(remove(datum.begin(), datum.end(), 'ï'), datum.end());
				datum.erase(remove(datum.begin(), datum.end(), '»'), datum.end());
				datum.erase(remove(datum.begin(), datum.end(), '¿'), datum.end());
				if (datum == "") continue;
				tempVector.push_back(stoul(datum));
			}
		}
		catch (...) {
			cout << "Error parsing data";
		}
	}
	else {
		cout << "Error opening CSV";
	}

	mem.setMemory(tempVector);

	CSV.close();
}

FetchUnit::~FetchUnit() {

}
