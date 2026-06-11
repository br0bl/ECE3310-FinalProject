#ifndef SCHEDULERPROJECT_WORKER_H
#define SCHEDULERPROJECT_WORKER_H

#include "Deque.h"

class Worker {
private:
    int workerID;
    Deque taskQueue;

    bool hasCurrentTask;
    Task currentTask;

    int completedTasks;
    int stolenTasks;
    int tasksStolen;

    int initialMaxPriorityTasks;
    int maxPriorityTasksStole;
    int maxPriorityTasksStolen;

    int specialty;

public:
    Worker(int id = 0, int specialty = 0);

    int getID() const;
    bool idle() const;
    size_t queueSize() const;
    int getCompletedTasks() const;

    void addTask(const Task& task);
    bool loadNextTask();
    bool runOneCycle();

    void displayQueue() const;
    void displayCurrentTask() const;

    bool hasQueuedTasks() const;
    bool hasStealableTask() const;
    bool stealOneTaskFrom(Worker& victim);

    int getStolenTasks() const;
    int getTasksStolen() const;

    void reset();

    int getSpecialty() const;

    bool hasMaxPriorityTask(int maxPriority) const;
    bool stealMaxPriorityTaskFrom(Worker& victim, int maxPriority);

    void addInitialTask(const Task& task, int maxPriority);

    int getInitialMaxPriorityTasks() const;
    int getMaxPriorityTasksStole() const;
    int getMaxPriorityTasksStolen() const;
};

#endif
