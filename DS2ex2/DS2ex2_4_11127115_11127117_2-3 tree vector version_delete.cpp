// 11127115 ³¯«º¦w
# include <iostream>
# include <fstream>    // ifstream
# include <sstream>    // istringstream
# include <algorithm>  // swap
# include <vector>
# include <string>
# include <iomanip>

using namespace std;

struct schoolType{
  string schoolName = "";
  string departmentName = "";
  string type = "";
  string level = "";
  int numOfStudent = 0;
  int numOfProfessor = 0;
  int numOfGraduate = 0;
};  // schoolType

class Tree {
  protected:
    struct node {
      vector<int> id;
      int key;
    };  // treeNode

    vector<schoolType> data;

    // show whole data content
    void showData();

    // remove non-numeric characters from string
    void cleanString( string & input );

  public:
    // read data from file
    bool readFile();

    // check there are data in vector or not
    bool noData();

    // get whole data
    vector<schoolType> getData();
};  // class Tree

class TwoThreeTree: public Tree {
  private:
    struct treeNode {
      vector<node> item;
      treeNode * previous;
      vector<treeNode*> child;

      treeNode(): previous(nullptr) {
        fill( child.begin(), child.end(), nullptr);
      };

    };  // tree

    treeNode * root;

    // clear up whole tree
    void clearUp( treeNode * & node );

    void findNode( treeNode * & temp, node input );

    // insert node into 2-3 tree
    void insertNode( node item );

    // split illegal node to legal
    void splitNode( treeNode * & node );

    // get tree height
    int getHeight( treeNode * root );

    // get number of nodes
    int getNumOfNode( treeNode * root, int numOfNode );

    // show all of item of tree
    void showAll( treeNode * node );

    void getSuccessor( int index, treeNode * root, treeNode * & successor );

    void fix( treeNode * & temp );

    void redistribute( treeNode * & borrowPtr, treeNode * & lendPtr );

    void mergeNode( treeNode * & borrowPtr );


  public:
    TwoThreeTree();
    ~TwoThreeTree();
    void build23tree();
    bool treeIsEmpty();
    void deleteNode();
    void show();
};  // TwoThreeTree

class AVLTree: public Tree {
  private:
    struct treeNode {
      int BF;
      node item;
      treeNode * left;
      treeNode * right;

      treeNode( int id, int student):BF(0), left(nullptr), right(nullptr) {
        item.id.push_back(id);
        item.key = student;
      }  // treeNode

    };  // treeNode

    treeNode * root;

    // clean up whole tree
    void clearUp( treeNode * & node );

    // insert node into AVL tree
    void insertNode( treeNode * & root, node item );

    // get tree height
    int getHeight( treeNode * node );

    // get number of node
    int getNumOfNode( treeNode * node, int numOfNode );

    // show all of item in tree
    void showAll( treeNode * node );

    void rotateNode( treeNode * & node );
    void RRrotate( treeNode * & node );
    void LLrotate( treeNode * & node );
    void RLrotate( treeNode * & node );
    void LRrotate( treeNode * & node );

  public:
    AVLTree();
    ~AVLTree();
    void buildAVLTree( vector<schoolType> inData );
    void show();
};  // AVLTree

int main() {
  string command = "";
  bool done = false;
  TwoThreeTree twoThreeTree;
  AVLTree avlTree;

  while ( done == false ) {
    cout << endl << "**** Balanced Search Trees ****";
    cout << endl << "* 0. QUIT                     *";
    cout << endl << "* 1. Build 23 tree            *";
    cout << endl << "* 2. Build AVL tree           *";
    cout << endl << "*******************************";
    cout << endl << "Input a choice(0, 1, 2): ";
    cin >> command;

    if ( command == "2" && twoThreeTree.noData() == true ) {
      cout << "### Choose 1 first. ###" << endl;
    }  // if

    if ( command == "1" ) {
      if ( twoThreeTree.readFile() ) {
        twoThreeTree.build23tree();
        twoThreeTree.show();
      }  // if

    }  // if
    else if ( command == "2" ) {
      if ( twoThreeTree.noData() == false ) {
        avlTree.buildAVLTree( twoThreeTree.getData() );
        avlTree.show();
      }  // if

    }  // else if
    else if ( command == "3" ) {
      if ( twoThreeTree.treeIsEmpty() == false ) {
        twoThreeTree.deleteNode();
      }  // if

    }  // else if
    else if ( command == "0" ) {
      done = true;
    }  // else if
    else {
      cout << endl << "Command does not exist!" << endl;
    }  // else

  }  // while

  return 0;
}  // main

/** class Tree **/
/*=============================== Tree::readFile() ================================*/
bool Tree::readFile() {
  ifstream fin;
  string fileName;
  string temp;
  schoolType tempSchool;
  bool success = false;

  data.erase(data.begin(), data.end() );

  cout << endl << "Input a file number ([0] Quit): ";
  cin >> fileName;

  fin.open( "input" + fileName + ".txt" );

  // check the file is exist or stop input fileName
  while ( fin.fail() && fileName != "0" ) {
    cout << endl << "### input" + fileName + ".txt does not exist! ###" << endl;
    cout << endl << "Input a file number ([0] Quit): ";
    cin >> fileName;
    fin.open( "input" + fileName + ".txt" );
  }  // while

  if ( !fin.fail() ) {
    // remove first three lines from file
    for ( int i = 0 ; i < 3 ; i = i + 1 ) {
      getline( fin, temp );
    }  // for

    while ( getline( fin, temp ) ) {
      success = true;

      istringstream line( temp );  // istringstream -> read string from tempS
      string item;

      for ( int k = 1 ; getline( line, item, '\t' ) ; k = k + 1 ) {

        if ( k % 11 == 2 )
          tempSchool.schoolName = item;
        else if ( k % 11 == 4 )
          tempSchool.departmentName = item;
        else if ( k % 11 == 5 )
          tempSchool.type = item;
        else if ( k % 11 == 6 )
          tempSchool.level = item;
        else if ( k % 11 == 7 && !item.empty() ){
          cleanString( item );
          tempSchool.numOfStudent = stoi( item );
        }  // else if
        else if ( k % 11 == 8 && !item.empty() ) {
          cleanString( item );
          tempSchool.numOfProfessor = stoi( item );
        }  // else if
        else if ( k % 11 == 9 && !item.empty() ) {
          cleanString( item );
          tempSchool.numOfGraduate = stoi( item );
        }  // else if

        if ( k % 11 == 0 )
          data.push_back(tempSchool);
        }  // for

    }  // while

  }  // if

  fin.close();
  return success;
}  // Tree::readFile()

/*============================== Tree::cleanString() ==============================*/
void Tree::cleanString( string & input ) {
  string temp = input;
  int index = 0;

  while ( index < temp.size() ) {
    if ( (int)(temp[index]) > 57 || (int)(temp[index]) < 48 ) {
      temp.erase( index, 1 );
    }  // if
    else {
      index = index + 1;
    }  // else

  }  // while

  input = temp;
}  // Tree::clearString()

/*================================= Tree::getData() ===============================*/
vector<schoolType> Tree::getData() {
  return data;
}  // Tree::getData()

/*================================ Tree::showData() ===============================*/
void Tree::showData() {
  for ( int j = 0 ; j < data.size() ; j = j + 1  ) {
    cout << data[j].schoolName << "\t"
          << data[j].departmentName << "\t"
          << data[j].type << "\t"
          << data[j].level << "\t"
          << data[j].numOfStudent << "\t"
          << data[j].numOfProfessor << "\t"
          << data[j].numOfGraduate << endl;
  }  // for

}  // Tree::showData()

/*================================ Tree::isEmpty() ================================*/
bool Tree::noData() {
  return data.empty();
}  // Tree::isEmpty()

/** class TwoThreeTree **/
/*========================= TwoThreeTree::TwoThreeTree() ==========================*/
TwoThreeTree::TwoThreeTree() {
  root = NULL;
}  // TwoThreeTree::TwoThreeTree()

/*======================== TwoThreeTree::~TwoThreeTree() ==========================*/
TwoThreeTree::~TwoThreeTree() {
  clearUp(root);
  root = NULL;
}  // TwoThreeTree::TwoThreeTree()

/*=========================== TwoThreeTree::clearUp() =============================*/
void TwoThreeTree::clearUp( treeNode * & node ) {
  if ( node == NULL ) {
    return;
  }  // if

  for ( int i = 0 ; i < node -> child.size() ; i = i + 1 ) {
    if ( node -> child[i] != NULL )
      clearUp( node -> child[i] );
  }  // for

  delete node;
  node = NULL;
}  // TwoThreeTree::clearUp()

/*========================= TwoThreeTree::build23tree() ===========================*/
void TwoThreeTree::build23tree() {
  node temp;
  clearUp(root);

  temp.id.push_back(0);
  for ( int i = 0 ; i < data.size() ; i = i + 1 ) {
    temp.id[0] = i + 1;
    temp.key = data[i].numOfGraduate;
    insertNode( temp );
  }  // for

  showAll(root);
}  // TwoThreeTree::build23tree()

/*=========================== TwoThreeTree::findNode() ============================*/
void TwoThreeTree::findNode( treeNode * & temp, node input ) {
  bool done = false;
  treeNode * next = root;

  while ( next != NULL && done == false ) {
    // check the number of school graduate is equal to the node of 2-3 tree or not
    for ( int i = 0 ; i < ( temp -> item.size() ) ; i = i + 1 ) {
      if ( input.key == temp -> item[i].key ) {
        done = true;
      }  // if

    }  // for

    if ( !temp -> child.empty() ) {
      // the value of insert item is smaller than the value of current smallest item
      if ( input.key < ( temp -> item.front().key ) ) {
        next = next -> child.front();
      }  // if

      // the value of insert item is bigger than the value of current largest item
      else if ( input.key > ( temp -> item.back().key ) ) {
        next = next -> child.back();
      }  // if

      // the value of insert item is between largest and smallest value of current node
      else if ( input.key > ( temp -> item.front().key ) &&
                 input.key < ( temp -> item.back().key ) ) {
        next = next -> child[1];
      }  // else if

      temp = next;
    } // if
    else {
      next = NULL;
    }  // else

  }  // while

}  // findNode

/*========================== TwoThreeTree::insertNode() ===========================*/
void TwoThreeTree::insertNode( node input ) {
  if ( root == NULL ) {
    root = new treeNode;
    root -> item.push_back(input);
  }  // if
  else {
    bool equalGraduate = false;
    treeNode * temp = root;

    findNode( temp, input );

    // check the number of school graduate is equal to the node of 2-3 tree or not
    for ( int i = 0 ; i < ( temp -> item.size() ) ; i = i + 1 ) {
      if ( input.key == temp -> item[i].key ) {
        temp ->item[i].id.push_back( input.id[0] );
        equalGraduate = true;
      }  // if

    }  // for

      if ( equalGraduate == false ) {
        temp -> item.push_back(input);

        // sort item in node from small to large
        for ( int i = 0 ; i < temp -> item.size() ; i = i + 1 ) {
          for ( int j = i + 1 ; j < temp -> item.size() ; j = j + 1 ) {
            if ( temp -> item[i].key > temp -> item[j].key )
              swap( temp -> item[i], temp -> item[j] );

          }  // for

        }  // for

      }  // if

    if ( ( temp -> item.size() ) > 2 ) {
      splitNode( temp );
    }  // if

  }  // else

}  // TwoThreeTree::insertNode()

/*=========================== TwoThreeTree::splitNode() ===========================*/
void TwoThreeTree::splitNode( treeNode * & node ) {
  treeNode * parent = node -> previous;
  treeNode * node2 = new treeNode;  // node2 is the largest item split from node

  node2 -> item.push_back( node -> item.back() );

  if ( !node -> child.empty() ) {
    node2 -> child.push_back( node -> child[ node -> child.size() - 2] );
    node -> child[ node -> child.size() - 2] -> previous = node2;

    node2 -> child.push_back( node -> child.back() );
    node -> child.back() -> previous = node2;
  }  // if

  // tree grow up
  if ( parent == NULL ) {
    parent = new treeNode;
    root = parent;

    parent -> item.push_back( node -> item[1]);

    parent -> child.push_back( node );
    parent -> child.push_back( node2 );
  }  // if
  else {
    // item up from right child (larger)
    if ( node -> item[1].key > parent -> item.back().key ) {
      parent -> item.push_back( node -> item[1] );

      parent -> child.push_back( node2 );
    }  // if

    // item up from left child (smaller)
    else  if ( node -> item[1].key < parent -> item.front().key ) {
      parent -> item.insert( parent -> item.begin(), node -> item[1] );

      parent -> child.insert( parent -> child.begin() + 1, node2 );
    }  // else if

    // item up from middle child => definitely is 3-node
    else {
      parent -> item.insert( parent -> item.begin() + 1, node -> item[1] );

      parent -> child.insert( parent -> child.begin() + 2, node2 );
    }  // else

  }  // else

  // update node content
  node -> item.erase( node -> item.begin() + 1, node ->item.end() );
  if ( !node -> child.empty() )
    node -> child.erase( node -> child.end() - 2, node -> child.end() );

  // set parent of split nodes
  node -> previous = parent;
  node2 -> previous = parent;

  if ( ( parent -> item.size() ) > 2 ) {
    splitNode(parent);
  }  // if

}  // TwoThreeTree::spiltNode()

/*============================ TwoThreeTree::showAll() ============================*/
void TwoThreeTree::showAll( treeNode * node ) {
  treeNode * temp = node;

  if ( node != NULL ) {
    cout << "item num: " << node -> item.size() << endl;
    for ( int j = 0 ; j < (node -> item.size() ) ; j = j + 1 ) {
      cout << j + 1 << "-total id num: " << node -> item[j].id.size() << endl;

      for ( int i = 0 ; i < ( node -> item[j].id.size() ) ; i = i + 1 ) {
        cout << "[" << node -> item[j].id[i] << "] \t" << ( node -> item[j].key ) << endl;
      }  // for

    }  // for
    cout << "---------------------------------------" << endl;

    for ( int i = 0 ; i < node -> child.size() ; i = i + 1 ) {
      if ( node -> child[i] != NULL )
        showAll( node -> child[i] );
    }  // for

  }  // if


}  // TwoThreeTree::showData()

/*============================== TwoThreeTree::show() =============================*/
void TwoThreeTree::show() {
  int num = 0;

  cout << "Tree height = " << getHeight(root) << endl;
  cout << "Number of nodes = " << getNumOfNode( root, 0 ) << endl;

  for ( int i = 0 ; i < root -> item.size() ; i = i + 1 ) {
    for ( int j = 0 ; j < root -> item[i].id.size() ; j = j + 1 ) {
      int id = root -> item[i].id[j] - 1;

      num = num + 1;
      cout << num << ": "
            << "[" << id + 1 << "] "
            << data[id].schoolName << ", "
            << data[id].departmentName << ", "
            << data[id].type << ", "
            << data[id].level << ", "
            << data[id].numOfStudent << ", "
            << data[id].numOfGraduate << endl;
    }  // for

  }  // for

  cout << endl;
}  // TwoThreeTree::show()

/*============================ TwoThreeTree::getHeight() ==========================*/
int TwoThreeTree::getHeight( treeNode * root ) {
  treeNode * temp = root;
  int height = 1;

  while ( !temp -> child.empty() ) {
    temp = temp -> child.front();
    height = height + 1;
  }  // while

  return height;
}  // TwoThreeTree::getHeight()

/*========================== TwoThreeTree::getNumOfNode() =========================*/
int TwoThreeTree::getNumOfNode( treeNode * root, int numOfNode ) {
  treeNode * temp = root;

  numOfNode = numOfNode + 1;

  for ( int i = 0 ; i < temp -> child.size() ; i = i + 1 ) {
    if ( temp -> child[i] != NULL )
      numOfNode = getNumOfNode( temp -> child[i], numOfNode );
  }  // for

  return numOfNode;
}  // TwoThreeTree::getNumOfNode()

/*=========================== TwoThreeTree::treeIsEmpty() =========================*/
bool TwoThreeTree::treeIsEmpty() {
  return root == NULL;
}  // TwoThreeTree::treeIsEmpty()

/*=========================== TwoThreeTree::deleteNode() ==========================*/
void TwoThreeTree::deleteNode() {
  bool equalKey = false;
  int index = 0;
  int needDeleteNum = 0;
  treeNode * temp = root;
  node tempNode;

  cout << endl << "input delete school graduate student number: ";
  cin >> needDeleteNum;

  tempNode.id.push_back( -1 );
  tempNode.key = needDeleteNum;

  findNode( temp, tempNode );

  // check the number of school graduate is equal to the node of 2-3 tree or not
  while ( index < temp -> item.size() && equalKey == false ) {
    if ( needDeleteNum == temp -> item[index].key ) {
      equalKey = true;
    }  // if
    else {
      index = index + 1;
    }  // else

  }  // while

  if ( equalKey == true ) {
    // if current node is not leaf -> find in-order successor
    if ( !temp -> child.empty() ) {
       treeNode * successor;
       getSuccessor( index, temp, successor );
       swap(temp -> item[index], successor -> item[0] );

       temp = successor;
       index = 0;
    }  // if

    // delete item from node
    temp -> item.erase( temp -> item.begin() + index );

    if ( temp -> item.empty() ) {
      if ( temp != root ) {
        fix( temp );
      }
      else
        root = NULL;
    }  // if

    showAll(root);
  }  // if
  else {
    cout << "no such node!" << endl;
  }  //

}  // TwoThreeTree::deleteNode()

/*========================== TwoThreeTree::getSuccessor() =========================*/
void TwoThreeTree::getSuccessor( int index, treeNode * root, treeNode * & successor ) {
  if ( root -> item.size() == 2 && index == 0 ) {
    successor = root -> child[1];
  }  // if
  else {
    successor = root -> child.back();
  }  // else

  while ( successor -> child.empty() == false ) {
    successor = successor -> child.front();
  }  // while

}  // TwoThreeTree::getSuccessor()

/*============================== TwoThreeTree::fix() ==============================*/
void TwoThreeTree::fix( treeNode * & temp ) {
  treeNode * parent = temp -> previous;
  treeNode * borrowPtr = NULL;
  treeNode * lendPtr = NULL;

  if ( temp == root ) {
    if ( temp -> child.empty() == false ) {
      root = temp -> child.front();
      root -> previous = NULL;
    }  // if
    else {
      delete temp;
      root - NULL;
    }  // else

    temp = NULL;
  }  // if

  else if ( parent -> child.front() == temp ) {
    cout << " left" << endl;
    borrowPtr = parent -> child.front();

    // parent is 3-node
    if ( parent -> item.size() == 2 ) {
      if ( parent -> child[1] -> item.size() == 2 )
        lendPtr = parent -> child[1];

    }  // if

    // parent is 2-node
    else {
      if ( parent -> child.back() -> item.size() == 2 )
        lendPtr = parent -> child.back();

    }  // else

  }  // else if

  else if ( parent -> child.back() == temp ) {
    cout << " right " << endl;
    borrowPtr = parent -> child.back();

    // parent is 3-node
    if ( parent -> item.size() == 2 ) {
      if ( parent -> child[1] -> item.size() == 2 )
        lendPtr = parent -> child[1];

    }  // if

    // parent is 2-node
    else {
      if ( parent -> child.front() -> item.size() == 2 )
        lendPtr = parent -> child.front();

    }  // else

  }  // else if

  else if ( parent -> child[1] == temp ) {
    cout << " middle " << endl;
    borrowPtr = parent -> child[1];

    if ( parent -> child.back() -> item.size() == 2 )
      lendPtr = parent -> child.back();

    if ( parent -> child.front() -> item.size() == 2 )
      lendPtr = parent -> child.front();

  }  // else if

  if ( lendPtr != NULL ) {
    cout << "==== enter redistribute ====" << endl;
    redistribute( borrowPtr, lendPtr );
    cout << "===== out redistribute =====" << endl;
  }  // if
  else if ( borrowPtr != NULL ) {
    cout << "==== enter mergeNode ====" << endl;
    mergeNode( borrowPtr );
    cout << "===== out mergeNode =====" << endl;
  }  // else if

}  // TwoThreeTree::fix()

/*========================== TwoThreeTree::redistribute() =========================*/
void TwoThreeTree::redistribute( treeNode * & borrowPtr, treeNode * & lendPtr ) {
  treeNode * parent = borrowPtr -> previous;

  // lendPtr is parent -> right
  if ( lendPtr -> item.front().key > parent -> item.back().key ) {

    if ( lendPtr -> child.empty() == false ) {
      borrowPtr -> child.push_back( lendPtr -> child.front() );
      lendPtr -> child.front() -> previous = borrowPtr;
      lendPtr ->child.erase( lendPtr -> child.begin() );
    }  // if

    borrowPtr -> item.push_back( parent -> item.back() );
    parent -> item.back() = lendPtr -> item.front();
    lendPtr -> item.erase( lendPtr -> item.begin() );
  }  // if

  // lendPtr is parent -> left
  else if ( lendPtr -> item.front().key < parent -> item.front().key ) {

    if ( lendPtr -> child.empty() == false ) {
      borrowPtr -> child.insert( borrowPtr -> child.begin(), lendPtr -> child.back() );
      lendPtr -> child.back() -> previous = borrowPtr;
      lendPtr -> child.pop_back();
    }  // if

    borrowPtr ->item.push_back( parent -> item.front() );
    parent -> item.front() = lendPtr -> item.back();
    lendPtr -> item.pop_back();
  }  // else if

  // lendPtr is parent -> middle
  else {
    if ( borrowPtr == parent -> child.front() ) {

      if ( lendPtr -> child.empty() == false ) {
        borrowPtr -> child.push_back( lendPtr -> child.front() );
        lendPtr -> child.front() -> previous = borrowPtr;
        lendPtr ->child.erase( lendPtr -> child.begin() );
      }  // if

      borrowPtr ->item.push_back( parent -> item.front() );
      parent -> item.front() = lendPtr -> item.front();
      lendPtr -> item.erase( lendPtr -> item.begin() );
    }  // if
    else if ( borrowPtr == parent -> child.back() ) {

      if ( lendPtr -> child.empty() == false ) {
        borrowPtr -> child.insert( borrowPtr -> child.begin(), lendPtr -> child.back() );
        lendPtr -> child.back() -> previous = borrowPtr;
        lendPtr -> child.pop_back();
      }  // if

      borrowPtr ->item.push_back( parent -> item.back() );
      parent -> item.back() = lendPtr -> item.back();
      lendPtr -> item.pop_back();
    }  // else if

  }  // else

}  // TwoThreeTree::redistribute()

/*============================ TwoThreeTree::mergeNode() ==========================*/
void TwoThreeTree::mergeNode( treeNode * & borrowPtr ) {
  treeNode * parent = borrowPtr -> previous;
  treeNode * child = NULL;
  treeNode * lendPtr = NULL;

  if ( borrowPtr -> child.empty() == false ) {
    cout << "have child ! " << endl;
    child = borrowPtr -> child.front();
    borrowPtr -> child.erase( borrowPtr -> child.begin() );
  }  // if

  if ( borrowPtr == root ) {
    root = child;
    root -> previous = NULL;
  }  // if
  else {
    if ( borrowPtr == parent -> child.back() ) {
      if ( parent -> child.size() == 3 ) {
        lendPtr = parent -> child[1];
      }  // if
      else {
        lendPtr = parent -> child.front();
      }  // else

      if ( child != NULL ) {
        lendPtr -> child.push_back(child);
        child -> previous = lendPtr;
      }  // if

      lendPtr -> item.push_back( parent -> item.back() );
      parent -> item.pop_back();

      parent -> child.pop_back();
    }  // if

    else if ( borrowPtr == parent -> child.front() ) {
      if ( parent -> child.size() == 3 ) {
        lendPtr = parent -> child[1];
      }  // if
      else {
        lendPtr = parent -> child.back();
      }  // else

      if ( child != NULL ) {
        lendPtr -> child.insert( lendPtr -> child.begin(), child );
        child -> previous = lendPtr;
      }  // if

      lendPtr -> item.insert( lendPtr -> item.begin(), parent -> item.front() );
      parent -> item.erase( parent -> item.begin() );

      parent -> child.erase( parent -> child.begin() );
    }  // else if

    else if ( borrowPtr == parent -> child[1] ) {
      lendPtr = parent -> child.front();
      if ( child != NULL ) {
        lendPtr -> child.push_back(child);
        child -> previous = lendPtr;
      }  // if

      lendPtr -> item.push_back( parent -> item.front() );
      parent -> item.erase( parent -> item.begin() );
      parent -> child.erase( parent -> child.begin() + 1 );
    }  // else if

    delete borrowPtr;
    borrowPtr = NULL;

    if ( parent -> child.size() == 0) {

    }  // if

    if ( parent -> item.empty() ) {
      cout << "test" << endl;
      fix( parent );
    }  // if

  }  // else

}  // TwoThreeTree::mergeNode()

/** class AVLTree **/
/*============================== AVLTree::AVLTree() ===============================*/
AVLTree::AVLTree() {
  root = NULL;
}  // AVLTree::AVLTree()

/*============================== AVLTree::~AVLTree() ==============================*/
AVLTree::~AVLTree() {
  data.erase( data.begin(), data.end() );
  clearUp( root );
  root = NULL;
}  // AVLTree::AVLTree()

/*============================== AVLTree::clearUp() ===============================*/
void AVLTree::clearUp( treeNode * & node ) {
  if ( node == NULL ) {
    return;
  }  // if

  if ( node -> left != NULL ) {
    clearUp( node -> left );
  }  // if

  if ( node -> right != NULL ) {
    clearUp( node -> right );
  }  // if

  delete node;
  node = NULL;
}  // AVLTree::clearUp()

/*============================ AVLTree::buildAVLTree() ============================*/
void AVLTree::buildAVLTree( vector<schoolType> inData ) {
  data = inData;

  // clean tree from previous data
  clearUp( root );

  node temp;
  temp.id.push_back(0);

  for ( int i = 0 ; i < data.size() ; i = i + 1 ) {
    temp.id[0] = i + 1;
    temp.key = data[i].numOfStudent;
    insertNode(root, temp);
  }  // for

}  // AVLTree::buildAVLTree()

/*============================= AVLTree::insertNode() =============================*/
void AVLTree::insertNode( treeNode * & root, node item ) {
  if ( root == NULL ) {
    root = new treeNode( item.id[0], item.key );
  }  // if
  else {
    treeNode * temp = root;

    if ( item.key > temp -> item.key ) {
      insertNode( temp -> right, item );
    }  // if
    else if ( item.key < temp -> item.key ) {
      insertNode( temp -> left, item );
    }  // else if
    else {
      temp -> item.id.push_back( item.id[0] );
    }  // else

  }  // else

  // update balance factor of current node
  root -> BF = getHeight(root -> left) - getHeight(root -> right);

  // if right and left subtree of current node are not balance -> rotate to make it balance
  while ( root -> BF == 2 || root -> BF == -2 ) {
    rotateNode( root );

    // update balance factor after rotate, make sure current node is balance
    root -> BF = getHeight(root -> left) - getHeight(root -> right);
  }  // while

}  // AVLTree::insertNode()

/*============================= AVLTree::rotateNode() =============================*/
void AVLTree::rotateNode( treeNode * & node ) {
  if ( node -> BF == 2 ) {
    if ( ( node -> left -> BF ) == 1 || ( node -> left -> BF == 0 ) ) {
      LLrotate( node );
    }  // if
    else if ( node -> left -> BF == -1 ) {
      LRrotate( node );
    }  // else if

  }  // if
  else if ( node -> BF == -2 ) {
    if ( ( node -> right -> BF ) == -1 || ( node -> right -> BF == 0 ) ) {
      RRrotate( node );
    }  // if
    else if ( node -> right -> BF == 1 ) {
      RLrotate( node );
    }  // else if

  }  // if

}  // AVLTree::rotateNode

/*============================== AVLTree::LLrotate() ==============================*/
void AVLTree::LLrotate( treeNode * & node ) {
  treeNode * temp = node -> left;
  node -> left = temp -> right;
  temp -> right = node;

  node = temp;
}  // AVLTree::LLrotate()

/*============================== AVLTree::RRrotate() ==============================*/
void AVLTree::RRrotate( treeNode * & node ) {
  treeNode * temp = node -> right;
  node -> right = temp -> left;
  temp -> left = node;

  node = temp;
}  // AVLTree::LLrotate()

/*============================== AVLTree::LRrotate() ==============================*/
void AVLTree::LRrotate( treeNode * & node ) {
  treeNode * tempL = node -> left;
  treeNode * tempR = tempL -> right;

  tempL -> right = tempR -> left;
  node -> left = tempR -> right;
  tempR -> left = tempL;
  tempR -> right = node;

  node = tempR;
}  // AVLTree::LLrotate()

/*============================== AVLTree::RLrotate() ==============================*/
void AVLTree::RLrotate( treeNode * & node ) {
  treeNode * tempR = node -> right;
  treeNode * tempL = tempR -> left;

  tempR -> left = tempL -> right;
  node -> right = tempL -> left;
  tempL -> left = node;
  tempL -> right = tempR;

  node = tempL;
}  // AVLTree::LLrotate()

/*============================== AVLTree::getHeight() =============================*/
int AVLTree::getHeight( treeNode * node ) {
  if ( node == NULL ) {
    return 0;
  }  // if

  int rightH = getHeight( node -> right );
  int leftH = getHeight( node -> left );
  int height = 0;

  if ( rightH > leftH ) {
    height = rightH + 1;
  }  // if
  else {
    height = leftH + 1;
  }  // else

  return height;
}  // AVLTree::getHeight()

/*============================ AVLTree::getNumOfNode() ============================*/
int AVLTree::getNumOfNode( treeNode * root, int numOfNode ) {
  treeNode * temp = root;

  numOfNode = numOfNode + 1;

  if ( temp -> left != NULL ) {
    numOfNode = getNumOfNode( temp -> left, numOfNode );
  }  // if

  if ( temp -> right != NULL ) {
    numOfNode = getNumOfNode( temp -> right, numOfNode );
  }  // if

  return numOfNode;
}  // AVLTree::getNumOfNode()

/*=============================== AVLTree::showAll() ==============================*/
void AVLTree::showAll( treeNode * node ) {
  treeNode * temp = node;

    cout << "total id num: " << node -> item.id.size() << endl;

    for ( int i = 0 ; i < ( node -> item.id.size() ) ; i = i + 1 ) {
      cout << "[" << node -> item.id[i] << "] \t" << ( node -> item.key ) << endl;
    }  // for

  cout << "---------------------------------------" << endl;

  if ( node -> left != NULL ) {
    cout << node -> item.key << "-left" << endl;
    showAll(node -> left);
  }  // if

  if ( node -> right != NULL ) {
    cout << node -> item.key << "-right" << endl;
    showAll( node -> right );
  }  // if

}  // AVLTree::showData()

/*================================ AVLTree::show() ================================*/
void AVLTree::show() {
  int num = 0;

  cout << "Tree height = " << getHeight(root) << endl;
  cout << "Number of nodes = " << getNumOfNode( root, 0 ) << endl;

  for ( int i = 0 ; i < root -> item.id.size() ; i = i + 1 ) {
      int id = root -> item.id[i] - 1;

      num = num + 1;
      cout << num << ": "
            << "[" << id + 1 << "] "
            << data[id].schoolName << ", "
            << data[id].departmentName << ", "
            << data[id].type << ", "
            << data[id].level << ", "
            << data[id].numOfStudent << ", "
            << data[id].numOfGraduate << endl;
  }  // for

  cout << endl;
}  // AVLTree::show()
