#include "Statistics.h"

void Statistics::caclulateAvgLatency() {
	int tempRunningTotal = 0;
	vector<tuple<int, int, int>>::iterator it;

	if (!latencyVector.empty()) {
		for (it = latencyVector.begin(); it != latencyVector.end(); it++) {
			tempRunningTotal += get<1>(*it) - get<0>(*it); //exits - enters
		}
	}

	avgLatency = (double)tempRunningTotal / (double)latencyVector.size();
}

void Statistics::calculateAvgThroughput() {
	int tempRunningTotal = 0;
	vector<int>::iterator it;

	if (!throughputVector.empty()) {
		for (it = throughputVector.begin(); it != throughputVector.end(); it++) {
			tempRunningTotal += *it;
		}
	}

	avgThroughput = (double)tempRunningTotal / (double)throughputVector.size();
}

void Statistics::ROBIDEntersPipeline(int ROB_ID) {
	latencyVector.push_back(make_tuple(clockCycleCounter,clockCycleCounter,ROB_ID));
}

void Statistics::ROBIDExitsPipeline(int ROB_ID) {
	vector<tuple<int, int, int>>::iterator it;

	if (!latencyVector.empty()) {
		for (it = latencyVector.begin(); it != latencyVector.end(); it++) {
			if (get<2>(*it) == ROB_ID) {
				get<1>(*it) = clockCycleCounter;
				break;
			}
		}
	}
}

void Statistics::incrementThroughputCounter() {
	throughputCounter++;
}

void Statistics::throughputDoneThisCycle() {
	throughputVector.push_back(throughputCounter);
	throughputCounter = 0;
}

void Statistics::incrementClockCycle() {
	clockCycleCounter++;
}

void Statistics::printStatistics() {
	caclulateAvgLatency();
	calculateAvgThroughput();
	cout << "Number of clock cycles: " << clockCycleCounter << endl;
	cout << "Average latency per instruction: " << avgLatency << endl;
	cout << "Average throughput per instruction: " << avgThroughput << endl;
}

void Statistics::clearStatistics() {
	latencyVector.clear();
	throughputVector.clear();
	clockCycleCounter = 0;
	throughputCounter = 0;
	avgLatency = 0.0;
	avgThroughput = 0.0;
}
