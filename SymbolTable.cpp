#include "SymbolTable.h"

// Ohayo. Hajimemashou

SymbolTable::SymbolTable()
{
    this->root = nullptr;
}

bool validID(string lineName)
{
    // Kiem tra ID hop le
    if (lineName[0] >= 'a' && lineName[0] <= 'z')
    { // Phai bat dau bang ky tu thuong
        for (unsigned int i = 0; i < lineName.size(); i++)
        {
            if ((lineName[i] >= 'A' && lineName[i] <= 'Z') || (lineName[i] >= 'a' && lineName[i] <= 'z') || (lineName[i] >= '0' && lineName[i] <= '9') || lineName[i] == '_')
                continue;
            else
                return false;
        }
    }
    else
        return false;
    return true;
}
bool isNum(string lineValue)
{
    regex numConst("\\d+"); // Kiem tra const number (string chi gom cac chu so)
    if (lineValue[0] == '\'' && lineValue[lineValue.size() - 1] == '\'')
        return false;
    if (regex_match(lineValue, numConst))
        return true;
    else
        return false;
    return true;
}
bool isString(string lineValue)
{
    regex strConst("^'([a-zA-Z0-9\\s]+)'$"); // Kiem tra const string

    if (lineValue[0] == '\'' && lineValue[lineValue.size() - 1] == '\'')
    { // Kiem tra value co la string khong
        if (regex_match(lineValue, strConst))
            return true;
        else
            return false;
    }
    else
        return false;
    return true;
}
bool isFunction(string lineValue){
    string s = lineValue;
    string delimeter = "(";
    string end = ")";
    if (lineValue.find(delimeter) != string::npos && lineValue.find(end) != string::npos)
    {
        size_t pos = s.find(delimeter);
        string token1 = s.substr(0, pos);
        // cout << token1 << "here" << endl;
        string token2 = s.substr(pos, s.size() - 1);
        if (token2 == "()")
            return 1;
        // cout << token2 << "here" << endl;

        if (lineValue.find(' ') != string::npos)
            return 0;
        if (!validID(token1))
            return 0;
        string argList = token2.substr(1, token2.size() - 2);
        // cout << "Here: " << argList << endl;
        pos = 0, delimeter = ",";
        while ((pos = argList.find(delimeter)) != string::npos)
        {
            string token = argList.substr(0, pos);
            argList.erase(0, pos + delimeter.length());
            if (!isString(token) && !isNum(token))
                return 0;
        }
        if (!isString(argList) && !isNum(argList))
            return 0;
    }
    return 1;
}
bool checkType(string type){
    // If is a number or string type
    if(type == "number" || type == "string") return 1;

    // If is a function type
    string s = type;
    string delimeter = "->";
    size_t pos = s.find(delimeter);
    string token1 = s.substr(0, pos);
    if (token1 == "()") return 1;
    string token2 = s.substr(pos + 2, s.size() - 1);

    regex function("^\\(([a-zA-Z0-9\\,]+)\\)$");
    if (regex_match(token1, function) != 1) return 0; // If there is special char in type string
    string argList = token1.substr(1, token1.size() - 2);
    pos = 0, delimeter = ",";
    while((pos = argList.find(delimeter)) != string::npos){
        string token = argList.substr(0,pos);
        argList.erase(0, pos + delimeter.length());
        if (token != "number" && token != "string") 
            return 0;
    }
    if (argList != "number" && argList != "string")
        return 0;
    if (token2 != "number" && token2 != "string")
        return 0;
    return 1;
}

void checkLineValid(string line, string action, string arg[])
{
    if (action != "INSERT" && action != "ASSIGN" && action != "BEGIN"
        && action != "END" && action != "PRINT" && action != "LOOKUP")
        throw InvalidInstruction(line);

    // Check Action
    if (action == "INSERT"){
        string curr = arg[0] + ' ' + arg[1] +' ' + arg[2] + ' ' + arg[3];
        if (line != curr || !validID(arg[1])) throw InvalidInstruction(line);
        if (arg[3] != "true" && arg[3] != "false") throw InvalidInstruction(line);
        if (!checkType(arg[2])) throw InvalidInstruction(line);
    }
    else if(action == "ASSIGN"){
        string curr = arg[0] + ' ' + arg[1] + ' ' + arg[2];
        if (line != curr || !validID(arg[1])) throw InvalidInstruction(line);
        if (!isFunction(arg[2]) && !isNum(arg[2]) && !isString(arg[2]) && !validID(arg[2]))
            throw InvalidInstruction(line);
    }
    //Check Variable and Value
    
}

void splitString(string line, string arg[])
{
    regex reg(" +(?=(?:[^\']*\'[^\']*\')*[^\']*$)");
    regex_token_iterator<string::iterator> iter(line.begin(), line.end(), reg, -1);
    regex_token_iterator<string::iterator> end;
    int i = 0;
    while (iter != end)
    {
        if(i >= 4)
            throw InvalidInstruction(line);
        arg[i] = *iter++;
        i++;
    }
}

void SymbolTable::zig(BinaryNode* x){
    BinaryNode* p = x->parent;
    if(p->left == x){
        //BinaryNode* A = x->left;
        BinaryNode* B = x->right;
        //BinaryNode* C = p->right;

        x->parent = nullptr;
        x->right = p;

        p->parent = x;
        p->left = B;

        if (B!= nullptr)
            B->parent = p;
    }
    else{
        //BinaryNode* A = p->left;
        BinaryNode* B = x->left;
        //BinaryNode* C = x->right;

        x->parent = nullptr;
        x->left = p;

        p->parent = x;
        p->right = B;

        if (B != nullptr)
            B->parent = p;
    }
}
void SymbolTable::zig_zig(BinaryNode* x){
    BinaryNode *p = x->parent;
    BinaryNode *g = p->parent;
    if (p->left == x)
    {
        //BinaryNode *A = x->left;
        BinaryNode *B = x->right;
        BinaryNode *C = p->right;
        //BinaryNode *D = g->right;

        x->parent = g->parent;
        x->right = p;

        p->parent = x;
        p->left = B;
        p->right = g;

        g->parent = p;
        g->left = C;

        if (x->parent != NULL)
        {
            if (x->parent->left == g)
                x->parent->left = x;
            else
                x->parent->right = x;
        }

        if (B != NULL)
            B->parent = p;

        if (C != NULL)
            C->parent = g;
    }
    else
    {
        //BinaryNode *A = g->left;
        BinaryNode *B = p->left;
        BinaryNode *C = x->left;
        //BinaryNode *D = x->right;

        x->parent = g->parent;
        x->left = p;

        p->parent = x;
        p->left = g;
        p->right = C;

        g->parent = p;
        g->right = B;

        if (x->parent != NULL)
        {
            if (x->parent->left == g)
                x->parent->left = x;
            else
                x->parent->right = x;
        }

        if (B != NULL)
            B->parent = g;

        if (C != NULL)
            C->parent = p;
    }
}
void SymbolTable::zig_zag(BinaryNode* x){
    BinaryNode *p = x->parent;
    BinaryNode *g = p->parent;
    if (p->right == x)
    {
        //BinaryNode *A = p->left;
        BinaryNode *B = x->left;
        BinaryNode *C = x->right;
        //BinaryNode *D = g->right;

        x->parent = g->parent;
        x->left = p;
        x->right = g;

        p->parent = x;
        p->right = B;

        g->parent = x;
        g->left = C;

        if (x->parent != NULL)
        {
            if (x->parent->left == g)
                x->parent->left = x;
            else
                x->parent->right = x;
        }

        if (B != NULL)
            B->parent = p;

        if (C != NULL)
            C->parent = g;
    }
    else
    {
        //BinaryNode *A = g->left;
        BinaryNode *B = x->left;
        BinaryNode *C = x->right;
        //BinaryNode *D = p->right;

        x->parent = g->parent;
        x->left = g;
        x->right = p;

        p->parent = x;
        p->left = C;

        g->parent = x;
        g->right = B;

        if (x->parent != NULL)
        {
            if (x->parent->left == g)
                x->parent->left = x;
            else
                x->parent->right = x;
        }

        if (B != NULL)
            B->parent = g;

        if (C != NULL)
            C->parent = p;
    }
}
void SymbolTable::splaying(BinaryNode* x, string name, int& scope, int& numcomp, int& numsplay){
    while (x->parent != nullptr){
        BinaryNode* p = x->parent;
        BinaryNode* g = p->parent;

        if(g == nullptr){
            zig(x);
            numsplay++;
        }
        else if(g->left == p && p->left == x){
            zig_zig(x);
            numsplay++;
        }
        else if(g->right == p && p->right == x){
            zig_zig(x);
            numsplay++;
        }
        else{
            zig_zag(x);
            numsplay++;
        }
    }
    this->root = x;
}

bool SymbolTable::isBigger(BinaryNode* a, BinaryNode* b){
    if (a->idNode == b->idNode && a->levelNode == b->levelNode && a->typeNode == b->typeNode)
        return false;
    
    // Node a > Node b
    if (a->levelNode > b->levelNode)
        return true;
    else if (a->levelNode == b->levelNode && a->idNode.compare(b->idNode) > 0)
        return true;

    return false;
}

bool SymbolTable::isNotDuplicate(BinaryNode* root, string name, int scope){
    if (root == nullptr) return true;

    if (root->idNode == name && root->levelNode == scope) return false;

    if ((root->levelNode == scope && root->idNode.compare(name) < 0) || root->levelNode < scope){
        // cout << root->idNode << "Here";
        return isNotDuplicate(root->right, name, scope);
    }
    return isNotDuplicate(root->left, name, scope);
}

void SymbolTable::insert(BinaryNode *p, string name, string type, string condition, int &scope, int &numcomp, int &numsplay)
{
    // First, insert value to Tree as BST Tree
    if (this->root == nullptr)
    {
        root = new BinaryNode(name, type, condition, scope);
        cout << numcomp << ' ' << numsplay << endl;
        return;
    }

    if (p->idNode == name && p->levelNode == scope && p->typeNode == type)
    {
        cout << numcomp << ' ' << numsplay << endl;
        return;
    }

    else
    {
        BinaryNode *newNode = new BinaryNode(name, type, condition, scope);
        BinaryNode *temp = root;
        while (temp != nullptr)
        {
            if (!isBigger(newNode, temp) && temp->left == nullptr)
            { // If node is smaller than root + left of root is null, go to left and insert
                //cout << "OK1";
                temp->left = newNode;
                newNode->parent = temp;
                numcomp++;
                splaying(newNode,name, scope, numcomp, numsplay);
                break;
            }
            else if (!isBigger(newNode, temp))
            {
                temp = temp->left;
                numcomp++;
            }
            else if (isBigger(newNode, temp) && temp->right == nullptr)
            {
                temp->right = newNode;
                numcomp++;
                newNode->parent = temp;
                splaying(newNode, name, scope, numcomp, numsplay);
                //cout << "OK2";
                break;
            }
            else if(isBigger(newNode, temp))
            {
                numcomp++;
                temp = temp->right;
            }
            else {
                splaying(temp, name, scope, numcomp, numsplay);
                cout << numcomp << ' ' << numsplay << endl;
                return;
            }
        }
    }

    // Finally, print <numcomp><numsplay>
    cout << numcomp << ' ' << numsplay << endl;
    numcomp = numsplay = 0;
    return;
}

BinaryNode* SymbolTable::search(string line, string name, int& scope, int& numcomp, int& numsplay){
    BinaryNode* ret = nullptr;
    BinaryNode* curr = this->root;
    BinaryNode* prev = nullptr;

    while(curr != nullptr){
        prev = curr;
        if (name.compare(curr->idNode) < 0){
            numcomp++;
            curr = curr->left;
        }
        else if (name.compare(curr->idNode) > 0) {
            numcomp++;
            curr = curr->right;
        } 
        else {
            numcomp++;
            ret = curr;
            break;
        }
    }
    if (ret != nullptr){
        //numsplay++;
        // cout << "Here: " << ret->idNode << endl;
        splaying(ret, name, scope, numcomp, numsplay);
    }
    else if (prev != nullptr){
        //numsplay++;
        // cout << prev->idNode << endl;
        splaying(prev, name, scope, numcomp, numsplay);
    }
    else throw Undeclared(line);
    //cout << numcomp << ' ' << numsplay << endl;
    //numcomp = numsplay = 0;
    return ret;
}
void SymbolTable::assign(BinaryNode* root, string line, string name, string value, int& scope, int& numcomp, int& numsplay){
    string lineValue = value;
    if(value.find("(") != string::npos){
        string delimeter = "(";
        size_t pos = value.find(delimeter);
        value = value.substr(0, pos);
        lineValue = lineValue.substr(pos+1,(lineValue.size()-2 - pos));
    }
    // cout << lineValue << endl;
    BinaryNode* toAssign = search(line, value, scope, numcomp, numsplay);
    BinaryNode* ret = search(line, name, scope, numcomp, numsplay);
    // cout << toAssign->typeNode << ' ' << ret->typeNode << endl;

    string delimeter = "->";
    size_t posVar = string::npos;
    if (ret !=nullptr) posVar = ret->typeNode.find(delimeter);
    size_t posVal = string::npos;
    if (toAssign != nullptr) posVal = toAssign->typeNode.find(delimeter);
    // cout << posVal << ' ' << posVar;
    // If var is name of function and value is a function
    if (posVal != string::npos && posVar != string::npos){
        // If line is ASSIGN <ID (of a function)> <function>
        //cout << "here\n";
        string typeVal = toAssign->typeNode.substr(posVal+2,toAssign->typeNode.size());
        string typeVar = ret->typeNode.substr(posVar+2, ret->typeNode.size());
        if (typeVal != typeVar) throw TypeMismatch(line);
        //string ListParaVar = ret->typeNode.substr(1, posVar-2);
        string ListParaVal = toAssign->typeNode.substr(1,posVal-2);
        // cout << ListParaVal  << "\n"  << lineValue << endl;
        string delimeter = ",";
        size_t pos1 , pos2;
        while(((pos1 = ListParaVal.find(delimeter)) != string::npos) && ((pos2 = lineValue.find(delimeter)) != string::npos)){
            string parameter = ListParaVal.substr(0,pos1);
            string value = lineValue.substr(0, pos2);
            // cout << "Here: " << parameter <<' ' << value << endl;
            if (parameter == "number" && !isNum(value))
                throw TypeMismatch(line);
            else if(parameter == "string" && !isString(value))
                throw TypeMismatch(line);
            else{
                ListParaVal.erase(0, pos1 + delimeter.length());
                lineValue.erase(0, pos2+delimeter.length());
            }
        }
        if (ListParaVal == "number" && !isNum(lineValue))
            throw TypeMismatch(line);
        else if (ListParaVal == "string" && !isString(lineValue))
            throw TypeMismatch(line);
        // cout << ListParaVal << ' ' << lineValue << endl;
        // cout << type << endl;
    }
    else if(posVal != string::npos){
        /* If line is: ASSIGN <ID(not ID of a function)> <function> */
        // cout << "Here";
        string typeVal = toAssign->typeNode.substr(posVal+2,toAssign->typeNode.size());
        if (ret != nullptr && typeVal != ret->typeNode) throw TypeMismatch(line);
        string ListParaVal = toAssign->typeNode.substr(1,posVal-2);
        // cout << ListParaVal  << "\n"  << lineValue << endl;
        string delimeter = ",";
        size_t pos1 = string::npos, pos2 = string::npos;
        while(((pos1 = ListParaVal.find(delimeter)) != string::npos) && ((pos2 = lineValue.find(delimeter)) != string::npos)){ 
            string parameter = ListParaVal.substr(0,pos1);
            string value = lineValue.substr(0, pos2);
            // cout << "Here: " << parameter <<' ' << value << endl;
            if (parameter == "number" && !isNum(value))
                throw TypeMismatch(line);
            else if(parameter == "string" && !isString(value))
                throw TypeMismatch(line);
            else{
                lineValue.erase(0, pos2 + delimeter.length());
                ListParaVal.erase(0, pos1 + delimeter.length());

            }
        }
        // cout << "here: " << lineValue << ' ' << ListParaVal << endl;
        if (ListParaVal == "number" && !isNum(lineValue))
            throw TypeMismatch(line);
        else if (ListParaVal == "string" && !isString(lineValue))
            throw TypeMismatch(line);
        else if ((ListParaVal == "" && lineValue != "") || (ListParaVal != "" && lineValue == ""))
            throw TypeMismatch(line);
    }
    else if(posVar != string::npos){
        //cout << "here\n";
        string typeVar = ret->typeNode.substr(posVar+2, ret->typeNode.size());
        // cout << typeVar << endl;
        // cout << toAssign->typeNode << endl;
        if (toAssign != nullptr && typeVar != toAssign->typeNode) throw TypeMismatch(line);
        else if (toAssign == nullptr && ((typeVar == "number" && !isNum(value)) || (typeVar == "string" && !isString(value))))
            throw TypeMismatch(line);
    }
    else if(ret->typeNode != toAssign->typeNode) throw TypeMismatch(line);
    else if((ret->typeNode == "number" && !isNum(value)) || (ret->typeNode == "string" && !isString(value))){
        // cout << "Here\n";
        throw TypeMismatch(line);
    }
    // if (toAssign == nullptr) cout << "NULL" << endl;
    // cout << ret->typeNode << endl;
    cout << numcomp << ' ' << numsplay << endl;
    numcomp = numsplay = 0;
    return;
}

BinaryNode* SymbolTable::searchScope(int &scope){
    BinaryNode* curr = this->root;
    while(curr != nullptr){

    }
    return curr;
}
BinaryNode* SymbolTable::subtreeMax(BinaryNode* subRoot){
    BinaryNode* curr = subRoot;
    while(curr->right != nullptr)
        curr = curr->right;
    return curr;
}
BinaryNode* SymbolTable::subtreeMin(BinaryNode* subRoot){
    BinaryNode* curr = subRoot;
    while (curr->left != NULL)
        curr = curr->left;
    return curr; 
}
void SymbolTable::getOut(string name, int& scope){

}

void SymbolTable::inOrder(BinaryNode* root){
    if (root != nullptr)
    {
        inOrder(root->left);
        cout << root->idNode << root->typeNode << ' ';
        inOrder(root->right);
    }
}

void SymbolTable::run(string filename)
{
    ifstream file;
    file.open(filename);
    
    int scope = 0, numsplay = 0, numcomp = 0;
    string line;
    while (getline(file, line))
    {
        string parameter[4];
        splitString(line, parameter);
        string action = parameter[0];
        checkLineValid(line, action, parameter);
        if (action == "INSERT"){
            //cout << parameter[1] << ' ' << scope << endl;
            if (!isNotDuplicate(root, parameter[1], scope))
                throw Redeclared(line);
            else if(scope != 0 && parameter[2].find("->") != string::npos)
                throw InvalidDeclaration(line);
            else 
                this->insert(root, parameter[1], parameter[2], parameter[3], scope, numcomp, numsplay);
        }
        else if (action == "ASSIGN"){
            this->assign(root, line, parameter[1], parameter[2], scope, numcomp, numsplay);
        }
        else if (action == "BEGIN"){
            scope++;
        }
        else if (action == "END"){
            scope--;
        }
        else if (action == "LOOKUP"){}
        else if (action == "PRINT"){}
        //cout << parameter[0]
    }
    //this->inOrder(root);
    return;
}

// Kanryoushimasu. Omedetou gozaimasu!