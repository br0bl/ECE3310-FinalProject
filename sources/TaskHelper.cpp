#include "TaskHelper.h"
#include <random>

std::vector<Task> Build_RandomScenario(int numberOfTasks, int numberOfWorkers, int minTime, int maxTime, int minPriority, int maxPriority, unsigned int seed) {
    std::vector<Task> tasks;

    std::mt19937 generator(seed);

    std::uniform_int_distribution<int> timeDistribution(minTime, maxTime);
    std::uniform_int_distribution<int> typeDistribution(0, numberOfWorkers - 1);
    std::uniform_int_distribution<int> priorityDistribution(minPriority, maxPriority);

    for (int i = 0; i < numberOfTasks; i++) {
        int taskID = 1 + i;
        int remainingTime = timeDistribution(generator);
        int taskType = typeDistribution(generator);
        int priority = priorityDistribution(generator);
        int initialWorker = taskType;

        tasks.push_back(Task(taskID, remainingTime, taskType, priority, initialWorker));
    }

    return tasks;
}

void Sort_TasksByPriority(std::vector<Task>& tasks) {
    for (size_t i = 1; i < tasks.size(); i++) {
        Task tempTask = tasks[i];
        size_t j = i;

        while (j > 0 && tempTask.priority > tasks[j - 1].priority) {
            tasks[j] = tasks[j - 1];
            j--;
        }

        tasks[j] = tempTask;
    }
}

void Populate_WorkerQueues(std::vector<Worker>& workers, const std::vector<Task>& scenarioTasks, int& totalInitialWork, int maxPriority) {
    totalInitialWork = 0;

    for (size_t i = 0; i < scenarioTasks.size(); i++) {
        Task task = scenarioTasks[i];

        totalInitialWork += task.remainingTime;

        for (size_t j = 0; j < workers.size(); j++) {
            if (task.taskType == workers[j].getSpecialty()) {
                task.initialWorker = workers[j].getID();
                workers[j].addInitialTask(task, maxPriority);
                break;
            }
        }
    }
}
