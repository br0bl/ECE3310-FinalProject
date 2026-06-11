cmake_minimum_required(VERSION 4.1)
project(SchedulerProject)

set(CMAKE_CXX_STANDARD 20)

add_executable(SchedulerProject main.cpp
        Deque.cpp
        Deque.h
        Task.h
        Worker.h
        Cycle.h
        Cycle.cpp
        Worker.cpp
        TaskHelper.cpp
        TaskHelper.h
        Simulation.cpp
        Simulation.h
        Tester.cpp
        Tester.h
)
