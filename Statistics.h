#pragma once
#include <vector>
#include <tuple>
#include <iostream>

using namespace std;

class Statistics {
private:
	int clockCycleCounter = 0;
	int throughputCounter = 0;
	double avgLatency = 0.0;
	double avgThroughput = 0.0;
	vector<tuple<int, int, int>> latencyVector; //enters, exits, ROB_ID
	vector<int> throughputVector;
	void caclulateAvgLatency();
	void calculateAvgThroughput();
public:
	void ROBIDEntersPipeline(int ROB_ID);
	void ROBIDExitsPipeline(int ROB_ID);
	void incrementThroughputCounter();
	void throughputDoneThisCycle();
	void incrementClockCycle();
	void printStatistics();
	void clearStatistics();
};

