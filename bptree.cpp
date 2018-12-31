#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>

using namespace std;
class Element {
  string name;
  int year, month, day;
  string lang;
  int runtime;
  int indx; // Index in DB array
  double revenue;
  int valid;

public:
  Element(string n, int y, int m, int d, string l, int rt, double rev)
      : name(n), year(y), month(m), day(d), lang(l), runtime(rt), revenue(rev),
        valid(1){};

  void setIndx(int num) { indx = num; }

  int getIndx() { return indx; }

  int getKey() const { return year * 10000 + month * 100 + day; }

  string getName() const { return name; }

  int getYM() const { return year * 100 + month; }

  void notValid() { valid = 0; }

  int isValid() { return valid; }

  void printEl() const {
    if (valid) {
      cout << lang << ", " << name << ", " << year << "-" << month << "-" << day
           << ", " << revenue << ", " << runtime << endl;
    }
  }
};

class Database {
  Element **arr;
  int size;
  int movieNum;

public:
  Database(int s) : movieNum(0), size(s) { arr = new Element *[size]; };

  Database(const Database &d) {
    arr = new Element *[d.size];
    for (int i = 0; i < movieNum; i++) {
      arr[i] = d.arr[i];
    }
  }

  Database &insert(Element *el) {
    arr[movieNum++] = el;
    return *this;
  }

  Database &sort() {
    int j;
    Element *K;
    for (int i = 1; i < movieNum; i++) {
      K = arr[i];
      j = i - 1;
      while (j >= 0 && arr[j]->getKey() > K->getKey()) {
        arr[j + 1] = arr[j];
        j = j - 1;
      }
      arr[j + 1] = K;
    }
    return *this;
  }

  Database &setIndx() {
    for (int i = 0; i < movieNum; i++) {
      arr[i]->setIndx(i);
    }
    return *this;
  }

  Element *mark(string name, int k) {
    int low = 0;
    int high = movieNum;
    int mid;

    while (low <= high) {
      mid = (low + high) / 2;
      string nm = arr[mid]->getName();
      if (!name.compare(arr[mid]->getName()) && arr[mid]->isValid()) {
        arr[mid]->notValid();
        return arr[mid];
      } else if (arr[mid]->getKey() > k) {
        high = mid - 1;
      } else {
        low = mid + 1;
      }
    }
    cout << "Key not found!" << endl;
    return nullptr;
  }

  int findExKey(string name) {
    int comparison;
    for (int i = 0; i < movieNum; i++) {
      comparison = name.compare(arr[i]->getName());
      if (comparison == 0) {
        return arr[i]->getKey();
      }
    }
    return NULL;
  }

  Element *findKey(int key) {
    int low = 0;
    int high = movieNum;
    int mid;
    int res = -1;

    while (low <= high) {
      mid = (low + high) / 2;
      if (arr[mid]->getYM() == key && arr[mid]->isValid()) {
        arr[mid]->printEl();
        res = mid;
        high = mid - 1;
      } else if (arr[mid]->getYM() > key) {
        high = mid - 1;
      } else {
        low = mid + 1;
      }
    }

    if (res == -1) {
      return nullptr;
    } else {
      return arr[res];
    }
  }

  void printDB() const {
    for (int i = 0; i < movieNum; i++) {
      arr[i]->printEl();
    }
  }

  int getMovieNum() const { return movieNum; }

  Element **getMovies() const { return arr; }

  Database(Database &&d) {
    movieNum = d.movieNum;
    arr = d.arr;
    d.arr = nullptr;
  }

  void destroyDB() {
    for (int i = 0; i < movieNum; i++) {
      delete arr[i];
    }
    delete[] arr;
  }
};

string *split(string str, const char *delimt) {
  string *splt = new string[3];

  string delimiter = delimt;

  string word;
  size_t pos = 0;
  int num = 0;
  while ((pos = str.find(delimiter)) != string::npos) {
    word = str.substr(0, pos);
    splt[num++] = word;
    str.erase(0, pos + delimiter.length());
  }
  splt[num] = str;
  return splt;
}

struct lstNode {
  Element *e;
  lstNode *sled;
};

void addLstNode(lstNode **frst, lstNode **lst, Element *el, int &numEl) {
  if (!(*frst)) {
    *frst = new lstNode;
    (*frst)->e = el;
    (*frst)->sled = nullptr;
    *lst = *frst;
  } else {
    lstNode *newNode = new lstNode;
    newNode->e = el;
    newNode->sled = nullptr;
    (*lst)->sled = newNode;
    *lst = newNode;
  }
  numEl++;
}

void destroyLst(lstNode *frst) {
  lstNode *temp = frst;

  while (temp) {
    temp = temp->sled;
    delete frst;
    frst = temp;
  }
}

Database *inputDB(int file) {

  string name, lang, dat;
  int year, month, day, runtime;
  double revenue;
  lstNode *frst = nullptr;
  lstNode *lst = nullptr;

  string *splt = new string[3];

  int numEl = 0;
  cin.ignore(1000, '\n');
  if (!file) {
    cout << "\nNumber of elements: ";
    cin >> numEl;
    Database *db = new Database(numEl);
    for (int i = 0; i < numEl; i++) {
      cout << "\nName: ";
      cin >> name;
      cout << "\nLanguage (eg. en): ";
      cin >> lang;
      cout << "\nYear (YYYY MM DD): ";
      cin >> year >> month >> day;
      cout << "\nRuntime: ";
      cin >> runtime;
      cout << "\nRevenue: ";
      cin >> revenue;

      Element *e = new Element(name, year, month, day, lang, runtime, revenue);
      db->insert(e);
    }
    db->sort();
    db->setIndx();
    return db;
  } else {
    string fname;
    ifstream file;
    cout << "\nFile name: ";
    getline(cin, fname);
    file.open(fname);

    if (file) {
      cout << "\nFile is open" << endl;
    } else {
      cout << "\nFile is not open" << endl;
      return nullptr;
    }

    getline(file, lang);
    string rev = "";
    string rt = "";

    string delimiter = "-";

    string part;
    string quoteName;
    size_t pos = 0;
    int num = 0;
    char delim = '-';
    char quote = '\"';

    int i = 0;
    while (!file.eof()) {

      getline(file, lang, ',');
      getline(file, name, ',');
      if (name[0] == quote) {
        getline(file, quoteName, ',');
        name += quoteName;
      }
      getline(file, dat, ',');
      getline(file, rev, ',');
      getline(file, rt, '\n');

      // Date separation
      num = 0;
      pos = 0;
      part = "";

      while ((pos = dat.find(delimiter)) != string::npos) {
        part = dat.substr(0, pos);
        splt[num++] = part;
        dat.erase(0, pos + delimiter.length());
      }
      splt[num] = dat;

      day = stoi(splt[2]);

      month = stoi(splt[1]);

      year = stoi(splt[0]);

      runtime = stoi(rt);
      revenue = stod(rev);

      Element *e = new Element(name, year, month, day, lang, runtime, revenue);
      addLstNode(&frst, &lst, e, numEl);
    }
    Database *db = new Database(numEl);

    lstNode *temp = frst;
    while (temp) {
      db->insert(temp->e);
      temp = temp->sled;
    }

    file.close();
    destroyLst(frst);

    db->sort();
    db->setIndx();
    return db;
  }
}

class Node {
  int numEl;

  Node *next;
  Node *parent;

  void **elArr;
  int *keys;
  bool isLeaf;
  int deg;

public:
  Node(int d)
      : numEl(0), next(nullptr), elArr(nullptr), isLeaf(true), deg(d),
        parent(nullptr) {
    keys = new int[d];
    elArr = new void *[d];
  }

  bool isFull(int deg) const {
    if (numEl < deg - 1) {
      return false;
    } else {
      return true;
    }
  }

  bool leaf() const {
    if (isLeaf) {
      return true;
    } else {
      return false;
    }
  }

  void notLeaf() { isLeaf = false; }

  bool hasKey(int key) const {
    for (int i = 0; i < numEl; i++) {
      if (keys[i] == key) {
        return true;
      }
    }
    return false;
  }

  Node *getParent() { return parent; }

  void setParent(Node *ptr) { parent = ptr; }

  Node *getNext() { return next; }

  void setNext(Node *nxt) { next = nxt; }

  int getNumEl() const { return numEl; }

  int getDeg() const { return deg; }

  int *getKeys() { return keys; }

  void **getElArr() { return elArr; }

  void incNum() { numEl++; }

  void decNum() { numEl--; }

  void print() const {
    cout << "| ";
    for (int i = 0; i < numEl; i++) {
      cout << keys[i] << " ";
    }
    cout << " | ";
  }

  void printToFile(ofstream &file) const {
    file << "| ";
    for (int i = 0; i < numEl; i++) {
      file << keys[i] << " ";
    }
    file << " | ";
  }
};

Node *addToParent(int &key, Node *leftNode, Node *rightNode, Node **tRoot);

int getIndx(int deg) {
  int indx;
  if (deg % 2 == 0) {
    indx = deg / 2;
  } else {
    indx = (deg / 2) + 1;
  }
  return indx;
}

int findPos(int key, int numEl, int *arr) {
  int pos;
  for (pos = 0; arr[pos] < key && pos < numEl; pos++)
    ;
  return pos;
}

Node *splitLeafNode(int &key, Node **tRoot, Element *el, Node *oldLeaf) {

  int *keys = oldLeaf->getKeys();
  void **elArr = oldLeaf->getElArr();
  int deg = oldLeaf->getDeg();
  int numEl = oldLeaf->getNumEl();
  int indx = getIndx(deg);

  cout << "\nSplitting leaf node...\n";
  Node *newLeafNode = new Node(deg);
  int *newLeafKeys = newLeafNode->getKeys();
  void **newLeafEl = newLeafNode->getElArr();

  int pos = findPos(key, numEl, keys);
  int i = 0;
  int j = 0;

  void **tempElArr = new void *[deg];
  int *tempKeys = new int[deg];

  // Getting the sorted array of keys + elements
  while (i < numEl) {
    if (j == pos) {
      j++;
    }
    tempElArr[j] = elArr[i];
    tempKeys[j] = keys[i];
    i++;
    j++;
  }
  tempElArr[pos] = el;
  tempKeys[pos] = key;

  // oldLeaf->numEl = 0
  while (oldLeaf->getNumEl()) {
    oldLeaf->decNum();
  }

  for (i = indx, j = 0; i < deg; i++, j++) {
    newLeafNode->incNum();
    newLeafKeys[j] = tempKeys[i];
    newLeafEl[j] = tempElArr[i];
  }

  for (i = 0; i < indx; i++) {
    oldLeaf->incNum();
    keys[i] = tempKeys[i];
    elArr[i] = tempElArr[i];
  }
  newLeafNode->setParent(oldLeaf->getParent());
  newLeafNode->setNext(oldLeaf->getNext());
  oldLeaf->setNext(newLeafNode);
  key = keys[oldLeaf->getNumEl() - 1];

  delete[] tempElArr;
  delete tempKeys;

  Node *toParent = addToParent(key, oldLeaf, newLeafNode, tRoot);

  return toParent;
}

Node *splitIntNode(int &key, Node **tRoot, Node *oldInt, Node *rightNode,
                   int leftIndx) {
  int deg = oldInt->getDeg();
  int *oldIntKeys = oldInt->getKeys();
  void **oldIntElArr = oldInt->getElArr();
  int numEl = oldInt->getNumEl();

  cout << "\nSplitting internal node...\n";
  Node *newIntNode = new Node(deg);
  int *newIntKeys = newIntNode->getKeys();
  void **newIntElArr = newIntNode->getElArr();

  int pos = findPos(key, numEl, oldIntKeys);
  int i = 0;
  int j = 0;

  int *tempKeys = new int[deg];
  void **tempElArr = new void *[deg];

  // Getting the sorted array of keys + elements

  for (i = 0, j = 0; i < numEl; i++, j++) {
    if (j == leftIndx)
      j++;
    tempKeys[j] = oldIntKeys[i];
  }

  for (i = 0, j = 0; i < numEl + 1; i++, j++) {
    if (j == leftIndx + 1)
      j++;
    tempElArr[j] = oldIntElArr[i];
  }

  tempKeys[leftIndx] = key;
  tempElArr[leftIndx + 1] = rightNode;

  // oldLeaf->numEl = 0
  while (oldInt->getNumEl()) {
    oldInt->decNum();
  }

  int indx = getIndx(deg);

  for (i = 0; i < indx - 1; i++) {
    oldIntKeys[i] = tempKeys[i];
    oldIntElArr[i] = tempElArr[i];
    oldInt->incNum();
  }
  oldIntElArr[i] = tempElArr[i];
  for (i = indx, j = 0; i < deg; i++, j++) {
    newIntKeys[j] = tempKeys[i];
    newIntElArr[j] = tempElArr[i];
    newIntNode->incNum();
  }
  newIntElArr[j] = tempElArr[i];
  key = tempKeys[indx - 1];
  newIntNode->setParent(oldInt->getParent());

  oldIntElArr = oldInt->getElArr();
  for (i = 0; i <= oldInt->getNumEl(); i++) {
    static_cast<Node *>(oldIntElArr[i])->setParent(oldInt);
  }
  newIntElArr = newIntNode->getElArr();
  for (i = 0; i <= newIntNode->getNumEl(); i++) {
    static_cast<Node *>(newIntElArr[i])->setParent(newIntNode);
  }

  oldInt->notLeaf();
  newIntNode->notLeaf();

  Node *toParent = addToParent(key, oldInt, newIntNode, tRoot);
  return toParent;
}

Node *addToNode(int key, Node *destNode, Node *rightNode, int leftIndx) {

  int elNum = destNode->getNumEl();
  int *nodeKeys = destNode->getKeys();
  int pos = findPos(key, elNum, nodeKeys);
  void **nodeElArr = destNode->getElArr();

  int i = elNum;

  while (i > leftIndx) {
    nodeKeys[i] = nodeKeys[i - 1];
    nodeElArr[i + 1] = nodeElArr[i];
    i--;
  }
  destNode->incNum();

  nodeElArr[leftIndx + 1] = rightNode;
  nodeKeys[leftIndx] = key;
  return destNode;
}

int getLftIndx(Node *parent, Node *left) {

  void **elArr = parent->getElArr();

  int lftIndx;
  for (lftIndx = 0; lftIndx <= parent->getNumEl() && elArr[lftIndx] != left;
       lftIndx++)
    ;
  return lftIndx;
}

Node *addToParent(int &key, Node *leftNode, Node *rightNode, Node **tRoot) {

  Node *parentNode = leftNode->getParent();
  int deg = leftNode->getDeg();

  if (parentNode) {
    parentNode->notLeaf();
    int leftIndx = getLftIndx(parentNode, leftNode);
    if (parentNode->isFull(deg)) {
      return splitIntNode(key, tRoot, parentNode, rightNode, leftIndx);
    } else {
      return addToNode(key, parentNode, rightNode, leftIndx);
    }
  } else { // No internal node
    parentNode = new Node(leftNode->getDeg());
    void **pCptr = parentNode->getElArr();

    int *parentKeys = parentNode->getKeys();
    parentKeys[0] = key;
    parentNode->incNum();
    parentNode->notLeaf();
    pCptr[0] = leftNode;
    pCptr[1] = rightNode;
    Node *n0 = static_cast<Node *>(pCptr[0]);
    Node *n1 = static_cast<Node *>(pCptr[1]);

    leftNode->setParent(parentNode);
    rightNode->setParent(parentNode);

    *tRoot = parentNode;
  }
  return parentNode;
}

Node *findNode(Node *root, int key) {
  Node *temp = root;
  void **elArr = nullptr;
  int *keys = nullptr;
  int pos;

  while (!temp->leaf()) {
    keys = temp->getKeys();
    elArr = temp->getElArr();
    for (pos = 0; keys[pos] < key && pos < temp->getNumEl(); pos++)
      ;

    pos = 0;
    while (pos < temp->getNumEl()) {
      if (key > keys[pos])
        pos++;
      else
        break;
    }
    temp = static_cast<Node *>(elArr[pos]);
  }
  if (temp == root) {
    return root;
  } else {
    return temp;
  }
}

Node *addToLeaf(int key, Element *elPtr, Node *leafNode) {
  int pos;
  int elNum = leafNode->getNumEl();
  void **leafEl = leafNode->getElArr();
  int *leafKeys = leafNode->getKeys();

  for (pos = 0; leafKeys[pos] < key && pos < elNum; pos++)
    ; // SWAP OUT FOR FUNC findPos();
  int i = elNum;

  while (i > pos) {
    leafKeys[i] = leafKeys[i - 1];
    leafEl[i] = leafEl[i - 1];
    i--;
  }
  leafNode->incNum();
  leafEl[i] = elPtr;
  leafKeys[i] = key;

  cout << leafNode->getNumEl() << endl;
  return leafNode;
}

// Get the index of the current node in the Parent Element Array
int getPos(Node *node) {

  Node *parent = node->getParent();
  void **parentElArr = parent->getElArr();
  int numEl = parent->getNumEl();
  int indx;

  for (indx = 0;
       indx <= numEl && node != static_cast<Node *>(parentElArr[indx]); indx++)
    ;
  return indx;
}

Node *getLeftSibling(Node *parent, int indx) {
  void **elArr = parent->getElArr();

  if (indx) {
    return static_cast<Node *>(elArr[indx - 1]);
  } else {
    // Leftmost
    return nullptr;
  }
}

Node *getRightSibling(Node *parent, int indx) {
  void **elArr = parent->getElArr();

  if (!(indx == parent->getNumEl())) {
    return static_cast<Node *>(elArr[indx + 1]);
  } else {
    // Rightmost
    return nullptr;
  }
}

Node *mergeIntNodes(Node *parent, Node *left, Node *right) {

  int *leftKeys = left->getKeys();
  void **leftElArr = left->getElArr();

  int *rightKeys = right->getKeys();
  void **rightElArr = right->getElArr();

  int *parentKeys = parent->getKeys();
  void **parentElArr = parent->getElArr();

  int indx;
  for (indx = 0; indx < parent->getNumEl() &&
                 leftKeys[left->getNumEl() - 1] > parent->getKeys()[indx];
       indx++)
    ;

  leftKeys[left->getNumEl()] = parentKeys[indx];
  left->incNum();

  // Transfer keys from right node to left
  int i;
  int j;
  for (i = left->getNumEl(), j = 0; j < right->getNumEl(); i++, j++) {
    leftKeys[i] = rightKeys[j];
    leftElArr[i] = rightElArr[j];
    left->incNum();
  }
  leftElArr[i] = rightElArr[j];
  left->setNext(right->getNext());
  // Shift parent keys
  for (i = indx; i < parent->getNumEl() - 1; i++) {
    parentKeys[i] = parentKeys[i + 1];
  }

  // Shift parent pointers
  for (i = indx + 1; i < parent->getNumEl(); i++) {
    parentElArr[i] = parentElArr[i + 1];
  }

  delete[] rightElArr;
  delete rightKeys;
  delete right;
  parent->decNum();

  return parent;
}

Node *borrowIntLeft(Node *parent, Node *left, Node *right) {

  int *leftKeys = left->getKeys();
  void **leftElArr = left->getElArr();

  int *rightKeys = right->getKeys();
  void **rightElArr = right->getElArr();

  int *parentKeys = parent->getKeys();
  void **parentElArr = parent->getElArr();

  int indx;
  for (indx = 0; indx < parent->getNumEl() &&
                 leftKeys[left->getNumEl() - 1] > parent->getKeys()[indx];
       indx++)
    ;

  int i = right->getNumEl();
  rightElArr[i + 1] = rightElArr[i];
  for (i = right->getNumEl(); i > 0; i--) {
    rightKeys[i] = rightKeys[i - 1];
    rightElArr[i] = rightElArr[i - 1];
  }

  rightKeys[0] = parentKeys[indx];
  parentKeys[indx] = leftKeys[left->getNumEl() - 1];
  rightElArr[0] = leftElArr[left->getNumEl()];

  left->decNum();
  right->incNum();

  return parent;
}

Node *borrowIntRight(Node *parent, Node *left, Node *right) {

  int *leftKeys = left->getKeys();
  void **leftElArr = left->getElArr();

  int *rightKeys = right->getKeys();
  void **rightElArr = right->getElArr();

  int *parentKeys = parent->getKeys();
  void **parentElArr = parent->getElArr();

  int indx;
  for (indx = 0; indx < parent->getNumEl() &&
                 leftKeys[left->getNumEl() - 1] > parent->getKeys()[indx];
       indx++)
    ;

  leftKeys[left->getNumEl()] = parentKeys[indx];
  parentKeys[indx] = rightKeys[0];
  leftElArr[left->getNumEl() + 1] = rightElArr[0];

  int i;
  for (i = 0; i < right->getNumEl() - 1; i++) {
    rightKeys[i] = rightKeys[i + 1];
    rightElArr[i] = rightElArr[i + 1];
  }
  rightElArr[i] = rightElArr[i + 1];

  right->decNum();
  left->incNum();

  return parent;
}

void updateParentIndx(Node *parent, int indx, int key) {
  int *parentKeys = parent->getKeys();
  parentKeys[indx] = key;
}

Node *borrowLeafRight(Node *parent, Node *left, Node *right, int indx) {
  // indx - Where to update the parent

  int *leftKeys = left->getKeys();
  void **leftElArr = left->getElArr();

  int *rightKeys = right->getKeys();
  void **rightElArr = right->getElArr();

  // Add min right node key as max left node key
  leftKeys[left->getNumEl()] = rightKeys[0];
  leftElArr[left->getNumEl()] = rightElArr[0];
  int newKey = rightKeys[0];

  // Update right node
  for (int i = 0; i < right->getNumEl() - 1; i++) {
    rightKeys[i] = rightKeys[i + 1];
    rightElArr[i] = rightElArr[i + 1];
  }
  left->incNum();
  right->decNum();
  updateParentIndx(left->getParent(), indx, newKey);
  return parent;
}

Node *borrowLeafLeft(Node *parent, Node *left, Node *right, int indx) {
  // indx - Where to update the parent

  int *leftKeys = left->getKeys();
  void **leftElArr = left->getElArr();

  int *rightKeys = right->getKeys();
  void **rightElArr = right->getElArr();

  // Add max left node key as min right node key
  for (int i = right->getNumEl(); i > 0; i--) {
    rightKeys[i] = rightKeys[i - 1];
    rightElArr[i] = rightElArr[i - 1];
  }

  rightElArr[0] = leftElArr[left->getNumEl() - 1];
  leftElArr[left->getNumEl() - 1] = nullptr;
  rightKeys[0] = leftKeys[left->getNumEl() - 1];
  int newKey = leftKeys[left->getNumEl() - 2];

  left->decNum();
  right->incNum();
  updateParentIndx(left->getParent(), indx, newKey);
  return parent;
}

Node *mergeNodes(Node *parent, Node *left, Node *right) {

  int *rightKeys = right->getKeys();
  int *leftKeys = left->getKeys();

  void **rightElArr = right->getElArr();
  void **leftElArr = left->getElArr();

  int *parentKeys = parent->getKeys();
  void **parentElArr = parent->getElArr();

  int indx;
  if (left->getNumEl()) {
    for (indx = 0; indx < parent->getNumEl() &&
                   leftKeys[left->getNumEl() - 1] > parent->getKeys()[indx];
         indx++)
      ;
  } else {
    for (indx = 0; indx < parent->getNumEl() &&
                   leftKeys[left->getNumEl()] > parent->getKeys()[indx];
         indx++)
      ;
  }

  int pos = left->getNumEl();
  int i;
  int j;
  for (i = pos, j = 0; j < right->getNumEl(); i++, j++) {
    leftKeys[i] = rightKeys[j];
    leftElArr[i] = rightElArr[j];
    left->incNum();
  }
  leftElArr[i] = rightElArr[j];
  left->setNext(right->getNext());

  // Shift parent keys
  for (i = indx; i < parent->getNumEl() - 1; i++) {
    parentKeys[i] = parentKeys[i + 1];
  }

  // Shift parent pointers
  for (i = indx + 1; i < parent->getNumEl(); i++) {
    parentElArr[i] = parentElArr[i + 1];
  }

  delete[] rightElArr;
  delete rightKeys;
  delete right;
  parent->decNum();

  return parent;
}

Node *deleteFromLeaf(int key, Node *n, Node **tRoot) {
  bool isRightmost = false;

  Node *currNode = n;
  int *keys = n->getKeys();
  void **elArr = n->getElArr();
  int pos;
  int posInPar;

  Node *parentNode = n->getParent();

  for (pos = 0; pos < n->getNumEl() && key != keys[pos]; pos++)
    ;
  for (posInPar = 0; posInPar < parentNode->getNumEl() &&
                     key > parentNode->getKeys()[posInPar];
       posInPar++)
    ;

  if (pos == n->getNumEl() - 1) {
    isRightmost = true;
  }

  int i = ++pos;

  for (i; i < n->getNumEl(); i++) {
    keys[i - 1] = keys[i];
    elArr[i - 1] = elArr[i];
  }

  n->decNum();
  // Update parent node
  while (currNode) {

    if (currNode->getNumEl() >= currNode->getDeg() / 2) {
      // Just delete the key from the leaf, tree is good
      if (currNode->leaf()) {
        if (isRightmost) {
          // Update parents for the key

          int newKey = keys[n->getNumEl() - 1];
          while (!parentNode->hasKey(key)) {
            parentNode = parentNode->getParent();
          }
          for (posInPar = 0; posInPar < parentNode->getNumEl() &&
                             newKey > parentNode->getKeys()[posInPar];
               posInPar++)
            ;
          updateParentIndx(parentNode, posInPar, newKey);
        }
      }
      currNode = nullptr;
    } else {
      // Deletion breaks the rules
      Node *rightSibling = getRightSibling(parentNode, posInPar);
      Node *leftSibling = getLeftSibling(parentNode, posInPar);

      if (currNode->leaf()) {
        // Borrow from Siblings
        if (rightSibling &&
            rightSibling->getNumEl() - 1 >= currNode->getDeg() / 2) {
          borrowLeafRight(parentNode, currNode, rightSibling, posInPar);
          currNode = nullptr;
        } else if (leftSibling &&
                   leftSibling->getNumEl() - 1 >= currNode->getDeg() / 2) {
          borrowLeafLeft(parentNode, leftSibling, currNode, posInPar);
          currNode = nullptr;
        } else {

          if (rightSibling) {
            currNode = mergeNodes(parentNode, currNode, rightSibling);
          } else {
            currNode = mergeNodes(parentNode, leftSibling, currNode);
          }
        }
      } else {
        // Borrow from Siblings
        if (rightSibling &&
            rightSibling->getNumEl() - 1 >= currNode->getDeg() / 2) {
          borrowIntRight(parentNode, currNode, rightSibling);
        } else if (leftSibling &&
                   leftSibling->getNumEl() - 1 >= currNode->getDeg() / 2) {
          borrowIntLeft(parentNode, leftSibling, currNode);
        } else {
          if (rightSibling) {
            currNode = mergeIntNodes(parentNode, currNode, rightSibling);
          } else {
            currNode = mergeIntNodes(parentNode, leftSibling, currNode);
          }
        }
      }
    }
    if (currNode) {
      parentNode = currNode->getParent();
      if (parentNode) {
        key = currNode->getKeys()[currNode->getNumEl()];
        for (posInPar = 0; posInPar < parentNode->getNumEl() &&
                           key > parentNode->getKeys()[posInPar];
             posInPar++)
          ;
      } else {
        *tRoot = static_cast<Node *>(currNode->getElArr()[0]);
        delete currNode;
        break;
      }
    }
  }
  return n;
}

Node *initTree(int key, Element *el, int deg) {

  Node *root = new Node(deg);
  void **rEltr = root->getElArr();
  int *rKeys = root->getKeys();
  rEltr[0] = el;
  rKeys[0] = key;
  root->incNum();
  return root;
}

void markAll(Node *tRoot, Database *db, int key) {

  Node *currNode = tRoot;
  Element *el = nullptr;
  int startIndx = 0;
  Element **MovieArr = db->getMovies();
  bool good = true;

  currNode = findNode(tRoot, key);
  void **currElArr = currNode->getElArr();
  good = true;

  // Find starting index
  for (int i = 0; i < currNode->getNumEl() && good; i++) {
    el = static_cast<Element *>(currElArr[i]);
    if (el->getYM() == key) {
      startIndx = el->getIndx();
      good = false;
    }

    el = MovieArr[startIndx];

    while (el->getYM() == key && el->isValid()) {
      el->notValid();
      el = MovieArr[++startIndx];
    }
  }
}

class bpTree {
  int deg;
  Node *root;
  Database *db;

public:
  bpTree(int d = 3) : deg(d), root(nullptr), db(nullptr) {}

  bpTree &setDb(Database *d) {
    db = d;
    return *this;
  }

  Node *getRoot() const { return root; }

  bool insert(int key) {

    if (root) {
      Node *temp = findNode(root, key);

      if (temp->hasKey(key)) {
        return false;
      }

      Element *elPtr = db->findKey(key);

      if (temp->isFull(deg)) {
        temp = splitLeafNode(key, &root, elPtr, temp);
      } else {
        temp = addToLeaf(key, elPtr, temp);
      }
    } else {
      root = new Node(deg);
      Element *elPtr = db->findKey(key);
      root = initTree(key, elPtr, deg);
    }
    return true;
  }

  void updateEl(int key, Element *newEl) {
    Node *temp = findNode(root, key);
    void **elArr = temp->getElArr();
    int pos;
    for (pos = 0; pos < temp->getNumEl() && key > temp->getKeys()[pos]; pos++)
      ;
    elArr[pos] = newEl;
  }

  bool remove(string name) {

    if (root) {

      int exKey = db->findExKey(name);
      int exKeyYM = exKey / 100;

      Node *temp = findNode(root, exKeyYM);

      // Mark all entries with YYYYMM as invalid in DB
      markAll(root, db, exKeyYM);

      // Delete YYYYMM key from the B+ tree
      deleteFromLeaf(exKeyYM, temp, &root);
    } else {
      cout << "\nTree is empty!" << endl;
      return false;
    }
  }

  bool findKey(int key) {
    Node *temp = findNode(root, key);
    if (temp->hasKey(key)) {
      return true;
    } else {
      return false;
    }
  }

  void debugPrint() const {
    Node *temp = root;
    queue<Node *> Q;
    void **tempArr;

    Q.push(temp);
    while (!Q.empty()) {
      temp = Q.front();
      Q.pop();
      temp->print();
      cout << " Parent is: ";
      if (temp->getParent()) {
        temp->getParent()->print();
      } else {
        cout << "null";
      }
      if (!temp->leaf()) {
        tempArr = temp->getElArr();
        for (int i = 0; i <= temp->getNumEl(); i++) {
          Q.push(static_cast<Node *>(tempArr[i]));
          cout << "\nAdding to queue for ";
          temp->print();
          cout << " : ";
          static_cast<Node *>(tempArr[i])->print();
        }
        cout << endl;
      }
    }
  }

  void print() const {
    Node *temp = root;
    queue<Node *> Q;
    void **tempArr;
    int currLvl = 1;
    int nextLvl = 0;

    Q.push(temp);
    while (!Q.empty()) {
      temp = Q.front();
      Q.pop();
      currLvl--;
      temp->print();

      if (!temp->leaf()) {
        tempArr = temp->getElArr();
        for (int i = 0; i <= temp->getNumEl(); i++) {
          Q.push(static_cast<Node *>(tempArr[i]));
          nextLvl++;
        }
      }

      if (!currLvl) {
        cout << endl;
        currLvl = nextLvl;
        nextLvl = 0;
      }
    }
  }

  void printToFile(string fname) const {
    ofstream wrFile;
    wrFile.open(fname);

    Node *temp = root;
    queue<Node *> Q;
    void **tempArr;
    int currLvl = 1;
    int nextLvl = 0;

    Q.push(temp);
    while (!Q.empty()) {
      temp = Q.front();
      Q.pop();
      currLvl--;
      temp->printToFile(wrFile);

      if (!temp->leaf()) {
        tempArr = temp->getElArr();
        for (int i = 0; i <= temp->getNumEl(); i++) {
          Q.push(static_cast<Node *>(tempArr[i]));
          nextLvl++;
        }
      }

      if (!currLvl) {
        cout << endl;
        currLvl = nextLvl;
        nextLvl = 0;
      }
    }
  }

  void destroyTree() {
    Node *temp = root;
    queue<Node *> Q;
    void **tempArr;

    Q.push(temp);
    while (!Q.empty()) {
      temp = Q.front();
      Q.pop();

      if (!temp->leaf()) {
        tempArr = temp->getElArr();
        for (int i = 0; i <= temp->getNumEl(); i++) {
          Q.push(static_cast<Node *>(tempArr[i]));
        }
      }
      delete temp;
    }
    root = nullptr;
  }

  ~bpTree() { destroyTree(); }
};

void findInRange(Node *startNode, int sYM, int eYM) {

  Node *currNode = startNode;
  void **currElArr = currNode->getElArr();
  int i = 0;

  bool good = true;

  while (currNode && good) {
    for (int i = 0; i < currNode->getNumEl(); i++) {
      if (static_cast<Element *>(currElArr[i])->getYM() >= sYM &&
          static_cast<Element *>(currElArr[i])->getYM() <= eYM &&
          static_cast<Element *>(currElArr[i])->isValid()) {
        static_cast<Element *>(currElArr[i])->printEl();
      }

      // Check to see if out-of-bounds
      if (static_cast<Element *>(currElArr[i])->getYM() > eYM) {
        good = false;
        break;
      }
    }
    currNode = currNode->getNext();
    if (currNode) {
      currElArr = currNode->getElArr();
    }
  }
}

int *movieStats(int *arr, int *&resArr, int num, Node *tRoot, Database *db) {

  Node *currNode = tRoot;
  Element *el = nullptr;
  void **currElArr = currNode->getElArr();
  int concKey;
  int startIndx = 0;
  Element **MovieArr = db->getMovies();
  bool good = true;

  for (int i = 0; i < num; i++) {
    resArr[i] = 0;
  }

  for (int i = 0; i < num; i++) {
    concKey = arr[i] * 100 + 1;
    currNode = findNode(tRoot, concKey);
    currElArr = currNode->getElArr();
    good = true;

    // Find starting index
    for (int j = 0; j < currNode->getNumEl() && good; j++) {
      el = static_cast<Element *>(currElArr[j]);
      if (el->getYM() / 100 == arr[i]) {
        startIndx = el->getIndx();
        good = false;
      }

      el = MovieArr[startIndx];

      while (el->getYM() / 100 == arr[i] && el->isValid()) {
        resArr[i]++;
        el = MovieArr[++startIndx];
      }
    }
  }
  return resArr;
}

void printLeaves(Node *tRoot) {
  Node *temp = tRoot;

  while (!temp->leaf()) {
    temp = static_cast<Node *>(temp->getElArr()[0]);
  }

  while (temp) {
    temp->print();
    cout << " EL: ";
    for (int i = 0; i < temp->getNumEl(); i++) {
      static_cast<Element *>(temp->getElArr()[i])->printEl();
    }
    cout << endl;
    temp = temp->getNext();
  }
}

void menu() {
  cout << endl;
  cout << "\n  * * * * * * * * * * * * * * * * * * *" << endl;
  cout << "  *            Main Menu	      *" << endl;
  cout << "  * * * * * * * * * * * * * * * * * * *" << endl;
  cout << "  *                                   *" << endl;
  cout << "  * 1. Create DB                      *" << endl;
  cout << "  * 2. Delete Entry (Mark)            *" << endl;
  cout << "  * 3. Delete DB                      *" << endl;
  cout << "  * 4. Print DB                       *" << endl;
  cout << "  * 5. Create B+ Tree                 *" << endl;
  cout << "  * 6. Add Entry to B+ Tree           *" << endl;
  cout << "  * 7. Delete Entry from B+ Tree      *" << endl;
  cout << "  * 8. Find Entry in Tree             *" << endl;
  cout << "  * 9. Print B+ Tree                  *" << endl;
  cout << "  * 10. Find Entries in Range         *" << endl;
  cout << "  * 11. Statistics                    *" << endl;
  cout << "  * 12. Print Leaves                  *" << endl;
  cout << "  * 13. Delete B+ Tree                *" << endl;
  cout << "  * 14. Find Intersection             *" << endl;
  cout << "  * 15. Stop                          *" << endl;
  cout << "  *                                   *" << endl;
  cout << "  * * * * * * * * * * * * * * * * * * *" << endl;
  cout << endl;
}

struct RangeNode {
  Element *el;
  RangeNode *next;

  RangeNode(Element *e) {
    el = e;
    next = nullptr;
  }
};

void addToRange(RangeNode **frst, RangeNode **lst, Element *e) {
  if (!*frst) {
    *frst = new RangeNode(e);
    *lst = *frst;
  } else {
    RangeNode *temp = new RangeNode(e);
    (*lst)->next = temp;
    *lst = temp;
  }
}

void destroyRange(RangeNode *root) {
  RangeNode *temp = root;

  while (temp) {
    root = temp->next;

    delete temp->el;
    delete temp;

    temp = root;
  }
}

void findInRangeMOD(Node *startNode, int sYM, int eYM, RangeNode **rt,
                    RangeNode **lst) {

  Node *currNode = startNode;
  void **currElArr = currNode->getElArr();
  int i = 0;

  bool good = true;

  while (currNode && good) {
    for (int i = 0; i < currNode->getNumEl(); i++) {
      if (static_cast<Element *>(currElArr[i])->getYM() >= sYM &&
          static_cast<Element *>(currElArr[i])->getYM() <= eYM &&
          static_cast<Element *>(currElArr[i])->isValid()) {
        addToRange(rt, lst, static_cast<Element *>(currElArr[i]));
      }

      // Check to see if out-of-bounds
      if (static_cast<Element *>(currElArr[i])->getYM() > eYM) {
        good = false;
        break;
      }
    }
    currNode = currNode->getNext();
    if (currNode) {
      currElArr = currNode->getElArr();
    }
  }
}

void intersection(RangeNode *startNode, RangeNode *endNode) {

  while (startNode->el->getYM() != endNode->el->getYM() && startNode) {
    startNode->el->printEl();
    startNode = startNode->next;
  }
}

int main() {

  int odg;
  bpTree *tr = nullptr;
  Database *db = nullptr;

  RangeNode *r1 = nullptr;
  RangeNode *r2 = nullptr;
  RangeNode *l1 = nullptr;
  RangeNode *l2 = nullptr;

  string name;
  string delName;

  while (true) {
    menu();
    cout << "> ";
    cin >> odg;

    switch (odg) {
    case 1:
      int inp;
      cout << "\n1. Read from file";
      cout << "\n2. Manual input\n";
      cout << "> ";
      cin >> inp;
      if (inp == 1) {
        db = inputDB(1);
      } else {
        db = inputDB(0);
      }
      break;
    case 2:
      int key;
      if (db) {
        cout << "\nEnter Movie Name: ";
        cout << "> ";
        cin.ignore(1000, '\n');
        getline(cin, name);
        key = db->findExKey(name);
        Element *oldEl = db->mark(name, key);
        cout << "\nMovie marked";
        if (oldEl) {
          Element *newEl = db->findKey(oldEl->getYM());
          if (newEl) {
            // Update the pointer so it points to the next El with YYYYMM
            tr->updateEl(newEl->getYM(), newEl);
          } else if (tr) {
            // No next element with key YYYYMM exists, delete the key from the
            // tree
            Node *temp = findNode(tr->getRoot(), oldEl->getYM());
            Node *trRoot = tr->getRoot();
            deleteFromLeaf(oldEl->getYM(), temp, &trRoot);
          }
        } else {
          cout << "Element doesn't exist";
        }
      } else {
        cout << "\nDB doesn't exist";
      }
      break;
    case 3:
      if (db) {
        db->destroyDB();
        cout << "\nDB deleted";
        db = nullptr;
      } else {
        cout << "\nDB doesn't exist";
      }
      break;
    case 4:
      if (db) {
        db->printDB();
      } else {
        cout << "\nDB doesn't exist";
      }
      break;
    case 5:
      int deg;
      if (tr) {
        cout << "\nTree already exists!";
      } else {
        if (db) {
          cout << "\nEnter B+ tree degree (max 10): ";
          cout << "> ";
          cin >> deg;

          while (deg < 1 || deg > 10) {
            cout << "\nInvalid degree";
            cout << "\nEnter B+ tree degree (max 10): ";
            cout << "> ";
            cin >> deg;
          }

          tr = new bpTree(deg);
          tr->setDb(db);
          cout << "\nB+ tree with degree " << deg << " created";
        } else {
          cout << "\nDB doesn't exist";
        }
      }
      break;
    case 6:
      if (tr) {
        int inp2;
        int movieKey;
        int movieValid;
        bool good;

        cout << "\n1. Add entire DB to B+ tree";
        cout << "\n2. Add individually\n";
        cout << "> ";
        cin >> inp2;
        while (inp2 < 1 || inp2 > 2) {
          cout << "\nInvalid choice";
          cout << "> ";
          cin >> inp2;
        }
        if (inp2 == 1) {
          for (int i = 0; i < db->getMovieNum(); i++) {
            movieKey = db->getMovies()[i]->getYM();
            movieValid = db->getMovies()[i]->isValid();

            if (movieValid) {
              if (tr->getRoot()) {
                if (tr->findKey(movieKey)) {
                  cout << "\nKey " << movieKey << " is already in the tree\n";
                } else {
                  tr->insert(movieKey);
                }
              } else {
                tr->insert(movieKey);
              }
            }
          }
        } else {
          cout << "\nEnter movie key (YYYYMM)\n";
          cout << "> ";
          cin >> movieKey;
          if (tr->findKey(movieKey)) {
            cout << "\nKey " << movieKey << " is already in the tree";
          } else {
            if (tr->insert(movieKey)) {
              cout << "\nKey " << movieKey << " added to tree";
            }
          }
        }
      } else {
        cout << "\nTree doesn't exist";
      }
      break;
    case 7:
      if (tr) {
        cout << "\nEnter movie name\n";
        cout << "> ";
        cin.ignore(1000, '\n');
        getline(cin, delName);
        if (tr->remove(delName)) {
          cout << "\nEntry deleted";
        }
      } else {
        cout << "\nTree doesn't exist";
      }
      break;
    case 8:
      int sKey;
      if (tr) {
        cout << "\nEnter movie search key (YYYYMM)\n";
        cout << "> ";
        cin >> sKey;
        if (tr->findKey(sKey)) {
          cout << "\nKey found";
        } else {
          cout << "\nKey not found";
        }
      } else {
        cout << "\nTree doesn't exist";
      }
      break;
    case 9:
      if (tr) {
        string wrFileName;
        int odg3;
        cout << endl;
        cout << "\n1. Print to CMD\n";
        cout << "\n2. Print to file\n";
        cin >> odg3;
        if (odg3 == 1) {
          tr->print();
        } else {
          cout << "Enter file name\n";
          cout << "> ";
          cin >> wrFileName;
          tr->printToFile(wrFileName);
        }
        cout << endl;
      } else {
        cout << "\nTree doesn't exist";
      }
      break;
    case 10:
      if (tr) {
        int sYM;
        int eYM;
        cout << "\nEnter range (YYYYMM YYYYMM)";
        cout << "> ";
        cin >> sYM >> eYM;
        Node *startNode = findNode(tr->getRoot(), sYM);
        findInRange(startNode, sYM, eYM);
      } else {
        cout << "\nTree doesn't exist";
      }
      break;
    case 11:
      int arrNum;
      int *yArr;
      int *resArr;
      if (tr) {
        cout << "\nEnter number of years\n";
        cout << "> ";
        cin >> arrNum;
        yArr = new int[arrNum];
        resArr = new int[arrNum];
        cout << "\nEnter the years\n";

        for (int i = 0; i < arrNum; i++) {
          cout << "> ";
          cin >> yArr[i];
        }
        resArr = movieStats(yArr, resArr, arrNum, tr->getRoot(), db);
        cout << "\nStats for the inputted years\n";
        for (int i = 0; i < arrNum; i++) {
          cout << yArr[i] << ": " << resArr[i] << endl;
        }
      } else {
        cout << "\nTree doesn't exist";
      }
      break;
    case 12:
      printLeaves(tr->getRoot());
      break;
    case 13:
      if (tr) {
        tr->destroyTree();
        tr = nullptr;
        cout << "\nB+ Tree deleted";
      } else {
        cout << "\nB+ Tree doesn't exist";
      }
      break;
    case 14:
      if (tr) {
        int sYM1;
        int eYM1;

        int sYM2;
        int eYM2;

        cout << "\nEnter first range (YYYYMM YYYYMM)";
        cout << "> ";
        cin >> sYM1 >> eYM1;

        cout << "\nEnter second range (YYYYMM YYYYMM)";
        cout << "> ";
        cin >> sYM2 >> eYM2;

        // Link first range
        Node *startNodeOne = findNode(tr->getRoot(), sYM1);
        findInRangeMOD(startNodeOne, sYM1, eYM1, &r1, &l1);

        // Link second range
        Node *startNodeTwo = findNode(tr->getRoot(), sYM2);
        findInRangeMOD(startNodeTwo, sYM2, eYM2, &r2, &l2);

        if (r1->el->getYM() > r2->el->getYM()) {
          intersection(r1, l2);
        } else {
          intersection(r2, l1);
        }

        // destroyRange(r1);
        // destroyRange(r2);
        r1 = r2 = l1 = l2 = nullptr;
      } else {
        cout << "\nTree doesn't exist";
      }
      break;
    case 15:
      return 0;
    }
  }
}