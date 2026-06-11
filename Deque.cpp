#include "Deque.h"

// Default constructor
Deque::Deque() : myFront(nullptr), myBack(nullptr), size(0) {}

// Copy constructor
Deque::Deque(const Deque& other) : myFront(nullptr), myBack(nullptr), size(0) {
    copy_nodes(other);
}

// Destructor
Deque::~Deque() {
    clear();
}

// Assignment operator
Deque& Deque::operator=(const Deque& other) {
    if (this != &other) {
        clear();
        copy_nodes(other);
    }
    return *this;
}

// Helper function to deep-copy nodes from another deque
void Deque::copy_nodes(const Deque& other) {
    Node* current = other.myFront;
    while (current != nullptr) {
        push_back(current->data);
        current = current->next;
    }
}

// Helper function to delete all nodes
void Deque::clear() {
    DequeElementType temp;
    while (pop_front(temp)) {
    }
}

// Check if the deque is empty
bool Deque::empty() const {
    return size == 0;
}

// Return the number of elements in the deque
size_t Deque::get_size() const {
    return size;
}

// Add an element at the front of the deque
void Deque::push_front(const DequeElementType& value) {
    Node* newNode = new Node(value);
    if (empty()) {
        myFront = myBack = newNode;
    }
    else {
        newNode->next = myFront;
        myFront->prev = newNode;
        myFront = newNode;
    }
    size++;
}

// Add an element at the rear of the deque
void Deque::push_back(const DequeElementType& value) {
    Node* newNode = new Node(value);
    if (empty()) {
        myFront = myBack = newNode;
    }
    else {
        myBack->next = newNode;
        newNode->prev = myBack;
        myBack = newNode;
    }
    size++;
}

// Remove an element from the front of the deque
bool Deque::pop_front(DequeElementType& value) {
    if (empty()) {
        return false;
    }
    Node* temp = myFront;
    value = temp->data;
    myFront = myFront->next;

    // Check if Deque is empty
    if (myFront != nullptr) {
        myFront->prev = nullptr;
    }
    else {
        myBack = nullptr;
    }

    delete temp;
    size--;
    return true;
}

// Remove an element from the rear of the deque
bool Deque::steal_back(DequeElementType& value) {
    if (empty()) {
        return false;
    }

    value = myBack->data;

    if (myFront == myBack) {  // If there's only one element
        delete myBack;
        myFront = myBack = nullptr;
    }
    else {
        Node* temp = myBack;
        myBack = myBack->prev;
        myBack->next = nullptr;
        delete temp;
    }
    size--;
    return true;
}

// Get the front element
bool Deque::peek_front(DequeElementType& value) const {
    if (empty()) {
        return false;
    }
    value = myFront->data;
    return true;
}

// Get the rear element
bool Deque::peek_back(DequeElementType& value) const {
    if (empty()) {
        return false;
    }
    value = myBack->data;
    return true;
}

// Display all elements in the deque
void Deque::display() const {
    Node* current = myFront;
    std::cout << "Deque elements: ";
    while (current != nullptr) {
        std::cout << current->data << " ";
        current = current->next;
    }
    std::cout << std::endl;
}

bool Deque::hasPriority(int priority) const {
    Node* current = myFront;

    while (current != nullptr) {
        if (current->data.priority == priority) {
            return true;
        }

        current = current->next;
    }

    return false;
}

bool Deque::peekLastPriorityWithWork(DequeElementType& value, int priority, int& cyclesUntilTask, int workerSpecialty) const {
    Node* current = myFront;

    bool found = false;
    int runningCycles = 0;
    int savedCycles = 0;

    while (current != nullptr) {
        runningCycles += Estimate_TaskCycles(current->data, workerSpecialty);

        if (current->data.priority == priority) {
            value = current->data;
            savedCycles = runningCycles;
            found = true;
        }

        current = current->next;
    }

    if (found) {
        cyclesUntilTask = savedCycles;
        return true;
    }

    return false;
}

bool Deque::removeLastPriority(DequeElementType& value, int priority) {
    Node* current = myBack;

    while (current != nullptr) {
        if (current->data.priority == priority) {
            value = current->data;

            if (current == myFront && current == myBack) {
                myFront = nullptr;
                myBack = nullptr;
            }
            else if (current == myFront) {
                myFront = current->next;
                myFront->prev = nullptr;
            }
            else if (current == myBack) {
                myBack = current->prev;
                myBack->next = nullptr;
            }
            else {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }

            delete current;
            size--;

            return true;
        }

        current = current->prev;
    }

    return false;
}
