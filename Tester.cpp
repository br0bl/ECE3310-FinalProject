#include "Tester.h"
#include <iostream>
#include "TaskHelper.h"
#include "Simulation.h"
#include "Cycle.h"

typedef void (*SimulationFunction)(std::vector<Worker>& workers, Cycle& cycle, const int& maxPriority);

int Get_LeastPossibleCycles(int totalWork, int numWorkers) {
    int maxWorkPerCycle = 2 * numWorkers;

    return (totalWork + maxWorkPerCycle - 1) / maxWorkPerCycle;
}

SimulationResults Run_SimulationTest(std::vector<Worker>& workers, Cycle& simClock, const std::vector<Task>& scenarioTasks, int maxPriority, SimulationFunction simulationFunction) {
    resetWorkers(workers);
    simClock.reset();

    int totalInitialWork = 0;

    Populate_WorkerQueues(workers, scenarioTasks, totalInitialWork, maxPriority);

    simulationFunction(workers, simClock, maxPriority);

    SimulationResults result;

    result.totalCycles = simClock.getCurrentCycle();
    result.priorityFinishedCycle = simClock.getMaxPriorityFinishedCycle();

    return result;
}

TestResults Run_AllSimulations(int seed, Cycle& simClock, std::vector<Worker>& workers,  const std::vector<Task>& scenarioTasks, int maxPriority, int numWorkers) {

    TestResults test;
    test.seed = seed;

    test.nws = Run_SimulationTest(workers, simClock, scenarioTasks,  maxPriority, runSimulation_NWS);
    test.iws = Run_SimulationTest(workers, simClock, scenarioTasks,  maxPriority, runSimulation_IWS);
    test.pws = Run_SimulationTest(workers, simClock, scenarioTasks, maxPriority, runSimulation_PWS);

    test.totalInitialWork = 0;

    for (size_t i = 0; i < scenarioTasks.size(); i++) {
        test.totalInitialWork += scenarioTasks[i].remainingTime;
    }

    test.leastCyclesPossible = Get_LeastPossibleCycles(test.totalInitialWork, numWorkers);
    test.mostCyclesPossible = test.totalInitialWork;

    return test;
}

void Initialize_TestSummary(TestSummary& stats) {

    stats.totalLeastPossibleCycles = 0;
    stats.totalMostPossibleCycles = 0;

    stats.totalNWSCycles = 0;
    stats.totalIWSCycles = 0;
    stats.totalPWSCycles = 0;

    stats.iwsDecreasedTotalFromNWS = 0;

    stats.pwsDecreasedTotalFromNWS = 0;
    stats.pwsDecreasedTotalFromIWS = 0;

    stats.pwsTookMoreCyclesThanIWS = 0;
    stats.pwsTookMoreCyclesThanNWS = 0;

    stats.pwsDecreasedPriorityFromBaseline = 0;

    stats.totalCycleSavingsFromIWS = 0;

    stats.totalCycleSavingsWhenImproved = 0;
    stats.testsWherePWSTotalImproved = 0;

    stats.totalCycleLossWhenWorse = 0;
    stats.testsWherePWSTotalWorse = 0;

    stats.totalPriorityCycleSavingsFromBaseline = 0;
    stats.priorityComparisonCount = 0;

    stats.totalPrioritySavingsWhenImproved = 0;
    stats.testsWherePWSPriorityImproved = 0;

    stats.largestTotalCycleImprovement = 0;
    stats.largestTotalCycleLoss = 0;
    stats.largestPriorityCycleImprovement = 0;

    stats.testsProcessed = 0;
}

void Update_TestSummary(TestSummary& stats, const TestResults& test) {

    // Total cycle counts
    stats.totalLeastPossibleCycles += test.leastCyclesPossible;
    stats.totalMostPossibleCycles += test.mostCyclesPossible;
    stats.totalNWSCycles += test.nws.totalCycles;
    stats.totalIWSCycles += test.iws.totalCycles;
    stats.totalPWSCycles += test.pws.totalCycles;

    // IWS improved NWS
    if (test.iws.totalCycles < test.nws.totalCycles) {
        stats.iwsDecreasedTotalFromNWS++;
    }

    // PWS improved NWS
    if (test.pws.totalCycles < test.nws.totalCycles) {
        stats.pwsDecreasedTotalFromNWS++;
    }

    // PWS improved IWS
    if (test.pws.totalCycles < test.iws.totalCycles) {
        stats.pwsDecreasedTotalFromIWS++;
    }

    // PWS worse than IWS
    if (test.pws.totalCycles > test.iws.totalCycles) {
        stats.pwsTookMoreCyclesThanIWS++;
    }

    // PWS worse than NWS
    if (test.pws.totalCycles > test.nws.totalCycles) {
        stats.pwsTookMoreCyclesThanNWS++;
    }

    // PWS improved priority
    if (test.pws.priorityFinishedCycle >= 0 &&
    test.nws.priorityFinishedCycle >= 0 &&
    test.pws.priorityFinishedCycle < test.nws.priorityFinishedCycle) {
        stats.pwsDecreasedPriorityFromBaseline++;
    }

    // Delta cycles PWS to IWS
    int totalCycleImprovement = test.iws.totalCycles - test.pws.totalCycles;
    stats.totalCycleSavingsFromIWS += totalCycleImprovement;

    // Totals PWS improved IWS
    if (totalCycleImprovement > 0) {
        stats.totalCycleSavingsWhenImproved += totalCycleImprovement;
        stats.testsWherePWSTotalImproved++;
    }

    // Totals PWS worse than IWS
    if (totalCycleImprovement < 0) {
        stats.totalCycleLossWhenWorse += totalCycleImprovement * -1;
        stats.testsWherePWSTotalWorse++;
    }

    if (test.pws.priorityFinishedCycle >= 0 && test.nws.priorityFinishedCycle >= 0) {
        // Delta total priority cycles
        int priorityCycleImprovement = test.nws.priorityFinishedCycle - test.pws.priorityFinishedCycle;
        stats.totalPriorityCycleSavingsFromBaseline += priorityCycleImprovement;
        stats.priorityComparisonCount++;

        // Improved priority cycles
        if (priorityCycleImprovement > 0) {
            stats.totalPrioritySavingsWhenImproved += priorityCycleImprovement;
            stats.testsWherePWSPriorityImproved++;
        }

        // Update largest improvement
        if (priorityCycleImprovement > stats.largestPriorityCycleImprovement) {
            stats.largestPriorityCycleImprovement = priorityCycleImprovement;
        }
    }

    //  Update largest cycle deltas
    if (stats.testsProcessed == 0) {
        stats.largestTotalCycleImprovement = totalCycleImprovement;

        if (totalCycleImprovement < 0) {
            stats.largestTotalCycleLoss = totalCycleImprovement * -1;
        }
        else {
            stats.largestTotalCycleLoss = 0;
        }
    }
    else {
        if (totalCycleImprovement > stats.largestTotalCycleImprovement) {
            stats.largestTotalCycleImprovement = totalCycleImprovement;
        }

        if (totalCycleImprovement < 0 && totalCycleImprovement * -1 > stats.largestTotalCycleLoss) {
            stats.largestTotalCycleLoss = totalCycleImprovement * -1;
        }
    }
    stats.testsProcessed++;
}

void Print_TestResult(const TestResults& test) {
    std::cout << "Test " << test.seed << ":" << std::endl;

    std::cout << "  Total Work: " << test.totalInitialWork
              << " | Least Cycles Possible: " << test.leastCyclesPossible
              << " | Most Cycles Possible: " << test.mostCyclesPossible << std::endl;

    std::cout << "  NWS | Total Cycles: " << test.nws.totalCycles
              << " | Priority Finished: " << test.nws.priorityFinishedCycle << std::endl;

    std::cout << "  IWS | Total Cycles: " << test.iws.totalCycles
              << " | Priority Finished: " << test.iws.priorityFinishedCycle << std::endl;

    std::cout << "  PWS | Total Cycles: " << test.pws.totalCycles
              << " | Priority Finished: " << test.pws.priorityFinishedCycle << std::endl << std::endl;
}

void Print_FinalTestSummary(const TestSummary& stats, int numberOfTests) {

    std::cout << "Tests where IWS decreased total cycles from NWS: "
              << stats.iwsDecreasedTotalFromNWS << " / " << numberOfTests << std::endl;

    std::cout << std::endl;

    std::cout << "Tests where PWS decreased total cycles from NWS: "
              << stats.pwsDecreasedTotalFromNWS << " / " << numberOfTests << std::endl;

    std::cout << "Tests where PWS decreased total cycles from IWS: "
              << stats.pwsDecreasedTotalFromIWS << " / " << numberOfTests << std::endl;

    if (stats.testsWherePWSTotalImproved > 0) {
        std::cout << "Average cycles saved when PWS improved over IWS: "
                  << static_cast<double>(stats.totalCycleSavingsWhenImproved) / stats.testsWherePWSTotalImproved << std::endl;
    }

    std::cout << "Largest total-cycle improvement by PWS over IWS: "
          << stats.largestTotalCycleImprovement << std::endl;

    std::cout << std::endl;

    std::cout << "Tests where PWS took more total cycles than NWS: "
          << stats.pwsTookMoreCyclesThanNWS << " / " << numberOfTests << std::endl;

    std::cout << "Tests where PWS took more total cycles than IWS: "
              << stats.pwsTookMoreCyclesThanIWS << " / " << numberOfTests << std::endl;

    std::cout << "Largest total-cycle loss by PWS compared to IWS: "
          << stats.largestTotalCycleLoss << std::endl;

    if (stats.testsWherePWSTotalWorse > 0) {
        std::cout << "Average cycles lost when PWS was worse: "
                  << static_cast<double>(stats.totalCycleLossWhenWorse) / stats.testsWherePWSTotalWorse << std::endl;
    }

    std::cout << std::endl;

    std::cout << "Average cycles saved by PWS compared to IWS: "
      << static_cast<double>(stats.totalCycleSavingsFromIWS) / numberOfTests << std::endl;

    std::cout << std::endl;

    std::cout << "Tests where PWS decreased priority-finished cycles from baseline: "
          << stats.pwsDecreasedPriorityFromBaseline << " / " << numberOfTests << std::endl;

    if (stats.priorityComparisonCount > 0) {
        std::cout << "Average priority cycles saved by PWS compared to baseline: "
                  << static_cast<double>(stats.totalPriorityCycleSavingsFromBaseline) / stats.priorityComparisonCount << std::endl;
    }

    if (stats.testsWherePWSPriorityImproved > 0) {
        std::cout << "Average priority cycles saved when PWS improved priority completion: "
                  << static_cast<double>(stats.totalPrioritySavingsWhenImproved) / stats.testsWherePWSPriorityImproved << std::endl;
    }

    std::cout << "Largest priority-cycle improvement by PWS: "
          << stats.largestPriorityCycleImprovement << std::endl;

    std::cout << std::endl;

    std::cout << "Average Least Cycles Possible: "
              << static_cast<double>(stats.totalLeastPossibleCycles) / numberOfTests << std::endl;

    std::cout << "Average Most Cycles Possible: "
              << static_cast<double>(stats.totalMostPossibleCycles) / numberOfTests << std::endl;

    std::cout << "Average NWS Total Cycles: "
              << static_cast<double>(stats.totalNWSCycles) / numberOfTests << std::endl;

    std::cout << "Average IWS Total Cycles: "
              << static_cast<double>(stats.totalIWSCycles) / numberOfTests << std::endl;

    std::cout << "Average PWS Total Cycles: "
              << static_cast<double>(stats.totalPWSCycles) / numberOfTests << std::endl;
}

void Run_Tester(std::vector<Worker>& workers, Cycle& simClock, int numberOfTests, int numberOfTasks,
                int startingSeed, int numWorkers, int minTime, int maxTime, int minPriority, int maxPriority) {

    TestSummary stats;

    Initialize_TestSummary(stats);

    std::cout << "\nRunning " << numberOfTests << " tests..." << std::endl << std::endl;

    for (int testNumber = 0; testNumber < numberOfTests; testNumber++) {
        int seed = startingSeed + testNumber;

        std::vector<Task> scenarioTasks = Build_RandomScenario(numberOfTasks, numWorkers, minTime, maxTime, minPriority, maxPriority, seed);

        Sort_TasksByPriority(scenarioTasks);

        TestResults test = Run_AllSimulations(seed, simClock, workers, scenarioTasks, maxPriority, numWorkers);

        // Print_TestResult(test);

        Update_TestSummary(stats, test);
    }

    Print_FinalTestSummary(stats, numberOfTests);
}
