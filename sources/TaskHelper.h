#ifndef SCHEDULERPROJECT_TASKHELPER_H
#define SCHEDULERPROJECT_TASKHELPER_H

#include <vector>
#include "Deque.h"
#include "Task.h"
#include "Worker.h"

std::vector<Task> Build_RandomScenario(int numberOfTasks, int numberOfWorkers, int minTime, int maxTime, int minPriority, int maxPriority, unsigned int seed);
void Sort_TasksByPriority(std::vector<Task>& tasks);
void Populate_WorkerQueues(std::vector<Worker>& workers, const std::vector<Task>& scenarioTasks, int& totalInitialWork, int maxPriority);

#endif //SCHEDULERPROJECT_TASKHELPER_H
