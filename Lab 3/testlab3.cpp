#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

class Customer {
public:
    string name;
    int serviceTime;
};

//Node to store a customer instead of basic int.
class Node {
public:
    Customer data;
    Node* next;

    Node(Customer value) {
        data = value;
        next = nullptr;
    }
};

class LinkedList { //barebones SLL no tail or Length
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

    Node* iterate(int position) {
        if (position < 0) return nullptr;

        Node* current = head;
        int index = 0;

        while (current != nullptr && index < position) {
            current = current->next;
            index++;
        }
        return current;
    }

    void prepend(Customer value) {
        Node* newNode = new Node(value);
        newNode->next = head;
        head = newNode;
    }

    void append(Customer value) {
        if (head == nullptr) {
            head = new Node(value);
            return;
        }

        Node* last = head;
        while (last->next != nullptr) {
            last = last->next;
        }
        last->next = new Node(value);
    }

    bool isEmpty() {
        return head == nullptr;
    }

    int size() {
        int count = 0;
        Node* temp = head;
        while (temp != nullptr) {
            count++;
            temp = temp->next;
        }
        return count;
    }

    Customer getAt(int position) {
        Node* node = iterate(position);
        if (node == nullptr) {
            throw "Invalid position in getAt()";
        }
        return node->data;
    }

    Customer removeFirst() {
        if (head == nullptr)
            throw "List is empty";
        Node* temp = head;
        Customer val = head->data;
        head = head->next;
        delete temp;
        return val;
    }

    void display() {
        Node* temp = head;
        while (temp != nullptr) {
            cout << "[" << temp->data.name << ", " << temp->data.serviceTime << "min] -> ";
            temp = temp->next;
        }
        cout << "NULL" << endl;
    }
};

class Queue {
private:
    LinkedList list;

public:
    void enqueue(Customer value) {
        list.append(value);
    }

    Customer dequeue() {
        if (list.isEmpty())
            throw "Queue is empty";
        return list.removeFirst();
    }

    bool isEmpty() {
        return list.isEmpty();
    }

};

class Stack {
private:
    LinkedList list;

public:
    void push(Customer value) {
        list.prepend(value);
    }

    Customer pop() {
        if (list.isEmpty())
            throw "Stack is empty";
        return list.removeFirst();
    }

    bool isEmpty() {
        return list.isEmpty();
    }
};

int main() {
    Queue waitingQueue;
    Stack missedStack;

    ifstream inputFile("sample.txt");
    if (!inputFile) {
        cout << "Error: Could not open sample.txt" << endl;
        return -1;
    }
    string name, state;  
    int time;           

    cout << "--- Reading Incoming Calls ---" << endl;
    while (inputFile >> name >> state >> time) {
        Customer cust = {name, time};

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
            Customer cust = waitingQueue.dequeue();
            cout << "[Time: " << currentTime << "] Answering waiting call from "
                 << cust.name << " (service time: " << cust.serviceTime << " mins)." << endl;
            currentTime += cust.serviceTime;
            cout << "[Time: " << currentTime << "] Finished call with " << cust.name << "." << endl;
        }

        // Then handle 1 missed call
        if (!missedStack.isEmpty()) {
            Customer cust = missedStack.pop();
            cout << "[Time: " << currentTime << "] Calling back most recent missed call for "
                 << cust.name << " (service time: " << cust.serviceTime << " mins)." << endl;
            currentTime += cust.serviceTime;
            cout << "[Time: " << currentTime << "] Finished callback with " << cust.name << "." << endl;
        }

        cout << setw(50) << setfill('-') << "" << endl;

    }

    cout << "All calls have been handled. Total simulation time: "
         << currentTime << " minutes." << endl;

    return 0;
}
