// 11127115 陳姿安, 11127117 芦添佑
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

    // get whole data
    vector<schoolType> getData();
};  // class Tree

class TwoThreeTree: public Tree {
  private:
    struct treeNode {
      vector<node> item;
      treeNode * previous;
      treeNode * left;
      treeNode * middle;
      treeNode * right;
      treeNode * fourth;

      treeNode():previous(nullptr), left(nullptr), middle(nullptr), right(nullptr), fourth(nullptr) {};
    };  // tree

    treeNode * root;

    // clear up whole tree
    void clearUp( treeNode * & node );

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

    void getTop( treeNode * & node, int k, int & num );

  public:
    TwoThreeTree();
    ~TwoThreeTree();
    void build23tree();
    bool rootIsEmpty();
    void getTopK();
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

    void getTop( treeNode * & node, int k, int & num );

  public:
    AVLTree();
    ~AVLTree();
    void buildAVLTree( vector<schoolType> inData );
    bool rootIsEmpty();
    void getTopK();
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
    cout << endl << "* 3. Top-K search on 23 tree  *";
    cout << endl << "* 4. Top-K search on AVL tree *";
    cout << endl << "*******************************";
    cout << endl << "Input a choice(0, 1, 2, 3, 4): ";
    cin >> command;

    if ( command == "1" ) {
      avlTree.~AVLTree();
      if ( twoThreeTree.readFile() ) {
        twoThreeTree.build23tree();
        twoThreeTree.show();
      }  // if

    }  // if
    else if ( command == "2" ) {
      if ( twoThreeTree.rootIsEmpty() == false ) {
        avlTree.buildAVLTree( twoThreeTree.getData() );
        avlTree.show();
      }  // if
      else {
        cout << "### Choose 1 first. ###" << endl;
      }  // else

    }  // else if
    else if ( command == "3" ) {
      if ( twoThreeTree.rootIsEmpty() == false ) {
        twoThreeTree.getTopK();
      }  // if
      else {
        cout << "### Choose 1 first. ###" << endl;
      }  // else

    }  // else if
    else if ( command == "4" ) {
      if ( avlTree.rootIsEmpty() == false ) {
        avlTree.getTopK();
      }  // if
      else {
        cout << "### Choose 2 first. ###" << endl;
      }  // else

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
    // check current char is a number or not
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

  if ( node -> left != NULL ) {
    clearUp( node -> left );
  }  // if

  if ( node -> middle != NULL ) {
    clearUp( node -> middle );
  }  // if

  if ( node -> right != NULL ) {
    clearUp( node -> right );
  }  // if

  delete node;
  node -> previous = NULL;
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

}  // TwoThreeTree::build23tree()

/*========================== TwoThreeTree::insertNode() ===========================*/
void TwoThreeTree::insertNode( node input ) {
  if ( root == NULL ) {
    root = new treeNode;
    root -> item.push_back(input);
  }  // if
  else {
    bool equalGraduate = false;
    treeNode * next = root;
    treeNode * temp = root;

    while ( next != NULL && equalGraduate == false ) {
      temp = next;

      // the value of insert item is smaller than the value of current smallest item
      if ( input.key < ( temp -> item.front().key ) ) {
        next = next -> left;
      }  // if

      // the value of insert item is bigger than the value of current largest item
      else if ( input.key > ( temp -> item.back().key ) ) {
        next = next -> right;
      }  // if

      // the value of insert item is between largest and smallest value of current node
      else if ( input.key > ( temp -> item.front().key ) &&
                 input.key < ( temp -> item.back().key ) ) {
        next = next -> middle;
      }  // else if

      // the value of insert item is equal to one of values of current node
      else {
        // the number of school graduate is equal to the node of 2-3 tree
        for ( int i = 0 ; i < ( temp -> item.size() ) ; i = i + 1 ) {
          if ( input.key == temp -> item[i].key ) {
            temp ->item[i].id.push_back( input.id[0] );
            equalGraduate = true;
          }  // if

        }  // for

      }  // else

    }  // while

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

  if ( node -> right != NULL ) {
    node2 -> left = node -> right;
    node -> right -> previous = node2;
  }  // if

  if ( node -> fourth != NULL ) {
    node2 -> right = node -> fourth;
    node -> fourth -> previous = node2;
  }  // if

  // tree grow up
  if ( parent == NULL ) {
    parent = new treeNode;
    root = parent;

    parent -> item.push_back( node -> item[1]);

    parent -> left = node;
    parent -> right = node2;
  }  // if
  else {
    // item up from right child (larger)
    if ( node -> item[1].key > parent -> item.back().key ) {
      parent -> item.push_back( node -> item[1] );
      if ( parent -> item.size() < 3 ) {
        parent -> middle = node;
        parent -> right = node2;
      }  // if
      else {
        parent -> right = node;
        parent -> fourth = node2;
      }  // else

    }  // if

    // item up from left child (smaller)
    else  if ( node -> item[1].key < parent -> item.front().key ) {
      parent -> item.insert( parent -> item.begin(), node -> item[1] );
      if ( parent -> item.size() < 3 ) {
        parent -> left = node;
        parent -> middle = node2;
      }  // if
      else {
        parent -> fourth = parent -> right;
        parent -> right = parent -> middle;
        parent -> middle = node2;
        parent -> left = node;
      }  // else

    }  // else if

    // item up from middle child => definitely is 3-node
    else {
      parent -> item.insert( parent -> item.begin() + 1, node -> item[1] );

      parent -> fourth = parent -> right;
      parent -> right = node2;
      parent -> middle = node;
    }  // else

  }  // else

  // update node content
  node -> item.erase( node -> item.begin() + 1, node ->item.end() );
  node -> right = node -> middle;
  node -> middle = NULL;
  node -> fourth = NULL;

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

  cout << "item num: " << node -> item.size() << endl;
  for ( int j = 0 ; j < (node -> item.size() ) ; j = j + 1 ) {
    cout << j + 1 << "-total id num: " << node -> item[j].id.size() << endl;

    for ( int i = 0 ; i < ( node -> item[j].id.size() ) ; i = i + 1 ) {
      cout << "[" << node -> item[j].id[i] << "] \t" << ( node -> item[j].key ) << endl;
    }  // for

  }  // for
  cout << "---------------------------------------" << endl;

  if ( node -> left != NULL ) {
    cout << "left" << endl;
    showAll(node -> left);
  }  // if

  if ( node -> middle != NULL ) {
    cout << "middle" << endl;
    showAll( node -> middle );
  }  // if

  if ( node -> right != NULL ) {
    cout << "right" << endl;
    showAll( node -> right );
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
  int height = 0;

  while ( temp != NULL ) {
    temp = temp -> left;
    height = height + 1;
  }  // while

  return height;
}  // TwoThreeTree::getHeight()

/*================================ TwoThreeTree::rootIsEmpty() ================================*/
bool TwoThreeTree::rootIsEmpty() {
  return root == NULL;
}  // TwoThreeTree::rootIsEmpty()

/*========================== TwoThreeTree::getNumOfNode() =========================*/
int TwoThreeTree::getNumOfNode( treeNode * root, int numOfNode ) {
  treeNode * temp = root;

  numOfNode = numOfNode + 1;

  if ( temp -> left != NULL ) {
    numOfNode = getNumOfNode( temp -> left, numOfNode );
  }  // if

  if ( temp -> middle != NULL ) {
    numOfNode = getNumOfNode( temp -> middle, numOfNode );
  }  // if

  if ( temp -> right != NULL ) {
    numOfNode = getNumOfNode( temp -> right, numOfNode );
  }  // if

  return numOfNode;
}  // TwoThreeTree::getNumOfNode()

/*============================= TwoThreeTree::getTopK() ===========================*/
void TwoThreeTree::getTopK() {
  int k = 0;
  cout << endl << "Enter K in [1," << data.size() << "]: ";
  cin >> k;

  treeNode * temp = root;
  int num = 1;

  if ( k <= data.size() ) {
    getTop( temp, k, num );
    cout << endl;
  }  // if

}  // TwoThreeTree::getTopK();

/*=============================== AVLTree::getTop() ===============================*/
void TwoThreeTree::getTop( treeNode * & node, int k, int & num ) {
  if ( node -> right != NULL & k > 0 ) {
    getTop( node -> right, k, num );
  }  // if

  int index = 0;

  //cout << " ==== right ==== " << endl;
  while ( num <= k && index < ( node -> item.back().id.size() ) ) {
    int id = node -> item.back().id[index] - 1;

    cout << num << ": "
          << "[" << id + 1 << "] "
          << data[id].schoolName << ", "
          << data[id].departmentName << ", "
          << data[id].type << ", "
          << data[id].level << ", "
          << data[id].numOfStudent << ", "
          << data[id].numOfGraduate << endl;

    num = num + 1;
    index = index + 1;
  }  // while

  if ( node -> middle != NULL && k > 0 ) {
    getTop( node -> middle, k, num );
    index = 0;

    //cout << " ==== middle ==== " << endl;
    while ( num <= k && index < ( node -> item[1].id.size() ) && ( node -> item.size() == 3 ) ) {
      int id = node -> item[1].id[index] - 1;

      cout << num << ": "
            << "[" << id + 1 << "] "
            << data[id].schoolName << ", "
            << data[id].departmentName << ", "
            << data[id].type << ", "
            << data[id].level << ", "
            << data[id].numOfStudent << ", "
            << data[id].numOfGraduate << endl;

      num = num + 1;
      index = index + 1;
    }  // while

  }  // if

   //cout << " ==== left ==== " << endl;
    index = 0;
    while ( num <= k && index < ( node -> item.front().id.size() ) && ( node -> item.size() > 1 ) ) {
      int id = node -> item.front().id[index] - 1;

      cout << num << ": "
            << "[" << id + 1 << "] "
            << data[id].schoolName << ", "
            << data[id].departmentName << ", "
            << data[id].type << ", "
            << data[id].level << ", "
            << data[id].numOfStudent << ", "
            << data[id].numOfGraduate << endl;

      num = num + 1;
      index = index + 1;
    }  // while

  if ( node -> left != NULL && k > 0 ) {
    getTop( node -> left, k, num );
  }  // if

}  // TwoThreeTree::getTop()

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

/*============================ AVLTree::rootIsEmpty() =============================*/
bool AVLTree::rootIsEmpty() {
  return root == NULL;
}  // AVLTree::rootIsEmpty()

/*============================== AVLTree::getTopK() ===============================*/
void AVLTree::getTopK() {
  int k = 0;
  cout << endl << "Enter K in [1," << data.size() << "]: ";
  cin >> k;

  treeNode * temp = root;
  int num = 1;

  if ( k <= data.size() ) {
    getTop( temp, k, num );
    cout << endl;
  }  // if

}  // AVLTree::getTopK()

/*=============================== AVLTree::getTop() ===============================*/
void AVLTree::getTop( treeNode * & node, int k, int & num ) {
  if ( node -> right != NULL & k > 0 ) {
    getTop( node -> right, k, num );
  }  // if

  int index = 0;

  while ( num <= k && index < ( node -> item.id.size() ) ) {
    int id = node -> item.id[index] - 1;

    cout << num << ": "
          << "[" << id + 1 << "] "
          << data[id].schoolName << ", "
          << data[id].departmentName << ", "
          << data[id].type << ", "
          << data[id].level << ", "
          << data[id].numOfStudent << ", "
          << data[id].numOfGraduate << endl;

    num = num + 1;
    index = index + 1;
  }  // while

  if ( node -> left != NULL && k > 0 ) {
    getTop( node -> left, k, num );
  }  // if

}  // AVLTree::getTop()
