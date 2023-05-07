#include "IQueue.h"

IQueue::IQueue() {

}

tuple<int, int, bool, int, bool, int, int, int, int, int> IQueue::pushIntoIQ(int instructionType, int opCode, bool op1Valid, int op1, bool op2Valid, int op2, int immediate, int dest, int addy, int ROB_ID) {
	IQ.push_back(make_tuple(instructionType, opCode, op1Valid, op1, op2Valid, op2, immediate, dest, addy, ROB_ID));
	return IQ.back();
}

int IQueue::getFrontInstructType() {
	return get<0>(IQ.front());
}

int IQueue::getFrontIQopCode() {
	return get<1>(IQ.front());
}

bool IQueue::getFrontIQop1Valid() {
	return get<2>(IQ.front());
}

int IQueue::getFrontIQop1() {
	return get<3>(IQ.front());
}

bool IQueue::getFrontIQop2Valid() {
	return get<4>(IQ.front());
}

int IQueue::getFrontIQop2() {
	return get<5>(IQ.front());
}

int IQueue::getFrontIQimmediate() {
	return get<6>(IQ.front());
}

int IQueue::getFrontIQdestination() {
	return get<7>(IQ.front());
}

int IQueue::getFrontAddress() {
	return get<8>(IQ.front());
}

int IQueue::getFrontIQROB_ID() {
	return get<9>(IQ.front());
}

void IQueue::popIQ() {
	IQ.pop_front();
}

bool IQueue::isEmpty() {
	return IQ.empty();
}

int IQueue::size() {
	return IQ.size();
}

void IQueue::setFrontIQop1Valid(bool validity) {
	get<2>(IQ.front()) = validity;
}

void IQueue::setFrontIQop2Valid(bool validity) {
	get<4>(IQ.front()) = validity;
}

deque<tuple<int, int, bool, int, bool, int, int, int, int, int>>& IQueue::getIQ() {
	return IQ;
}

IQueue::~IQueue() {

}
