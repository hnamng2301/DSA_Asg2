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
    friend class SplayTree;
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
    bool isBigger(BinaryNode*, BinaryNode*);
    bool isNotDuplicate(BinaryNode*, string, int);
    void insert(BinaryNode*, string, string, string, int&, int&, int&);
    void inTree(BinaryNode*, string, BinaryNode*&);
    BinaryNode* search(string, string, int&, int&, int&);
    void assign(BinaryNode*, string, string, string, int&, int&, int&);
    BinaryNode* searchScope(string, string, int&, int&, int&);
    BinaryNode* subtreeMax(BinaryNode*);
    void split(BinaryNode*&, BinaryNode*&, BinaryNode *&, string, int&, int&, int&);
    BinaryNode* join(BinaryNode*, BinaryNode*, string, int&, int&, int&);
    void getOut(string, string, int&, int&, int&);
    void lookUp(string, string, int&, int&, int&);
    void inOrder(BinaryNode*);
    void preOrder(BinaryNode*, string&);
    void printPre(string& res);
    void clear(BinaryNode*);
    void run(string filename);  // Main function
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