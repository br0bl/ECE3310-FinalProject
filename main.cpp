#include <iostream>
#include <vector>

#include "Worker.h"
#include "Cycle.h"
#include "TaskHelper.h"
#include "Simulation.h"
#include "Tester.h"

using namespace std;

const int numWorkers = 4;

const int numberOfTests = 1000;
const int startingSeed = 1;

const int numberOfTasks = 50;
const int minTime = 3;
const int maxTime = 40;
const int minPriority = 1;
const int maxPriority = 5;

const int maxCycles = 1000;
bool demo = true;

int main() {

    vector<Worker> workers;
    workers.reserve(numWorkers);
    for (int i=0; i < numWorkers; i++) {
        workers.push_back(Worker(i,i));
    }

    Cycle simClock(maxCycles);

    if (demo) {
        vector<Task> scenarioTasks = Build_RandomScenario(8, numWorkers, 1, 4, 1, maxPriority, 1);
        Sort_TasksByPriority(scenarioTasks);

        int totalInitialWork = 0;
        Populate_WorkerQueues(workers, scenarioTasks, totalInitialWork, maxPriority);
        cout << "Initial Total Work: " << totalInitialWork << endl << endl;
        cout << "No work stealing:\n";
        demoSimulation_NWS(workers, simClock, maxPriority);
        cout << "\n\n\n";

        resetWorkers(workers);
        simClock.reset();

        Populate_WorkerQueues(workers, scenarioTasks, totalInitialWork, maxPriority);
        cout << "Idle work stealing:\n";
        demoSimulation_IWS(workers, simClock, maxPriority);
        cout << "\n\n\n";

        resetWorkers(workers);
        simClock.reset();

        Populate_WorkerQueues(workers, scenarioTasks, totalInitialWork, maxPriority);
        cout << "Priority work stealing:\n";
        demoSimulation_PWS(workers, simClock, maxPriority);
        cout << "\n\n";
    }

    Run_Tester(workers, simClock,
               numberOfTests, numberOfTasks,
               startingSeed,
               numWorkers,
               minTime, maxTime,
               minPriority, maxPriority);
}
