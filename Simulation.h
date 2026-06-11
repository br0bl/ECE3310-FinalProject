#ifndef SCHEDULERPROJECT_SIMULATION_H
#define SCHEDULERPROJECT_SIMULATION_H

#include <vector>

#include "Worker.h"
#include "Task.h"
#include "Cycle.h"

void runWorkersOneCycle(std::vector<Worker>& workers);
bool allWorkFinished(const std::vector<Worker>& workers);
void printSummary(const std::vector<Worker>& workers, const Cycle& cycle);

void runSimulation_IWS(std::vector<Worker>& workers, Cycle& cycle, const int& maxPriority);
void resetWorkers(std::vector<Worker>& workers);

void runSimulation_PWS(std::vector<Worker>& workers, Cycle& cycle, const int& maxPriority);

bool allMaxPriorityWorkFinished(const std::vector<Worker>& workers, int maxPriority);

void runSimulation_NWS(std::vector<Worker>& workers, Cycle& cycle, const int& maxPriority);

void demoSimulation_IWS(std::vector<Worker>& workers, Cycle& cycle, const int& maxPriority);
void demoSimulation_PWS(std::vector<Worker>& workers, Cycle& cycle, const int& maxPriority);
void demoSimulation_NWS(std::vector<Worker>& workers, Cycle& cycle, const int& maxPriority);

#endif //SCHEDULERPROJECT_SIMULATION_H
