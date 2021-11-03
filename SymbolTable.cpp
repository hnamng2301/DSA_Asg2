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
            if ((lineName[i] >= 'A' && lineName[i] <= 'Z') || (lineName[i] >= 'a' && lineName[i] <= 'z') 
                || (lineName[i] >= '0' && lineName[i] <= '9') || lineName[i] == '_')
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

        if (!validID(token1))
            return 0;
        string argList = token2.substr(1, token2.size() - 2);
        pos = 0, delimeter = ",";
        while ((pos = argList.find(delimeter)) != string::npos)
        {
            string token = argList.substr(0, pos);
            argList.erase(0, pos + delimeter.length());
            if (!isString(token) && !isNum(token) && !validID(token) && token.find(' ') != string::npos)
                return 0;
        }
        if (!isString(argList) && !isNum(argList) && !validID(argList) && argList.find(' ') != string::npos)
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
        if (!isFunction(arg[2]) && !isNum(arg[2]) && !isString(arg[2]) && !validID(arg[2])){
            throw InvalidInstruction(line);
        }
    }
    else if(action == "BEGIN" || action == "END" || action == "PRINT"){
        if (line != "BEGIN" && line != "END" && line != "PRINT") throw InvalidInstruction(line);
    }
    else if(action == "LOOKUP"){
        string test = arg[0];
        test += ' ' + arg[1];
        if (line != test || !validID(arg[1]))
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

void SymbolTable::leftRotate(BinaryNode* x){
    BinaryNode* y = x->right;
    x->right = y->left;
    if(y->left != nullptr){
        y->left->parent = x;
    }
    y->parent = x->parent;
    if(x->parent == nullptr){
        this->root = y;
    }
    else if(x == x->parent->left){
        x->parent->left = y;
    }
    else{
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}
void SymbolTable::rightRotate(BinaryNode* x){
    BinaryNode* y = x->left;
    x->left = y->right;
    if(y->right != nullptr){
        y->right->parent = x;
    }
    y->parent = x->parent;
    if(x->parent == nullptr){
        this->root = y;
    }
    else if(x == x->parent->right){
        x->parent->right = y;
    }
    else{
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}
void SymbolTable::splaying(BinaryNode *x, string name, int &scope, int &numcomp, int &numsplay)
{
    if (!x->parent) return;

    while (x->parent)
    {
        if (!x->parent->parent)
        {
            if (x == x->parent->left)
            {
                // zig rotation
                rightRotate(x->parent);
            }
            else
            {
                // zag rotation
                leftRotate(x->parent);
            }
        }
        else if (x == x->parent->left && x->parent == x->parent->parent->left)
        {
            // zig-zig rotation
            rightRotate(x->parent->parent);
            rightRotate(x->parent);
        }
        else if (x == x->parent->right && x->parent == x->parent->parent->right)
        {
            // zag-zag rotation
            leftRotate(x->parent->parent);
            leftRotate(x->parent);
        }
        else if (x == x->parent->right && x->parent == x->parent->parent->left)
        {
            // zig-zag rotation
            leftRotate(x->parent);
            rightRotate(x->parent);
        }
        else
        {
            // zag-zig rotation
            rightRotate(x->parent);
            leftRotate(x->parent);
        }
    }
    numsplay++;
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

void SymbolTable::isNotDuplicate(BinaryNode* root, string name, int scope, bool& isNot){
    if (root == nullptr){
        isNot = 1;
        return;
    } 

    if (root->idNode == name && root->levelNode == scope) {
        isNot = false;
        return;
    }

    if ((root->levelNode == scope && root->idNode.compare(name) < 0) || root->levelNode < scope){
        isNotDuplicate(root->right, name, scope, isNot);
    }
    isNotDuplicate(root->left, name, scope, isNot);
}

void SymbolTable::insert(BinaryNode *p, string name, string type, string condition, int &scope, int &numcomp, int &numsplay)
{
    // First, insert value to Tree as BST Tree
    if (this->root == nullptr)
    {
        root = new BinaryNode(name, type, condition, scope);
        cout << numcomp << ' ' << numsplay << endl;
        numcomp = numsplay = 0;
        return;
    }

    if (p->idNode == name && p->levelNode == scope && p->typeNode == type)
    {
        cout << numcomp << ' ' << numsplay << endl;
        numcomp = numsplay = 0;
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
                break;
            }
            else if(isBigger(newNode, temp))
            {
                numcomp++;
                temp = temp->right;
            }
            else {
                // numcomp++;
                splaying(temp, name, scope, numcomp, numsplay);
                cout << numcomp << ' ' << numsplay << endl;
                return;
            }
        }
    }
    // Finally, print <numcomp> <numsplay>
    cout << numcomp << ' ' << numsplay << endl;
    numcomp = numsplay = 0;
    return;
}

void SymbolTable::inTree(BinaryNode* root, string name, BinaryNode*& toFind){
    if(root != nullptr){
        if (root->idNode == name){
            toFind = root;
            return;
        }
        inTree(root->right, name, toFind);
        inTree(root->left, name, toFind);
    }
}
BinaryNode* SymbolTable::search(string line, string name, int& scope, int& numcomp, int& numsplay){
    BinaryNode *ret = nullptr;
    BinaryNode *curr = this->root;
    BinaryNode *prev = nullptr;

    if (this->root == nullptr)
        throw Undeclared(line);

    BinaryNode *toSearch = nullptr;
    inTree(root, name, toSearch);
    // if(toSearch == nullptr) cout << "NULL\n";
    if(toSearch == nullptr) return nullptr;

    while (curr != nullptr)
    {
        prev = curr;
        if (toSearch->idNode == curr->idNode)
        {
            numcomp++;
            ret = curr;
            break;
        }
        else if (!isBigger(toSearch, curr))   // if toSearch < curr
        {
            numcomp++;
            curr = curr->left;
        }
        else if (isBigger(toSearch, curr))  // if toSearch > curr
        {
            numcomp++;
            curr = curr->right;
        }
    }

    if (ret != nullptr)
    {
        splaying(ret, name, scope, numcomp, numsplay);
    }
    else if (prev != nullptr)
    {
        splaying(prev, name, scope, numcomp, numsplay);
    }

    return ret;
}
void SymbolTable::assign(BinaryNode* root, string line, string name, string value, int& scope, int& numcomp, int& numsplay){
    /* ASSIGN <name> <value> */
    string lineValue = value;
    if (value.find("(") != string::npos)  // Take List of parameter: sum(a,b) ---> a,b
    {
        string delimeter = "(";
        size_t pos = value.find(delimeter);
        value = value.substr(0, pos);
        lineValue = lineValue.substr(pos + 1, (lineValue.size() - 2 - pos));
    }

    if (this->root == nullptr)
        throw Undeclared(line);

    BinaryNode* ret = nullptr;
    BinaryNode* toAssign = nullptr;
    if (validID(value))
        toAssign = search(line, value, scope, numcomp, numsplay);
    if (validID(name))
        ret = search(line, name, scope, numcomp, numsplay);

    if (!toAssign && !ret && validID(value) && validID(name)) throw Undeclared(line);   // if both are ID and not declared
    if (validID(value) && !toAssign) throw Undeclared(line);     // if value is a ID and not declared

    string delimeter = "->";
    size_t posVar = string::npos;
    size_t posVal = string::npos;
    if (ret != nullptr)
        posVar = ret->typeNode.find(delimeter);
    if (toAssign != nullptr)
        posVal = toAssign->typeNode.find(delimeter);

    if (posVar != string::npos)     // If name is ID of function and is a function's ID
    {
        throw TypeMismatch(line);
    }

    if (ret && toAssign)    // if both are ID 
    {
        if (posVal == string::npos && posVar == string::npos)   // if both is variable and same type (except type function)
        { 
            if (ret->typeNode == toAssign->typeNode)
            {
                cout << numcomp << ' ' << numsplay << endl;
                numcomp = numsplay = 0;
                return;
            }
        }
    }
    // cout << "Null\n";
    if (validID(name) && validID(value))  // if <name> and <value> are both ID. <name> maybe is not declared.
    {
        if (toAssign && posVal != string::npos)    /* If line is: ASSIGN <ID(not ID of a function)> <function call> */
        {
            // Check List of parameter pass in function call is same type as argument in function def
            string typeVal = toAssign->typeNode.substr(posVal + 2, toAssign->typeNode.size());
            string ListParaVal = toAssign->typeNode.substr(1, posVal - 2);
            string delimeter = ",";
            size_t pos1 = string::npos, pos2 = string::npos;
            while (((pos1 = ListParaVal.find(delimeter)) != string::npos) && ((pos2 = lineValue.find(delimeter)) != string::npos))
            {
                string parameter = ListParaVal.substr(0, pos1);
                string value = lineValue.substr(0, pos2);
                // cout << "Here: " << parameter <<' ' << value << endl;

                // if in argument List of ID has type "number"
                if (parameter == "number"){
                    // if value pass in parameter is not a number
                    if(!isNum(value)){
                        // if it is a valid ID, then check it is declared?
                        if(validID(value)){
                            BinaryNode* val = search(line,value,scope,numcomp,numsplay);
                            if(val == nullptr) throw Undeclared(line);
                            if(val->typeNode.find("->") != string::npos){
                                throw TypeMismatch(line);
                            }
                            else if(val->typeNode != "number") throw TypeMismatch(line);
                        }
                        else    // if not, throw
                            throw TypeMismatch(line);
                    }
                }
                // if in argument List of ID (in INSERT) has type "string"
                else if (parameter == "string"){
                    // if value pass in parameter (in ASSIGN) is not a string
                    // cout << "Here\n";
                    if(!isString(value)){
                        // if it is a valid ID, then check if it is declared?
                        if(validID(value)){
                            BinaryNode* val = search(line,value,scope,numcomp,numsplay);
                            if(val == nullptr) throw Undeclared(line);
                            if(val->typeNode.find("->") != string::npos){
                                throw TypeMismatch(line);
                            }
                            else if(val->typeNode != "string") throw TypeMismatch(line);
                        }
                        else    // if not, throw
                            throw TypeMismatch(line);
                    }
                }

                lineValue.erase(0, pos2 + delimeter.length());
                ListParaVal.erase(0, pos1 + delimeter.length());
            }
            // cout << "here: " << lineValue << ' ' << ListParaVal << endl;
            if (ListParaVal == "number"){
                if (!isNum(lineValue)){
                    if (validID(lineValue))
                    {
                        BinaryNode *val = search(line, lineValue, scope, numcomp, numsplay);
                        if (val == nullptr)
                            throw Undeclared(line);
                        if (val->typeNode.find("->") != string::npos)
                        {   
                            throw TypeMismatch(line);
                        }
                        else if(val->typeNode != "number") throw TypeMismatch(line);
                    }
                    else
                        throw TypeMismatch(line);
                }
            }
            else if (ListParaVal == "string")
            {
                if (!isString(lineValue))
                {
                    if (validID(lineValue))
                    {
                        BinaryNode *val = search(line, lineValue, scope, numcomp, numsplay);
                        if (val == nullptr)
                            throw Undeclared(line);
                        if (val->typeNode.find("->") != string::npos)
                        {
                            throw TypeMismatch(line);
                        }
                        else if(val->typeNode != "string") throw TypeMismatch(line);
                    }
                    else
                        throw TypeMismatch(line);
                }
            }
            else if ((ListParaVal == "" && lineValue != "") || (ListParaVal != "" && lineValue == ""))
                throw TypeMismatch(line);

            if (!ret) throw Undeclared(line);  // if function in value is OK, check <name>ID
            if (ret != nullptr && typeVal != ret->typeNode)
                throw TypeMismatch(line);
        }
        else if (ret && toAssign && ret->typeNode != toAssign->typeNode)  /* If line is: ASSIGN <ID (not ID if a function)> <ID (not ID of a function)> */
        {
            // if both ID are not function and not same type
            throw TypeMismatch(line);
        }
        else throw Undeclared(line);
    }
    
    else if ((ret->typeNode == "number" && !isNum(value)) 
            || (ret->typeNode == "string" && !isString(value))) /* If line is: ASSIGN <ID (not ID of a function)> <value> */
    {
        // if (<name> is ID and found) and <value> is value
        throw TypeMismatch(line);
    }

    cout << numcomp << ' ' << numsplay << endl;
    numcomp = numsplay = 0;
    return;
}

BinaryNode* SymbolTable::searchScope(string line, string name, int& scope, int& numcomp, int& numsplay){
    BinaryNode* curr = this->root;
    BinaryNode* ret = nullptr;

    bool isFound = 0;
    while(curr != nullptr){
        if (scope == curr->levelNode) {
            ret = curr;
            isFound = 1;
            break;
        }
        curr = curr->left;
    }
    if (!isFound){
        curr = root;
        while (curr != nullptr)
        {
            if (scope == curr->levelNode)
            {
                ret = curr;
                break;
            }
            curr = curr->right;
        }
    }
    return ret;
}
BinaryNode* SymbolTable::subtreeMax(BinaryNode* subRoot){
    while(subRoot->right != nullptr)
        subRoot = subRoot->right;
    return subRoot;
}

void SymbolTable::split(BinaryNode* &x, BinaryNode* &s, BinaryNode* &t, string name, int& scope, int& numcomp, int& numsplay){
    splaying(x, name, scope, numcomp, numsplay);
    if (x->right){
        t = x->right;
        t->parent = nullptr;
    }
    else{
        t = nullptr;
    }
    s = x;
    s->right = nullptr;
    x = nullptr;
}
BinaryNode* SymbolTable::join(BinaryNode* s, BinaryNode* t, string name, int& scope, int& numcomp, int& numsplay){
    if(!s) return t;
    if(!t) return s;
    BinaryNode* x = subtreeMax(s);
    splaying(x, name, scope, numcomp, numsplay);
    x->right = t;
    t->parent = x;
    return x;
}

void SymbolTable::getOut(string line, string name, int& scope, int& numcomp, int& numsplay){
    BinaryNode* x = nullptr;
    while((x = searchScope(line, name, scope, numcomp, numsplay)) != nullptr){
        BinaryNode *t;
        BinaryNode *s;
        if (x == nullptr)
        {
            numcomp = numsplay = 0;
            return;
        }
        split(x, s, t, name, scope, numcomp, numsplay);

        if (s->left)
        {
            s->left->parent = nullptr;
        }
        root = join(s->left, t, name, scope, numcomp, numsplay);

        delete s;
        s = nullptr;
        numcomp = numsplay = 0;
    }
}

void SymbolTable::lookUp(string line, string name, int& scope, int& numcomp, int& numsplay){
    BinaryNode *ret = nullptr;
    BinaryNode *curr = this->root;
    BinaryNode *prev = nullptr;

    if (this->root == nullptr)
        throw Undeclared(line);

    BinaryNode *toSearch = nullptr;
    inTree(root, name, toSearch);

    if(toSearch == nullptr) throw Undeclared(line);

    while (curr != nullptr)
    {
        prev = curr;
        if (toSearch->idNode == curr->idNode)
        {
            numcomp++;
            ret = curr;
            break;
        }
        else if (!isBigger(toSearch, curr))   // if toSearch < curr
        {
            numcomp++;
            curr = curr->left;
        }
        else if (isBigger(toSearch, curr))  // if toSearch > curr
        {
            numcomp++;
            curr = curr->right;
        }
    }


    if (ret != nullptr) splaying(ret, name, scope, numcomp, numsplay);
    else if (prev != nullptr) splaying(prev, name, scope, numcomp, numsplay);

    if(ret == nullptr) throw Undeclared(line);
    else cout << ret->levelNode << endl;

    numcomp = numsplay = 0;
}

void SymbolTable::inOrder(BinaryNode* root){
    if (root != nullptr)
    {
        inOrder(root->left);
        cout << root->idNode << "//" << root->levelNode << ' ';
        inOrder(root->right);
    }
}

void SymbolTable::preOrder(BinaryNode* root, string& res){
    if (root != nullptr)
    {
        res += root->idNode; res += "//"; res += to_string(root->levelNode); res += ' ';
        preOrder(root->left, res);
        preOrder(root->right, res);
    }
}
void SymbolTable::printPre(string& res){
    this->preOrder(this->root, res);
    res = res.substr(0, res.size() - 1);
    cout << res;
}

void SymbolTable::clear(BinaryNode* root){
    if(root == nullptr) return;

    clear(root->left);
    clear(root->right);
    delete root;
}
SymbolTable::~SymbolTable(){
    this->clear(root);
}

void SymbolTable::run(string filename)
{
    ifstream file;
    file.open(filename);

    int scope = 0, numsplay = 0, numcomp = 0;
    int beginCount = 0, endCount = 0;
    string line;
    while (!file.eof())
    {
        getline(file, line);
        string parameter[4];
        splitString(line, parameter);
        string action = parameter[0];
        checkLineValid(line, action, parameter);

        if (action == "INSERT"){
            bool isNot = 0;
            isNotDuplicate(root, parameter[1], scope, isNot);
            if(!isNot)
                throw Redeclared(line);
            else if(scope != 0 && parameter[2].find("->") != string::npos && parameter[3] != "true")
                throw InvalidDeclaration(line);
            else
                this->insert(root, parameter[1], parameter[2], parameter[3], scope, numcomp, numsplay);
        }
        else if (action == "ASSIGN"){
            this->assign(root, line, parameter[1], parameter[2], scope, numcomp, numsplay);
        }
        else if (action == "BEGIN"){
            beginCount++;
            scope++;
        }
        else if (action == "END"){
            beginCount--;
            endCount++;
            if (beginCount == 0) endCount = 0;
            if (beginCount < 0) throw UnknownBlock(); // End when there is no Begin
            getOut(line ,"", scope, numcomp, numsplay);
            scope--;
            numcomp = numsplay = 0;
        }
        else if (action == "LOOKUP"){
            this->lookUp(line, parameter[1], scope, numcomp, numsplay);
        }
        else if (action == "PRINT"){
            string res = "";
            this->printPre(res);
            if (this->root != nullptr) cout << endl;
        }
    }

    if (beginCount == 0)
    {
        return;
    }
    else if (beginCount > 0)
        throw UnclosedBlock(beginCount);
    if (beginCount < endCount)
    {
        throw UnknownBlock();
    }
    else if (beginCount >= endCount)
    {
        throw UnclosedBlock(beginCount);
    }
    else
    {
        return;
    }
    return;
}

// Kanryoushimasu. Omedetou gozaimasu!
