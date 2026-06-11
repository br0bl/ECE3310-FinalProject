#ifndef SCHEDULERPROJECT_CYCLE_H
#define SCHEDULERPROJECT_CYCLE_H

class Cycle {
private:
    int currentCycle;
    int maxCycles;
    int maxPriorityFinishedCycle;

public:
    Cycle(int max = 0);

    int getCurrentCycle() const;
    int getMaxCycles() const;

    bool done() const;
    void advance();
    void reset();

    void setMaxPriorityFinishedCycle(int cycle);
    int getMaxPriorityFinishedCycle() const;
    bool maxPriorityFinishedCycleRecorded() const;
};

#endif
