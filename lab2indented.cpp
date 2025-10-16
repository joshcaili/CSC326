#include <cstdlib>  
#include <ctime>    // For time()
#include <iostream>
#include <string>

using namespace std;

// Part 1: Define the Commit Node
struct Commit {  // all given
  int hash;
  string message;
  Commit* next;

  // Constructor for a new commit
  Commit(int h, const string& msg) : hash(h), message(msg), next(nullptr) {}
};

class CommitHistory {  // HACK: //Define CommitHistory | LL class
 private:
  Commit* head;  // given
  Commit* tail;  // added tail for easier access
  int length;    // added length since github also keeps track of

 public:
  // Constructor
  CommitHistory() : head(nullptr) {}  // given,  you don't start
  // with one node because when creating git repo you start with
  // nothing not 1 node

  // Destructor (Implementation in Part 3) TODO:
  ~CommitHistory();

 public:                               // Class methods
  void commit(const string& message);  // Append a new commit
  void log() const;                    // Display all commits
  void reset();                        // Delete the last commit
  void gitShow(int hash) const;  // search & print a specific commit by hash

  static CommitHistory merge(
      const CommitHistory& branch1,
      const CommitHistory& branch2);  // merge to the end, assuming no conflicts
};
// Part 3: Implement Functions

void CommitHistory::commit(const string& message) {  // given
  // int hash = rand() % 10000;  // Simulated hash     
  int hash = 1;  //used to test gitShow() in main

  Commit* newCommit = new Commit(hash, message);  // HACK:  // create new  //
  // & pass hash & user's description/message

  // If the list is empty, the new commit is the head
  if (head == nullptr) {
    head = newCommit;
    tail = newCommit;
    length = 1;
  } else {
    tail->next = newCommit;
    tail = newCommit;
    length++;
  }  
}

void CommitHistory::log() const {  // TODO:

  cout << "\nInit File" << endl;  // could add author, date etc
  if (head == nullptr) {
    cout << "No commits yet." << endl;
    return;
  }
  Commit* temp = head;
  while (temp) {
    cout << temp->message << temp->hash << endl;  // log() without
    // onelineflag
    temp = temp->next;
  }
}

void CommitHistory::reset() {  // delete last node | LL delete last
  // TODO:
  //  If the list is empty, there's nothing to do
  if (head == nullptr) {
    cout << "No commits exist" << endl;
    return;
  }

  Commit* temp = head;
  // If there is only one commit in the history
  if (length == 1) {
    head = nullptr;
    tail = nullptr;
    cout << "\nLast commit removed (reset)." << endl;
    return;  // delete will happen at end of all checks
  }

  // Traverse entire LL to find the second-to-last node
  else {
    Commit* prior = head;
    while (temp->next) {
      prior = temp;
      temp = temp->next;
    }
  }

  // Delete the last node and update the new last node's 'next' pointer
  delete temp->next;
  length--;
  cout << "\nLast commit removed (reset)." << endl;
}

CommitHistory CommitHistory::merge(const CommitHistory& branch1,
                                   const CommitHistory& branch2) {  // TODO:
  CommitHistory merged;  // initialize a new LL
  Commit* temp = branch1.head;
  while (temp) {
    merged.commit(temp->message);
    temp = temp->next;
  }
  temp = branch2.head;  // change branch.
  while (temp) {
    merged.commit(temp->message);
    temp = temp->next;
  }
  cout << "\nBranches merged." << endl;
  return merged;
}

/**
 * Destructor – Releases all memory by deleting every node in the list.
 */
CommitHistory::~CommitHistory() {  // TODO:
  Commit* current = head;
  while (head) {
    head = head->next;
    delete current;                    // Delete the current one
    current = head;                // Move to the next one
  }
  head = nullptr;  // Set head to null after clearing
}

void CommitHistory::gitShow(int hash) const {  // Task 2 TODO:
  Commit* current = head;
  while (current) {
    if (current->hash == hash) {
      cout << "\nFound commit with hash " << hash << ":" << endl;
      cout << "Message: " << current->message << endl;
      return;
    }
  }
  cout << "\nCommit with hash " << hash << " not found." << endl;
}

// Part 4: Main Function
int main() {
  // Seed the random number generator
  srand(time(0));

  CommitHistory master;
  master.commit("Initial commit");
  master.commit("Add README");
  master.commit("Implement login system");
  // Create feature branch and add commits
  CommitHistory featureBranch;
  featureBranch.commit("Start feature X");
  featureBranch.commit("Fix bug in feature X");

  // Log initial states
  cout << "\n\n== Master Branch ==\n";
  master.log();

  cout << "\n\n== Feature Branch ==\n";
  featureBranch.log();

  //Test gitShow //                                                   HACK:
  cout << "\n\n== Testing gitShow on master ==";
  // master.gitShow(1); // has a bug because all commits have same
  // hash if I use hash 1. & runs infinitely
  // rand() also is a bug because it could still randomly recieve
  // the same hash.
  // master.gitShow(2); // Test non-existing hash 

  // Test reset
  cout << "\n== Reset last commit on master ==";
  master.reset();
  master.log();


   cout << "\n== Merged History ==";
   CommitHistory merged = CommitHistory::merge(master, featureBranch);
   merged.reset();
   merged.log();
  // Verify original branches are unchanged
  cout << "\n== Master Branch Unchanged after merge ==";
  master.log();

  cout << "\n== Feature Branch Unchanged after merge ==";
  featureBranch.log();

  cout << endl;

  return 0;
}
