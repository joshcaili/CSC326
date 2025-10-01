#include <iostream>
#include <vector>
#include <string>
#include <termios.h>
#include <unistd.h>
using namespace std;

// (6) Create a class Bag
class Bag {
public:  // bag variables
    string collectionName;      // (6a)
    vector<string> items;      // (6b) array of items
    int itemCount;             // (6c) member variable to keep track of items

    string input;  //used in search function
    char ch;       //used in search function

public: //bag constructor 
    Bag(string name, vector<string> list) { 
        collectionName = name;
        items = list;
        itemCount = list.size(); // flexible way to count items in array
    }

public: //bag member functions
    // (1) Method displayAll(): shows all items 

    //(6D)
    void displayAll(string input) const {
        cout << "-------------" << collectionName << endl;

        // (2) search window if less than 3 chars entered
        cout << "Search: " << input << endl;
        if (input.size() < 3) {
            cout << "Showing all items:" << endl;
            for (int i = 0; i < items.size(); i++)
                cout << " - " << items[i] << endl;
        } else {
            // (3) Display smaller list if substring matches
            cout << "Suggestions:" << endl;
            for (int i = 0; i < items.size(); i++) {
                if (items[i].find(input) == 0) { // starts with input
                    cout << " - " << items[i] << endl;
                }
            }
        }
    }
    void search(){//// Reading one character at a time     // (4) & (6E) Autocomplete suggestion list
/////// // Setup terminal for updating without pressing <cr> (macOS)///////////////////////
        termios oldt, newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO); 
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);


////////////// (4a) if user erases or adds new one////////////////////////
        while (true) {
            cout << "[2J[1;1H"; // clear screen
            if (read(STDIN_FILENO, &ch, 1) == 1) {
                if (ch == 10)   // Enter 
                    break;      
                else if (ch == 127) {      // Backspace  (4a) if user erases
                    if (!input.empty()) input.pop_back();
                } 
                else if (isprint(ch)) {  // If alpha char (4b) add new chars via pushback  
                    input.push_back(ch);
                }   
                // [[nya]] could add an else in case every check fails
                displayAll(input); // refresh suggestions
            }
        }
   
 // restore normal mode | put terminal back to normal state
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
};

int main() {

///// initializing vector outside of class. [[nya]] I could put it inside the bag class to initalize
    vector<string> fruits = {
        "apple", "apricot", "banana", "blueberry", "blackberry",
    };

    Bag bag("Fruit Collection", fruits);   // object created

    ////////////////////////////// declare query & each char then display contents of bag upon running program

    bag.displayAll("");
    bag.search();

/// todo optional. Clear terminal so it's easier on the eyes [[nya]]





    return 0;
}
