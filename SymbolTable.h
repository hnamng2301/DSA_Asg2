#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

// Hello. This is Assignment 2 of DSA in Autumn 2021 : Using Splay Tree to simulate SymbolTable

class BinaryNode
{
    BinaryNode* left;
    BinaryNode* right;
    BinaryNode* parent;
    string idNode;
    string typeNode;
    int levelNode;

public:
    BinaryNode(string ID, string type, string condition, int level)
    {
        left = nullptr;
        right = nullptr;
        parent = nullptr;
        idNode = ID;
        typeNode = type;
        if (condition == "true") levelNode = 0;
        else levelNode = level;
    }
    friend class SymbolTable;
};

class SymbolTable
{
    BinaryNode* root;
public:
    SymbolTable();  // Constructor
    ~SymbolTable(); // Destructor
    void leftRotate(BinaryNode*);
    void rightRotate(BinaryNode*);
    void splaying(BinaryNode*, string, int&, int&, int&);
    bool isBigger(BinaryNode*, BinaryNode*);                                // Compare 2 nodes <a> and <b>, return true if a > b
    void isNotDuplicate(BinaryNode*, string, int, bool&);                          // Check a <Name> if it is declared
    void insert(BinaryNode*, string, string, string, int&, int&, int&);     // Insert task
    void inTree(BinaryNode*, string, BinaryNode*&);                         // Support for assign, return a node has name to Assign
    BinaryNode* search(string, string, int&, int&, int&);                   // Search node which found in inTree, show numcomp and numsplay of Assign
    void assign(BinaryNode*, string, string, string, int&, int&, int&);     // Assign task
    BinaryNode* searchScope(string, string, int&, int&, int&);              // Find all node in this scope
    BinaryNode* subtreeMax(BinaryNode*);                                    // Support to remove
    void split(BinaryNode*&, BinaryNode*&, BinaryNode *&, string, int&, int&, int&);    // Support to remove
    BinaryNode* join(BinaryNode*, BinaryNode*, string, int&, int&, int&);               // Support to remove
    void getOut(string, string, int&, int&, int&);      // End task: Remove all node in this scope
    void lookUp(string, string, int&, int&, int&);      // Look up task
    void inOrder(BinaryNode*);                          // Test 
    void preOrder(BinaryNode*, string&);                // Support to print in-order
    void printPre(string& res);                         // Print task
    void clear(BinaryNode*);                            // Support Destructor
    void run(string filename);                          // Main function
};

void checkLine(string action);                      // Check first code is INSERT, ASSIGN, ...
void splitString(string line, string arg[]);        // Split line / tokenize string
void checkLineValid(string, string, string arg[]);  // Check line is valid
bool validID(string lineName);                      // Check variable's is valid
bool isNum(string lineValue);                       // Check value to assign is valid number
bool isString(string lineValue);                    // Check value to assign is valid string
bool checkType(string type);                        // Check type is valid

// This is end of Assignment. Good luck.
#endif