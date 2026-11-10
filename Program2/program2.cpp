/*
Name: Aidan Lam
Description: Processes named stacks and queues using templated, singly linked lists and writes the results to an output file.
*/

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <list>

// Abstract singly linked list shared by stacks and queues.
template <typename T>
class SimpleList {
public:
    explicit SimpleList(const std::string& name);
    virtual ~SimpleList();

    SimpleList(const SimpleList&) = delete;
    SimpleList& operator=(const SimpleList&) = delete;

    const std::string& getName() const;
    bool empty() const;
    virtual void push(const T& value) = 0;
    virtual T pop() = 0;

protected:
    void insertFront(const T& value);
    void insertBack(const T& value);
    T removeFront();

private:
    struct Node {
        T data;
        Node* next;
    };

    Node* head_;
    Node* tail_;
    std::string name_;
};

// =============== SimpleList member functions ===============

template <typename T>
SimpleList<T>::SimpleList(const std::string& name)
    : head_(nullptr), tail_(nullptr), name_(name) {}

// Free all remaining nodes in the list.
template <typename T>
SimpleList<T>::~SimpleList() {
    while (head_ != nullptr) {
        Node* oldHead = head_;
        head_ = head_->next;
        delete oldHead;
    }
}

template <typename T>
const std::string& SimpleList<T>::getName() const {
    return name_;
}

template <typename T>
bool SimpleList<T>::empty() const {
    return head_ == nullptr;
}

// Insert a node at the front, updating the tail if the list was empty.
template <typename T>
void SimpleList<T>::insertFront(const T& value) {
    head_ = new Node{value, head_};
    if (tail_ == nullptr) {
        tail_ = head_;
    }
}

// Append a node, updating the head if the list was empty.
template <typename T>
void SimpleList<T>::insertBack(const T& value) {
    Node* node = new Node{value, nullptr};
    if (tail_ == nullptr) {
        head_ = node;
    }
    else {
        tail_->next = node;
    }
    tail_ = node;
}

// Remove the first node and return its value.
template <typename T>
T SimpleList<T>::removeFront() {
    Node* oldHead = head_;
    T value = oldHead->data;
    head_ = oldHead->next;
    if (head_ == nullptr) {
        tail_ = nullptr;
    }
    delete oldHead;
    return value;
}

// LIFO list: insert and remove from the front.
template <typename T>
class Stack : public SimpleList<T> {
public:
    explicit Stack(const std::string& name);
    void push(const T& value) override;
    T pop() override;
};

// =============== Stack member functions ===============

template <typename T>
Stack<T>::Stack(const std::string& name) : SimpleList<T>(name) {}

template <typename T>
void Stack<T>::push(const T& value) {
    this->insertFront(value);
}

template <typename T>
T Stack<T>::pop() {
    return this->removeFront();
}

// FIFO list: insert at the back and remove from the front.
template <typename T>
class Queue : public SimpleList<T> {
public:
    explicit Queue(const std::string& name);
    void push(const T& value) override;
    T pop() override;
};

// =============== Queue member functions ===============

template <typename T>
Queue<T>::Queue(const std::string& name) : SimpleList<T>(name) {}

template <typename T>
void Queue<T>::push(const T& value) {
    this->insertBack(value);
}

template <typename T>
T Queue<T>::pop() {
    return this->removeFront();
}

// =============== List lookup, command processing, and cleanup ===============

// Find a named stack or queue in the collection for its data type.
template <typename T>
SimpleList<T>* findList(const std::list<SimpleList<T>*>& lists, const std::string& name) {
    for (SimpleList<T>* list : lists) {
        if (list->getName() == name) {
            return list;
        }
    }
    return nullptr;
}

// Execute a command using the collection for the name's data type.
template <typename T>
void processCommand(const std::string& action, const std::string& name,
                    const std::string& argument,
                    std::list<SimpleList<T>*>& lists, std::ofstream& output) {
    if (action == "create") {
        if (findList(lists, name) != nullptr) {
            output << "ERROR: This name already exists!\n";
        }
        else if (argument == "stack") {
            lists.push_front(new Stack<T>(name));
        }
        else {
            lists.push_front(new Queue<T>(name));
        }

        return;
    }

    SimpleList<T>* list = findList(lists, name);
    if (list == nullptr) {
        output << "ERROR: This name does not exist!\n";
    }
    else if (action == "push") {
        T value;
        std::istringstream valueStream(argument);
        valueStream >> value;
        list->push(value);
    }
    else if (list->empty()) {
        output << "ERROR: This list is empty!\n";
    }
    else {
        output << "Value popped: " << list->pop() << '\n';
    }
}

// Release all dynamically allocated stacks and queues.
template <typename T>
void clearLists(std::list<SimpleList<T>*>& lists) {
    for (SimpleList<T>* list : lists) {
        delete list;
    }
    lists.clear();
}

// =============== Main program ===============

// Process input commands and write results to the output file.
int main() {
    std::string inputFilename, outputFilename;

    std::cout << "Enter the name of the input file: ";
    std::getline(std::cin, inputFilename);

    std::cout << "Enter the name of the output file: ";
    std::getline(std::cin, outputFilename);

    std::ifstream input(inputFilename);

    if (!input) {
        std::cerr << "Error: Could not open input file: " << inputFilename << '\n';
        return 1;
    }

    std::ofstream output(outputFilename, std::ios::binary);

    if (!output) {
        std::cerr << "Error: Could not open output file: " << outputFilename << '\n';
        return 1;
    }

    std::list<SimpleList<int>*> integers;
    std::list<SimpleList<double>*> doubles;
    std::list<SimpleList<std::string>*> strings;

    std::string command;
    while (std::getline(input, command)) {
        output << "PROCESSING COMMAND: " << command << '\n';

        std::istringstream line(command);
        std::string action, name, argument;
        line >> action >> name;
        if (action != "pop") {
            line >> argument;
        }

        switch (name[0]) {
            case 'i':
                processCommand(action, name, argument, integers, output);
                break;
            case 'd':
                processCommand(action, name, argument, doubles, output);
                break;
            case 's':
                processCommand(action, name, argument, strings, output);
                break;
        }
    }

    clearLists(integers);
    clearLists(doubles);
    clearLists(strings);

    output.close();

    if (!output) {
        std::cerr << "Error: Could not write output file: " << outputFilename << '\n';
        return 1;
    }

    return 0;
}