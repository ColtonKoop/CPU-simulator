#include "ReorderBuffer.h"

bool ReorderBuffer::isEmpty() {
	return ROB.empty();
}

void ReorderBuffer::popROB() {
	ROB.pop_front();
}

void ReorderBuffer::setValid(int ROB_ID) {
	deque<tuple<bool, unsigned int, int>>::iterator it;

	for (it = ROB.begin(); it != ROB.end(); it++) {
		if (ROB_ID == get<2>(*it)) {
			get<0>(*it) = true;
			break;
		}
	}
}

bool ReorderBuffer::getFrontROBValid() {
	return get<0>(ROB.front());
}

int ReorderBuffer::getFrontROBID() {
	return get<2>(ROB.front());
}

ReorderBuffer::ReorderBuffer() {
}

void ReorderBuffer::pushIntoROB(bool valid, unsigned int instruction, int ROB_ID) {
	ROB.push_back(make_tuple(valid, instruction, ROB_ID));
}

ReorderBuffer::~ReorderBuffer() {
}
