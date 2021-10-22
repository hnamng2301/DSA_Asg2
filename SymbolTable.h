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
    static int scope;
    BinaryNode* root;
public:
    SymbolTable();  // Constructor
    //~SymbolTable(); // Destructor
    void zig(BinaryNode*);
    void zig_zig(BinaryNode*);
    void zig_zag(BinaryNode*);
    void splaying(BinaryNode*, string, int&, int&, int&);
    BinaryNode* findNode(BinaryNode*, int);
    bool isBigger(BinaryNode*, BinaryNode*);
    BinaryNode* remove(BinaryNode*, int);
    void inOrder(BinaryNode*);
    bool isNotDuplicate(BinaryNode*, string, int);
    void insert(BinaryNode*, string, string, string, int&, int&, int&);
    BinaryNode* search(string, string, int&, int&, int&);
    void assign(BinaryNode*, string, string, string, int&, int&, int&);
    BinaryNode* searchScope(int& scope);
    BinaryNode* subtreeMax(BinaryNode*);
    BinaryNode* subtreeMin(BinaryNode*);
    void getOut(string, int&);
    void run(string filename);  // Main function
};

void checkLine(string action);                      // Check first code is INSERT, ASSIGN, ...
void splitString(string line, string arg[]);        // Split line / tokenize string
void checkLineValid(string, string, string arg[]);  // Check line is valid
bool validID(string lineName);                      // Check variable's is valid
bool isNum(string lineValue);                       // Check value to assign is valid number
bool isString(string lineValue);                    // Check value to assign is valid string
bool checkType(string type);                        // Check type is valid
int SymbolTable::scope = 0;

// This is end of Assignment. Good luck.
#endif