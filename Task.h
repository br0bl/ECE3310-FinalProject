#ifndef SCHEDULERPROJECT_TASK_H
#define SCHEDULERPROJECT_TASK_H

#include <iostream>

struct Task {
    int taskID;
    int remainingTime;
    int taskType;
    int priority;
    int initialWorker;

    Task() : taskID(0), remainingTime(0), taskType(0), priority(0), initialWorker(0) {}

    Task(int id, int time, int type, int priority, int worker)
    : taskID(id), remainingTime(time), taskType(type), priority(priority), initialWorker(worker) {}
};

inline std::ostream& operator<<(std::ostream& out, const Task& task) {
    out << "[Task " << task.taskID
        << ", Time: " << task.remainingTime
        << ", Priority: " << task.priority
        << ", Type: " << task.taskType
        << ", Worker: " << task.initialWorker << "]";
    return out;
}

inline int Estimate_TaskCycles(const Task& task, int workerSpecialty) {
    if (task.taskType == workerSpecialty) {
        return (task.remainingTime + 1) / 2;
    }

    return task.remainingTime;
}

#endif
