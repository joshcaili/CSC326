#ifndef BSTCHECKER_H
#define BSTCHECKER_H

#include "BSTNode.h"
#include <unordered_set>
#include <limits>


using namespace std;
class BSTChecker {
public:
   // Helper to check validity with min/max constraints
   static BSTNode* Validate(BSTNode* node, long minVal, long maxVal, unordered_set<BSTNode*>& visited) { //without static, compiler error. //long is necessary as well
      if (!node) return nullptr;   // Case 1 Empty Tree (or subtree when doing recursive call) which is still valid

      // Case 2 Node contains a duplicate valid in set so this invalidates
      if (visited.count(node)) {  // Couldn't get visited.contains() method working. count returns 1 or 0 
         return node; // structure is invalid return invalid node
      }
      //Case 3 Otherwise track the Node so we can Check for duplicate Node->values (Necessary to check for cycles in structure) 
      // If it was just BSTinsert() or contains() you wouldn't need tracking prev nodes.
      visited.insert(node);
// how do you keep track of local min and max
      // Case3a: if current Node's Value is NOT between minVal and maxVal. Not BST
      if (!(node->key > minVal && node->key < maxVal)) {
         return node; // Problem Node found
      }
      
      // Case3b Else, if value is between bounds, continue checking the rest of nodes
      // validate left
      BSTNode* badLeft = Validate(node->left, minVal, node->key, visited);
      if (badLeft) return badLeft; // If not nullptr, that means it's an error node, return it.

      // Same but for the right direction
      BSTNode* badRight = Validate(node->right, node->key, maxVal, visited);
      if (badRight) return badRight; // If not nullptr, return the node

      return nullptr; // no error
   }

   static BSTNode* CheckBSTValidity(BSTNode* rootNode) { // without stack, compiler error
      unordered_set<BSTNode*> visited;

      return Validate(
         rootNode,
	 //set the initial bounds to be +\infty & -\infty. because
	 //root can be any value
         std::numeric_limits<long>::min(),
         std::numeric_limits<long>::max(),
         visited
      );
   }

};

#endif



































