#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

struct Customer {
public:
    string name;
    int serviceTime;
};

class Node {
public:
    int data;
    Node* next;

    Node(int value) {
        data = value;
        next = nullptr;
    }
};

class LinkedList {
private:
    Node* head;

public:
    LinkedList() {
        head = nullptr;
    }

    ~LinkedList() {
        Node* current = head;
        while (current != nullptr) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    // Iterate to a given position (0-based index)
    Node* iterate(int position) {
        if (position < 0) return nullptr;

        Node* current = head;
        int index = 0;

        while (current != nullptr && index < position) {
            current = current->next;
            index++;
        }
        return current; // nullptr if not found
    }

    // Insert at position
    void insertAt(int position, int value) {
        Node* newNode = new Node(value);

        if (position <= 0 || head == nullptr) { // Insert at beginning
            newNode->next = head;
            head = newNode;
            return;
        }

        Node* prev = iterate(position - 1);
        if (prev == nullptr) { // Position too big, append at end
            append(value);
            delete newNode; // already appended, avoid memory leak
            return;
        }

        newNode->next = prev->next;
        prev->next = newNode;
    }

    // Prepend (beginning)
    void prepend(int value) {
        insertAt(0, value);
    }

    // Append (end)
    void append(int value) {
        if (head == nullptr) {
            head = new Node(value);
            return;
        }

        Node* last = iterate(size() - 1);
        last->next = new Node(value);
    }

    // Find node by value
    Node* find(int value) {
        Node* temp = head;
        while (temp != nullptr) {
            if (temp->data == value)
                return temp;
            temp = temp->next;
        }
        return nullptr;
    }

    // Contains (true/false)
    bool contains(int value) {
        return find(value) != nullptr;
    }

    // Remove by value
    bool remove(int value) {
        if (head == nullptr) return false;

        // Special case: head
        if (head->data == value) {
            Node* temp = head;
            head = head->next;
            delete temp;
            return true;
        }

        // Find previous node of the target
        int index = 0;
        Node* current = head;
        while (current != nullptr && current->next != nullptr) {
            if (current->next->data == value) {
                Node* target = current->next;
                current->next = target->next;
                delete target;
                return true;
            }
            current = current->next;
            index++;
        }

        return false; // Not found
    }

    // Get size
    int size() {
        int count = 0;
        Node* temp = head;
        while (temp != nullptr) {
            count++;
            temp = temp->next;
        }
        return count;
    }

    // Display
    void display() {
        Node* temp = head;
        while (temp != nullptr) {
            cout << temp->data << " -> ";
            temp = temp->next;
        }
        cout << "NULL" << endl;
    }
};


class Queue {
private:
    LinkedList list;  // LL queue
    // int length = 0;
    //rear && end not needed bec we already have append() in LL implementation  
    //If I use queue's own head, it wouldn't use the LL at all.

public:
    bool isEmpty() {
        return list.size() == 0;
    }
    void enqueue(const Customer& value) { // could pass by reference to save memory
        list.append(value.serviceTime);
    }

    int dequeue() { //bool  return Customer object 
        if (this->isEmpty()) {
            cout << "Queue Empty" << endl;
            exit(1);
        }
        
        int value = list.iterate(0)->data;
        list.remove(value);
        return value;
    }
    void display();
    int peek();  //not redundant because sometimes user doesn't want to dequeue
};

class Stack {
private:
    LinkedList list;

public:
    bool isEmpty() {
        return list.size() == 0;
    }
    void push(Customer value) {
        list.prepend(value.serviceTime);
    }

    int pop() {
        if (this->isEmpty()){
            cout << "Stack Empty" << endl;
            exit(1);
        }
        int topValue = list.iterate(0)->data;
        list.remove(topValue);   //remove(0) removes node w/data 0 not index 0

        return topValue;
    }
};


int main() {
    Queue waitingQueue;
    Stack missedStack;

    ifstream inputFile("sample.txt");
    if (!inputFile) {
        cout << "Could not open sample.txt" << endl;
        return -1;
    }
    string name, state;  
    int time;           

    cout << "Reading Incoming Calls" << endl;
    while (inputFile >> name >> state >> time) {
        Customer cust = {name, time}; //create customer for each line

        if (state == "waiting") {
            waitingQueue.enqueue(cust);
            cout << "  " << name << " is waiting." << endl;
        } else if (state == "missed") {
            missedStack.push(cust);
            cout << "  " << name << " missed their call." << endl;
        }
    }
    inputFile.close();

    cout << setw(50) << setfill('-') << "" << endl;


    int currentTime = 0;
    cout << "--- Call Center Simulation Starting ---" << endl;

    while (!waitingQueue.isEmpty() || !missedStack.isEmpty()) {
        // Handle up to 3 waiting calls
        for (int i = 0; i < 3 && !waitingQueue.isEmpty(); i++) {
            int serviceTime = waitingQueue.dequeue();
            cout << "[Time: " << currentTime << "] Answering call (service time: "
                << serviceTime << " mins)." << endl;
            currentTime += serviceTime;
            cout << "[Time: " << currentTime << "] Finished call." << endl;
        }

        if (!missedStack.isEmpty()) {
            int serviceTime = missedStack.pop();
            cout << "[Time: " << currentTime << "] Calling back missed call (service time: "
                << serviceTime << " mins)." << endl;
            currentTime += serviceTime;
            cout << "[Time: " << currentTime << "] Finished callback." << endl;
        }


        cout << setw(50) << setfill('-') << "" << endl;

    }
    cout << "All calls have been handled. Total time: "
         << currentTime << " minutes." << endl;

    return 0;
}
