#include "Cycle.h"

Cycle::Cycle(int max)
    : currentCycle(0), maxCycles(max), maxPriorityFinishedCycle(-1) {
}

int Cycle::getCurrentCycle() const {
    return currentCycle;
}

int Cycle::getMaxCycles() const {
    return maxCycles;
}

bool Cycle::done() const {
    return currentCycle >= maxCycles;
}

void Cycle::advance() {
    if (!done()) {
        currentCycle++;
    }
}

void Cycle::reset() {
    currentCycle = 0;
    maxPriorityFinishedCycle = -1;
}

void Cycle::setMaxPriorityFinishedCycle(int cycle) {
    maxPriorityFinishedCycle = cycle;
}

int Cycle::getMaxPriorityFinishedCycle() const {
    return maxPriorityFinishedCycle;
}

bool Cycle::maxPriorityFinishedCycleRecorded() const {
    return maxPriorityFinishedCycle != -1;
}
