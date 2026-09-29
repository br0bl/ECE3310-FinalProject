#ifndef SCHEDULERPROJECT_TESTER_H
#define SCHEDULERPROJECT_TESTER_H

#include <vector>
#include "Task.h"
#include "Worker.h"
#include "Cycle.h"

struct SimulationResults {
    int totalCycles;
    int priorityFinishedCycle;
};

struct TestResults {
    int seed;

    int totalInitialWork;
    int leastCyclesPossible;
    int mostCyclesPossible;

    SimulationResults nws;
    SimulationResults iws;
    SimulationResults pws;
};

struct TestSummary {
    int totalLeastPossibleCycles;
    int totalMostPossibleCycles;

    int totalNWSCycles;
    int totalIWSCycles;
    int totalPWSCycles;

    int iwsDecreasedTotalFromNWS;

    int pwsDecreasedTotalFromNWS;
    int pwsDecreasedTotalFromIWS;

    int pwsTookMoreCyclesThanIWS;
    int pwsTookMoreCyclesThanNWS;

    int pwsDecreasedPriorityFromBaseline;

    int totalCycleSavingsFromIWS;

    int totalCycleSavingsWhenImproved;
    int testsWherePWSTotalImproved;

    int totalCycleLossWhenWorse;
    int testsWherePWSTotalWorse;

    int totalPriorityCycleSavingsFromBaseline;
    int priorityComparisonCount;

    int totalPrioritySavingsWhenImproved;
    int testsWherePWSPriorityImproved;

    int largestTotalCycleImprovement;
    int largestTotalCycleLoss;
    int largestPriorityCycleImprovement;

    int testsProcessed;
};

int Get_LeastPossibleCycles(int totalWork, int numWorkers);

void Initialize_TestSummary(TestSummary& stats);
void Update_TestSummary(TestSummary& stats, const TestResults& test);

void Print_TestResult(const TestResults& test);
void Print_FinalTestSummary(const TestSummary& stats, int numberOfTests);

void Run_Tester(std::vector<Worker>& workers, Cycle& simClock, int numberOfTests, int numberOfTasks,
                int startingSeed, int numWorkers, int minTime, int maxTime, int minPriority, int maxPriority);

#endif //SCHEDULERPROJECT_TESTER_H
