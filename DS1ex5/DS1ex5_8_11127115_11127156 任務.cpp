// 11127115 陳姿安, 11127156 王佩諭
# include <iostream>
# include <sstream>
# include <fstream>
# include <string>
# include <vector>
# include <cstdlib>
# include <iomanip>

using namespace std;

template <typename T>
class Tree {
  struct nodeType {
    T key;
    int index;
    nodeType* left;
    nodeType* right;
  };  // nodeType

  nodeType *root;

  void insertNode( nodeType* & root, T key, int index );  // insert a node
  int getHeight( nodeType* root );  // get tree height
  void clearTree( nodeType* & nodes );  // clean content of tree

  public:
    Tree();
    ~Tree();
    bool isEmpty();  // check a tree is empty or not
    int findLeftmost();  // find the most left node in tree
    int findRightmost();  // find the most right node in tree
    void add( T key, int index );  // insert a node by any type
    void showHeight();  // display tree height on screen
    void clearUp();  // clean content of tree and delete root

};  // class Tree

class PokemonList {
  struct pokemonType {
    string rawR;
    int num;
    string name;
    string type1;
    int total;
    int HP;
    int attack;
    int defense;
  };  // pokemonType

  vector<pokemonType> pSet;
  vector<pokemonType> maxHeap;
  string fileNO;
  Tree<int> HPtree;

  void show( pokemonType item );  // display the content of a item

  void heapRebuild( int root );
  void reHeapDown( int root, int bottom );

  public:
    PokemonList();
    ~PokemonList();
    bool isEmpty();  // check the content of input file is empty of not
    void readFile();  // read input file
    void showAll();  // display content, tree height, items of most left and right node
    void buildTree();  // build up a tree
    void searchTree();
    void clearUp();  // clean the content

    void buildHeap();  // build up a heap
    void heapSort();  // sort heap
    void deleteMax();  // delete the max item in heap
    void showHeap();  // show content, heap height, items of left node and bottom

};  // class PokemonList

int main() {
  string command = "";
  string fileName = "";
  bool done = false;

  PokemonList aList;

  while ( done == false ) {
    cout << endl << "***** Pokemon Tree and Heap *****";
    cout << endl << "* 0. QUIT                       *";
    cout << endl << "* 1. Read a file to build BST   *";
    cout << endl << "* 2. Transform it into Max Heap *";
    cout << endl << "*********************************";
    cout << endl << "Input a choice(0, 1, 2): ";

    cin >> command;

    if ( command == "2" && aList.isEmpty() == true ) {
      cout << endl << "----- Execute Mission 1 first! -----" << endl;
    }  // if

    else if ( command == "1" ) {
      aList.readFile();
      if ( !aList.isEmpty() ) {
        aList.buildTree();
        aList.showAll();
      }  // if
    }  // if
    else if ( command == "2" ) {
      aList.buildHeap();
      aList.showHeap();
    }  // else if
    else if ( command == "0" ) {
      done = true;
    }  // else if
    else {
      cout << endl << "Command does not exist!" << endl;
    }  // else

  }  // while

}  // main

/** class PokemonList **/
// ============================ PokemonList::PokemonList() ============================
PokemonList::PokemonList() {
  fileNO = "";
  pSet.erase( pSet.begin(), pSet.end() );
  maxHeap.erase( pSet.begin(), pSet.end() );
}  // PokemonList::PokemonList

// =========================== PokemonList::~PokemonList() ============================
PokemonList::~PokemonList() {
  clearUp();
}  // PokemonList::~PokemonList

// ============================== PokemonList::isEmpty() ==============================
bool PokemonList::isEmpty() {
  if ( pSet.empty() ) {
    return true;
  }  // if

  return false;
}  // PokemonList::isEmpty()

// ============================= PokemonList::readFile() ==============================
void PokemonList::readFile() {
  ifstream fin;
  pokemonType temp;
  string tempS = "";
  string fileName = "";
  bool success = true;

  clearUp();

  cout << endl << "Input a file number [0: quit]: ";
  cin >> fileName;

  fin.open( "input" + fileName + ".txt" );

  if ( fileName == "0" || fin.fail() ) {
    while ( fin.fail() && fileName != "0" ) {
      cout << endl << "### input" << fileName << ".txt does not exist! ###" << endl;
      cout << endl << "Input a file number [0: quit]: ";
      cin >> fileName;
      fin.open( "input" + fileName + ".txt" );
    }  // while

  }  // if

  if (fileName != "0" && !fin.fail() ) {
    getline( fin, tempS );
    istringstream line( tempS );

    // # Name Type1 HP Attack Defense
    for ( int i = 0 ; getline( line, tempS, '\t' ) ; i = i + 1 ) {
      if ( i == 0 || i == 1 || i == 6 || i == 7 ) {
        fileNO = fileNO + "\t" + tempS;
      }  // if
      else if ( i == 5 ) {
        fileNO = fileNO + "\t\t" + tempS;
      }  // else if
      else if ( i == 2 ){
        fileNO = fileNO + "\t\t\t\t" + tempS;
      }  // else

    }  // for

    for ( int j = 1 ; getline( fin, tempS ) ; j = j + 1 ) {
      istringstream line( tempS );  // istringstream -> read string from tempS
      temp.rawR = tempS;
      string item;

      for ( int k = 1 ; getline( line, item, '\t' ) ; k = k + 1 ) {
        if ( k % 13 == 1 )
          temp.num = stoi( item );
        else if ( k % 13 == 2 )
          temp.name = item;
        else if ( k % 13 == 3 )
          temp.type1 = item;
        else if ( k % 13 == 5 )
          temp.total = stoi( item );
        else if ( k % 13 == 6 )
          temp.HP = stoi( item );
        else if ( k % 13 == 7 ) {
          temp.attack = stoi( item );
        }  // else if
        else if ( k % 13 == 8 ) {
          temp.defense = stoi( item );
        }  // else if

        if ( k % 13 == 0 )
          pSet.push_back(temp);
      }  // for

    }  // for

    if ( pSet.empty() ) {
      cout << endl << "### Get nothing from the file input" << fileName << ".txt ! ###" << endl;
    }  // if

  }  // else

  maxHeap.assign(pSet.begin(), pSet.end() );
  fin.close();
}  // PokemonList::readFile()

// ================================ PokemonList::show() ===============================
void PokemonList::show( pokemonType item ) {
    cout << item.num << "\t"<< left << setw(24)
          << item.name << "\t" << left << setw(16)
          << item.type1 << left << setw(8)
          << item.HP << left << setw(8)
          << item.attack << left << setw(8)
          << item.defense << endl;

}  // PokemonList::show()

// =========================== PokemonList::showHeapHeight() ==========================
void PokemonList::showHeap() {
  int height = 1;
  int currentNum = 2;

  /* get heap height */
  for ( height = 1, currentNum = 2 ; maxHeap.size() >= currentNum ;
         height = height + 1, currentNum = currentNum * 2 ) {}  // for
  /*--------------- */

  cout << fileNO << endl;
  for ( int i = 0 ; i < maxHeap.size() ; i = i + 1 ) {
    cout << "[" << right << setw(2) << i << "]\t";
    show( maxHeap[i] );
  }  // for

  cout << "HP heap height = " << height << endl;

  cout << "Leftmost node:";
  cout << endl << fileNO << endl;
  cout << "[" << currentNum / 2 - 1  << "]\t";
  show( maxHeap[ currentNum / 2 - 1  ] );

  cout << "Bottom:";
  cout << endl << right << fileNO << endl;
  cout << "[" << maxHeap.size() - 1 << "]\t";
  show( maxHeap[ maxHeap.size() - 1 ] );
}  // PokemonList::showHeapHeight

// ============================== PokemonList::showAll() ==============================
void PokemonList::showAll() {
  cout << fileNO << endl;
  for ( int i = 0 ; i < pSet.size() ; i = i + 1 ) {
    cout << "[" << right << setw(2) << i + 1 << "]\t";
    show( pSet[i] );
  }  // for

  HPtree.showHeight();

  cout << "Leftmost node:";
  cout << endl << fileNO << endl;
  cout << "[" << right << setw(3) << HPtree.findLeftmost() << "]\t";
  show( pSet[ HPtree.findLeftmost() - 1 ] );

  cout << "Rightmost node:";
  cout << endl << right << fileNO << endl;
  cout << "[" << right << setw(3) << HPtree.findRightmost() << "]\t";
  show( pSet[ HPtree.findRightmost() - 1 ] );
}  // PokemonList::showAll

// ============================= PokemonList::buildTree() =============================
void PokemonList::buildTree() {
  for ( int i = 0 ; i < pSet.size() ; i = i + 1 ) {
    HPtree.add( pSet[i].HP, i + 1 );
  }  // for

}  // PokemonList::buildTree

// ============================= PokemonList::buildHeap() =============================
void PokemonList::buildHeap() {
  for ( int i = maxHeap.size() / 2 - 1 ; i >= 0  ; i = i - 1 ) {
    heapRebuild(i);
  }  // for

}  // PokemonList::buildHeap

// ============================= PokemonList::searchTree() ============================
void PokemonList::searchTree() {

}  // PokemonList::searchTree

// ============================ PokemonList::heapRebuild() ============================
void PokemonList::heapRebuild( int root ) {
  int child = 2 * root + 1;
  int range = maxHeap.size();
  if ( child < range ) {
    int rightChild = child + 1;
    if ( rightChild < range && maxHeap[rightChild].HP > maxHeap[child].HP ) {
      child = rightChild;
    }  // if

    if ( maxHeap[root].HP < maxHeap[child].HP ) {
      pokemonType temp = maxHeap[root];
      maxHeap[root] = maxHeap[child];
      maxHeap[child] = temp;
      heapRebuild(child);
    }  // if

  }  // if

}  // PokemonList::heapRebuild()

// ============================== PokemonList::heapSort() =============================
void PokemonList::heapSort() {
  int index;
  int range = maxHeap.size();

  for ( index = ( range - 1 ) / 2 ; index >= 0 ; index = index - 1 ) {
    reHeapDown(index, range / 2 );
  }  // for

  for ( index = range - 1 ; index >= 1 ; index = index - 1 ) {
    pokemonType tempItem = maxHeap[0];
    maxHeap[0] = maxHeap[index];
    maxHeap[index] = tempItem;
    reHeapDown( 0, index );
  }  // for

}  // PokemonList::heapSort()

// ============================== PokemonList::heapSort() =============================
void PokemonList::reHeapDown( int root, int bottom) {
  int child = 2 * root + 1;
  int range = maxHeap.size();
  if ( child < bottom ) {
    int rightChild = child + 1;
    if ( rightChild < bottom && maxHeap[rightChild].HP > maxHeap[child].HP ) {
      child = rightChild;
    }  // if

    if ( maxHeap[root].HP < maxHeap[child].HP ) {
      pokemonType temp = maxHeap[root];
      maxHeap[root] = maxHeap[child];
      maxHeap[child] = temp;
      reHeapDown(child, bottom);
    }  // if

  }  // if

}  // PokemonList::heapSort()

// ============================= PokemonList::deleteMax() =============================
void PokemonList::deleteMax() {
  if ( !maxHeap.empty() ) {
    maxHeap[0] = maxHeap[maxHeap.size() - 1];
    heapRebuild(0);
  }  // if
}  // PokemonList::deleteMax

// ============================== PokemonList::clearUp() ==============================
void PokemonList::clearUp() {
  HPtree.clearUp();
  fileNO = "";
  pSet.erase( pSet.begin(), pSet.end() );
  maxHeap.erase( pSet.begin(), pSet.end() );
}  // PokemonList::clearUp

/** class Tree **/
// =================================== Tree::Tree() ===================================
template <typename T>
Tree<T>::Tree() {
  root = NULL;
}  // Tree::Tree

// ================================== Tree::~Tree() ===================================
template <typename T>
Tree<T>::~Tree() {
  clearUp();
}  // Tree::~Tree

// =============================== Tree::insertNode() =================================
template <typename T>
void Tree<T>::insertNode( nodeType* & root, T key, int index ) {
  root = new nodeType;
  root -> key = key;
  root -> index = index;
  root -> left = NULL;
  root -> right = NULL;
}  // Tree::insertNode

// ================================ Tree::getHeight() =================================
template <typename T>
int Tree<T>::getHeight( nodeType* root ) {
  nodeType* temp = root;

  if ( root == NULL )
    return 0;
  else {
    int left = getHeight( temp -> left );
    int right = getHeight( temp -> right );

    if ( left > right ) {
      return 1 + left;
    }  // if
    else {
      return 1 + right;
    }  // else

  }  // else

}  // Tree::getHeight

// ================================ Tree::clearTree() =================================
template <typename T>
void Tree<T>::clearTree( nodeType* & nodes ) {
  if ( nodes == NULL ) {
    return;
  }  // if

  if ( nodes -> left != NULL ) {
    clearTree( nodes -> left );
  }  // else

  if ( nodes -> right != NULL ) {
    clearTree( nodes -> right );
  }  // if

  delete nodes;
  nodes = NULL;
}  // Tree::clearTree

// ================================= Tree::isEmpty() ==================================
template <typename T>
bool Tree<T>::isEmpty() {
  if ( root == NULL ) {
    return true;
  }  // if

  return false;
}  // Tree::isEmpty

// ============================== Tree::findLeftmost() ================================
template <typename T>
int Tree<T>::findLeftmost() {
  nodeType* temp = root;

  while ( temp -> left != NULL ) {
    temp = temp -> left;
  }  // while

  return temp -> index;
}  // Tree::findLeftmost

// =============================== Tree::findLeftmost() ===============================
template <typename T>
int Tree<T>::findRightmost() {
  nodeType* temp = root;

  while ( temp -> right != NULL ) {
    temp = temp -> right;
  }  // while

  return temp -> index;
}  // Tree::findRightmost

// =================================== Tree::add() ====================================
template <typename T>
void Tree<T>::add( T key, int index ) {
  if ( root == NULL ) {
    insertNode( root, key, index );
  }  // if
  else {
    bool done = false;
    nodeType* temp = root;

    do {
      if ( key >= ( temp -> key ) ) {
        if ( temp -> right == NULL ) {
          insertNode( temp -> right, key, index );
          done = true;
        }  // if
        else {
          temp = temp -> right;
        }  // else

      }  // if
      else if ( key < ( temp -> key ) ) {
        if ( temp -> left == NULL ) {
          insertNode( temp -> left, key, index );
          done = true;
        }  // if
        else {
          temp = temp -> left;
        }  // else

      }  // else if

    } while ( done == false );

  }  // else
}  // Tree::add

// =============================== Tree::showHeight() =================================
template <typename T>
void Tree<T>::showHeight() {
  cout << "HP tree height = " << getHeight(root) << endl;
}  // Tree::showHeight

// ================================= Tree::clearUp() ==================================
template <typename T>
void Tree<T>::clearUp() {
  clearTree( root );
  delete root;
  root = NULL;
}  // Tree::clearUp
