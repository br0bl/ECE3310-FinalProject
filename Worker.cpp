#include "Worker.h"
#include <iostream>

Worker::Worker(int id, int specialty)
    : workerID(id), taskQueue(), hasCurrentTask(false), currentTask(), completedTasks(0), stolenTasks(0), tasksStolen(0),
      initialMaxPriorityTasks(0), maxPriorityTasksStole(0), maxPriorityTasksStolen(0), specialty(specialty) {
}

int Worker::getID() const {
    return workerID;
}

bool Worker::idle() const {
    return !hasCurrentTask && taskQueue.empty();
}

size_t Worker::queueSize() const {
    return taskQueue.get_size();
}

int Worker::getCompletedTasks() const {
    return completedTasks;
}

void Worker::addTask(const Task& task) {
    taskQueue.push_back(task);
}

void Worker::addInitialTask(const Task& task, int maxPriority) {
    taskQueue.push_back(task);

    if (task.priority == maxPriority) {
        initialMaxPriorityTasks++;
    }
}

bool Worker::loadNextTask() {
    if (hasCurrentTask) {
        return false;
    }

    if (taskQueue.pop_front(currentTask)) {
        hasCurrentTask = true;
        return true;
    }

    return false;
}

bool Worker::runOneCycle() {
    if (!hasCurrentTask) {
        if (!loadNextTask()) {
            return false;
        }
    }

    if (currentTask.taskType == specialty) {
        currentTask.remainingTime -= 2;
    }
    else {
        currentTask.remainingTime -= 1;
    }

    if (currentTask.remainingTime <= 0) {
        completedTasks++;
        hasCurrentTask = false;
    }

    return true;
}

void Worker::displayQueue() const {
    taskQueue.display();
}

void Worker::displayCurrentTask() const {
    if (hasCurrentTask) {
        std::cout << "Worker " << workerID << " running " << currentTask << std::endl;
    }
    else {
        std::cout << "Worker " << workerID << " is idle" << std::endl;
    }
}

bool Worker::hasQueuedTasks() const {
    return !taskQueue.empty();
}

bool Worker::hasStealableTask() const {
    if (hasCurrentTask) {
        return !taskQueue.empty();
    }

    return taskQueue.get_size() > 1;
}

bool Worker::stealOneTaskFrom(Worker &victim) {
    if (workerID == victim.getID()) {
        return false;
    }

    if (!victim.hasStealableTask()) {
        return false;
    }

    Task stolenTask;

    if (victim.taskQueue.steal_back(stolenTask)) {
        taskQueue.push_front(stolenTask);
        stolenTasks++;
        victim.tasksStolen++;
        return true;
    }

    return false;
}

int Worker::getStolenTasks() const {
    return stolenTasks;
}

int Worker::getTasksStolen() const {
    return tasksStolen;
}

void Worker::reset() {
    taskQueue = Deque();
    hasCurrentTask = false;
    currentTask = Task();
    completedTasks = 0;
    stolenTasks = 0;
    tasksStolen = 0;
    initialMaxPriorityTasks = 0;
    maxPriorityTasksStole = 0;
    maxPriorityTasksStolen = 0;
}

int Worker::getSpecialty() const {
    return specialty;
}

bool Worker::hasMaxPriorityTask(int maxPriority) const {
    if (hasCurrentTask && currentTask.priority == maxPriority) {
        return true;
    }

    if (taskQueue.hasPriority(maxPriority)) {
        return true;
    }

    return false;
}

bool Worker::stealMaxPriorityTaskFrom(Worker& victim, int maxPriority) {
    if (workerID == victim.getID()) {
        return false;
    }

    if (hasCurrentTask) {
        return false;
    }

    if (hasMaxPriorityTask(maxPriority)) {
        return false;
    }

    Task taskToSteal;
    int victimQueueCyclesUntilTask = 0;

    if (!victim.taskQueue.peekLastPriorityWithWork(taskToSteal, maxPriority, victimQueueCyclesUntilTask, victim.getSpecialty())) {
        return false;
    }

    int victimCurrentCycles = 0;

    if (victim.hasCurrentTask) {
        victimCurrentCycles = Estimate_TaskCycles(victim.currentTask, victim.getSpecialty());
    }

    int totalVictimCyclesUntilTask = victimCurrentCycles + victimQueueCyclesUntilTask;

    int thiefCyclesToRunTask = Estimate_TaskCycles(taskToSteal, specialty);

    if (totalVictimCyclesUntilTask <= thiefCyclesToRunTask) {
        return false;
    }

    if (victim.taskQueue.removeLastPriority(taskToSteal, maxPriority)) {
        taskQueue.push_front(taskToSteal);

        stolenTasks++;
        victim.tasksStolen++;

        maxPriorityTasksStole++;
        victim.maxPriorityTasksStolen++;

        return true;
    }

    return false;
}

int Worker::getInitialMaxPriorityTasks() const {
    return initialMaxPriorityTasks;
}

int Worker::getMaxPriorityTasksStole() const {
    return maxPriorityTasksStole;
}

int Worker::getMaxPriorityTasksStolen() const {
    return maxPriorityTasksStolen;
}
