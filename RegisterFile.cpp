#include "RegisterFile.h"

RegisterFile::RegisterFile() {
	initializeRegisters();
}

void RegisterFile::setRegister(int position, int data, bool validity) {
	registers.at(position) = make_tuple(data, validity);
}

void RegisterFile::setRegisterData(int position, int data) {
	get<0>(registers.at(position)) = data;
}

void RegisterFile::setRegisterValid(int position, bool validity) {
	get<1>(registers.at(position)) = validity;
}

tuple<int, bool> RegisterFile::getRegister(int position) {
	return registers.at(position);
}

int RegisterFile::getRegisterData(int position) {
	return get<0>(registers.at(position));
}

bool RegisterFile::getRegisterValid(int position) {
	return get<1>(registers.at(position));
}

void RegisterFile::clearRegisters() {
	registers.clear();
	initializeRegisters();
}

void RegisterFile::initializeRegisters() {
	for (int i = 0; i < 16; i++) { //make 16 empty registers
		registers.push_back(make_tuple(0, true));
	}
}

RegisterFile::~RegisterFile() {

}
