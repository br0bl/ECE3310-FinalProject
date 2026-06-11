#include "Simulation.h"
#include <iostream>

void runWorkersOneCycle(std::vector<Worker> &workers) {
    for (size_t i = 0; i < workers.size(); i++) {
        workers[i].runOneCycle();
    }
}

bool allWorkFinished(const std::vector<Worker> &workers) {
    for (size_t i = 0; i < workers.size(); i++) {
        if (!workers[i].idle()) {
            return false;
        }
    }
    return true;
}

bool allMaxPriorityWorkFinished(const std::vector<Worker>& workers, int maxPriority) {
    for (size_t i = 0; i < workers.size(); i++) {
        if (workers[i].hasMaxPriorityTask(maxPriority)) {
            return false;
        }
    }

    return true;
}

void printSummary(const std::vector<Worker> &workers, const Cycle& cycle) {
    int totalCompleted = 0;
    std::cout << "Worker Summary:\n";
    for (size_t i = 0; i < workers.size(); i++) {
        std::cout << "Worker: " << workers[i].getID()
        << " | Completed: " << workers[i].getCompletedTasks()
        << " | Stole: " << workers[i].getStolenTasks()
        << " | Stolen: " << workers[i].getTasksStolen()
        << " | Initial Max Priority: " << workers[i].getInitialMaxPriorityTasks()
        << " | Max Priority Stole: " << workers[i].getMaxPriorityTasksStole()
        << " | Max Priority Stolen: " << workers[i].getMaxPriorityTasksStolen()
        << std::endl;

        totalCompleted += workers[i].getCompletedTasks();
    }

    std::cout << "Total completed tasks: " << totalCompleted
    << "    | Total cycles completed: " << cycle.getCurrentCycle()
    << "    | Cycles until all priority tasks finished: " << cycle.getMaxPriorityFinishedCycle()
    << std::endl;
}

void runSimulation_IWS(std::vector<Worker> &workers, Cycle &cycle, const int& maxPriority) {
    if (!cycle.maxPriorityFinishedCycleRecorded() && allMaxPriorityWorkFinished(workers, maxPriority)) {
        cycle.setMaxPriorityFinishedCycle(cycle.getCurrentCycle());
    }

    while (!cycle.done()) {

        // Idle workers try to steal
        for (size_t i = 0; i < workers.size(); i++) {
            if (workers[i].idle()) {
                for (size_t j = 0; j < workers.size(); j++) {
                    if (j == i) {
                        continue;
                    }

                    if (workers[j].hasQueuedTasks()) {
                        if (workers[i].stealOneTaskFrom(workers[j])) {
                            break;
                        }
                    }
                }
            }
        }

        // Run every worker one cycle
        for (size_t i = 0; i < workers.size(); i++) {
            workers[i].runOneCycle();
        }

        cycle.advance();

        if (!cycle.maxPriorityFinishedCycleRecorded() && allMaxPriorityWorkFinished(workers, maxPriority)) {
            cycle.setMaxPriorityFinishedCycle(cycle.getCurrentCycle());
        }

        bool finished = true;

        // Check if all work is complete
        for (size_t i = 0; i < workers.size(); i++) {
            if (!workers[i].idle()) {
                finished = false;
            }
        }

        if (finished) {
            break;
        }
    }
}

void resetWorkers(std::vector<Worker>& workers) {
    for (size_t i = 0; i < workers.size(); i++) {
        workers[i].reset();
    }
}

void runSimulation_PWS(std::vector<Worker> &workers, Cycle &cycle, const int &maxPriority) {
    if (!cycle.maxPriorityFinishedCycleRecorded() && allMaxPriorityWorkFinished(workers, maxPriority)) {
        cycle.setMaxPriorityFinishedCycle(cycle.getCurrentCycle());
    }

    while (!cycle.done()) {

        // Priority steal
        for (size_t i = 0; i < workers.size(); i++) {
            if (workers[i].hasMaxPriorityTask(maxPriority)) {
                continue;
            }

            for (size_t j = 0; j < workers.size(); j++) {
                if (i == j) {
                    continue;
                }

                if (workers[i].stealMaxPriorityTaskFrom(workers[j], maxPriority)) {
                    break;
                }
            }
        }

        // Idle steal
        for (size_t i = 0; i < workers.size(); i++) {
            if (workers[i].idle()) {
                for (size_t j = 0; j < workers.size(); j++) {
                    if (j == i) {
                        continue;
                    }

                    if (workers[j].hasQueuedTasks()) {
                        if (workers[i].stealOneTaskFrom(workers[j])) {
                            break;
                        }
                    }
                }
            }
        }

        // Run every worker one cycle
        for (size_t i = 0; i < workers.size(); i++) {
            workers[i].runOneCycle();
        }

        cycle.advance();

        if (!cycle.maxPriorityFinishedCycleRecorded() && allMaxPriorityWorkFinished(workers, maxPriority)) {
            cycle.setMaxPriorityFinishedCycle(cycle.getCurrentCycle());
        }

        bool finished = true;

        // Check if all work is complete
        for (size_t i = 0; i < workers.size(); i++) {
            if (!workers[i].idle()) {
                finished = false;
            }
        }

        if (finished) {
            break;
        }
    }
}

void runSimulation_NWS(std::vector<Worker>& workers, Cycle& cycle, const int& maxPriority) {
    if (!cycle.maxPriorityFinishedCycleRecorded() && allMaxPriorityWorkFinished(workers, maxPriority)) {
        cycle.setMaxPriorityFinishedCycle(cycle.getCurrentCycle());
    }

    while (!cycle.done()) {
        for (size_t i = 0; i < workers.size(); i++) {
            workers[i].runOneCycle();
        }

        cycle.advance();

        if (!cycle.maxPriorityFinishedCycleRecorded() && allMaxPriorityWorkFinished(workers, maxPriority)) {
            cycle.setMaxPriorityFinishedCycle(cycle.getCurrentCycle());
        }

        if (allWorkFinished(workers)) {
            break;
        }
    }
}

void demoSimulation_IWS(std::vector<Worker> &workers, Cycle &cycle, const int& maxPriority) {
    if (!cycle.maxPriorityFinishedCycleRecorded() && allMaxPriorityWorkFinished(workers, maxPriority)) {
        cycle.setMaxPriorityFinishedCycle(cycle.getCurrentCycle());
    }

    while (!cycle.done()) {
        std::cout << "\nCycle " << cycle.getCurrentCycle() << std::endl;

        std::cout << "Before stealing:" << std::endl;
        for (size_t i = 0; i < workers.size(); i++) {
            workers[i].displayCurrentTask();
            workers[i].displayQueue();
        }
        std::cout << std::endl;

        // Idle workers try to steal
        for (size_t i = 0; i < workers.size(); i++) {
            if (workers[i].idle()) {
                for (size_t j = 0; j < workers.size(); j++) {
                    if (j == i) {
                        continue;
                    }

                    if (workers[j].hasQueuedTasks()) {
                        if (workers[i].stealOneTaskFrom(workers[j])) {
                            break;
                        }
                    }
                }
            }
        }

        std::cout << "After stealing:" << std::endl;
        for (size_t i = 0; i < workers.size(); i++) {
            workers[i].displayCurrentTask();
            workers[i].displayQueue();
        }
        std::cout << std::endl;

        // Run every worker one cycle
        for (size_t i = 0; i < workers.size(); i++) {
            workers[i].runOneCycle();
        }

        std::cout << "After running:" << std::endl;
        for (size_t i = 0; i < workers.size(); i++) {
            workers[i].displayCurrentTask();
            workers[i].displayQueue();
        }

        cycle.advance();

        if (!cycle.maxPriorityFinishedCycleRecorded() && allMaxPriorityWorkFinished(workers, maxPriority)) {
            cycle.setMaxPriorityFinishedCycle(cycle.getCurrentCycle());
        }

        bool finished = true;

        // Check if all work is complete
        for (size_t i = 0; i < workers.size(); i++) {
            if (!workers[i].idle()) {
                finished = false;
            }
        }

        if (finished) {
            break;
        }
    }
}

void demoSimulation_PWS(std::vector<Worker> &workers, Cycle &cycle, const int &maxPriority) {
    if (!cycle.maxPriorityFinishedCycleRecorded() && allMaxPriorityWorkFinished(workers, maxPriority)) {
        cycle.setMaxPriorityFinishedCycle(cycle.getCurrentCycle());
    }

    while (!cycle.done()) {
        std::cout << "\nCycle " << cycle.getCurrentCycle() << std::endl;

        std::cout << "Before stealing:" << std::endl;
        for (size_t i = 0; i < workers.size(); i++) {
            workers[i].displayCurrentTask();
            workers[i].displayQueue();
        }
        std::cout << std::endl;

        // Priority steal
        for (size_t i = 0; i < workers.size(); i++) {
            if (workers[i].hasMaxPriorityTask(maxPriority)) {
                continue;
            }

            for (size_t j = 0; j < workers.size(); j++) {
                if (i == j) {
                    continue;
                }

                if (workers[i].stealMaxPriorityTaskFrom(workers[j], maxPriority)) {
                    break;
                }
            }
        }

        // Idle steal
        for (size_t i = 0; i < workers.size(); i++) {
            if (workers[i].idle()) {
                for (size_t j = 0; j < workers.size(); j++) {
                    if (j == i) {
                        continue;
                    }

                    if (workers[j].hasQueuedTasks()) {
                        if (workers[i].stealOneTaskFrom(workers[j])) {
                            break;
                        }
                    }
                }
            }
        }

        std::cout << "After stealing:" << std::endl;
        for (size_t i = 0; i < workers.size(); i++) {
            workers[i].displayCurrentTask();
            workers[i].displayQueue();
        }
        std::cout << std::endl;

        // Run every worker one cycle
        for (size_t i = 0; i < workers.size(); i++) {
            workers[i].runOneCycle();
        }

        std::cout << "After running:" << std::endl;
        for (size_t i = 0; i < workers.size(); i++) {
            workers[i].displayCurrentTask();
            workers[i].displayQueue();
        }

        cycle.advance();

        if (!cycle.maxPriorityFinishedCycleRecorded() && allMaxPriorityWorkFinished(workers, maxPriority)) {
            cycle.setMaxPriorityFinishedCycle(cycle.getCurrentCycle());
        }

        bool finished = true;

        // Check if all work is complete
        for (size_t i = 0; i < workers.size(); i++) {
            if (!workers[i].idle()) {
                finished = false;
            }
        }

        if (finished) {
            break;
        }
    }
}

void demoSimulation_NWS(std::vector<Worker>& workers, Cycle& cycle, const int& maxPriority) {
    if (!cycle.maxPriorityFinishedCycleRecorded() && allMaxPriorityWorkFinished(workers, maxPriority)) {
        cycle.setMaxPriorityFinishedCycle(cycle.getCurrentCycle());
    }

    while (!cycle.done()) {
        std::cout << "\nCycle " << cycle.getCurrentCycle() << std::endl;

        std::cout << "Before running:" << std::endl;
        for (size_t i = 0; i < workers.size(); i++) {
            workers[i].displayCurrentTask();
            workers[i].displayQueue();
        }
        std::cout << std::endl;

        for (size_t i = 0; i < workers.size(); i++) {
            workers[i].runOneCycle();
        }

        std::cout << "After running:" << std::endl;
        for (size_t i = 0; i < workers.size(); i++) {
            workers[i].displayCurrentTask();
            workers[i].displayQueue();
        }
        std::cout << std::endl;

        cycle.advance();

        if (!cycle.maxPriorityFinishedCycleRecorded() && allMaxPriorityWorkFinished(workers, maxPriority)) {
            cycle.setMaxPriorityFinishedCycle(cycle.getCurrentCycle());
        }

        if (allWorkFinished(workers)) {
            break;
        }
    }
}