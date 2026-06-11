#ifndef SCHEDULERPROJECT_DEQUE_H
#define SCHEDULERPROJECT_DEQUE_H

#include <iostream>
#include "Task.h"

typedef Task DequeElementType;

class Deque {
private:
    struct Node {
        DequeElementType data;
        Node* next;
        Node* prev;
        Node(const DequeElementType& value) : data(value), next(nullptr), prev(nullptr) {}
    };

    Node* myFront;  // Pointer to the front node
    Node* myBack;   // Pointer to the rear node
    size_t size;       // Current size of the deque

    // Helper function to deep-copy nodes from another deque
    void copy_nodes(const Deque& other);

    // Helper function to delete all nodes
    void clear();

public:
    // Constructors and Destructor
    Deque();
    Deque(const Deque& other); // Copy constructor
    ~Deque();

    // Assignment operator
    Deque& operator=(const Deque& other);

    // Check if the deque is empty
    bool empty() const;

    // Return the number of elements in the deque
    size_t get_size() const;

    // Add an element at the front of the deque
    void push_front(const DequeElementType& value);

    // Add an element at the rear of the deque
    void push_back(const DequeElementType& value);

    // Remove an element from the front of the deque
    bool pop_front(DequeElementType& value);

    // Remove an element from the rear of the deque
    bool steal_back(DequeElementType& value);

    // Get the front element
    bool peek_front(DequeElementType& value) const;

    // Get the rear element
    bool peek_back(DequeElementType& value) const;

    // Display all elements in the deque
    void display() const;

    bool hasPriority(int priority) const;
    bool peekLastPriorityWithWork(DequeElementType& value, int priority, int& cyclesUntilTask, int workerSpecialty) const;
    bool removeLastPriority(DequeElementType& value, int priority);
};

#endif //SCHEDULERPROJECT_DEQUE_H
