// 11127115 陳姿安, 11127156 王佩諭
# include <iostream>
# include <sstream>
# include <fstream>
# include <string>
# include <cstdlib>
# include <cstdio>
# include <iomanip>
# include <vector>
# include <algorithm>

using namespace std;

struct schoolType{
  string schoolName;
  string departmentName;
  string type;
  string level;
  int numOfStudent;
  int numOfProfessor;
  int numOfGraduate;
};  // schoolType

class SchoolList{

  vector<schoolType> aList;
  string fileID;

  void reset();
  void showList();  // show input content
  void cleanString( string & input );

  public:
    SchoolList();
    ~SchoolList();
    bool readFile( string fileName );  // read in input file
    bool isEmpty();  // check the list is empty or not
    vector<schoolType> getList();  // get the list of school data
};  // SchoolList

class Tree {
  struct node{
    schoolType school;
    node * right;
    node * left;

  };  // node

  node * root;
  int height;
  int visit;
  vector<schoolType> vSearchGraduate;
  vector<schoolType> vSearchName;
  vector<schoolType> vDelete;


  void clean();  // clean up every thing
  void cleanTree( node* & nodes );  // clean up whole tree
  void cleanVector();  // clean up the content of vector
  void show( node* root, int & num );  // show tree
  void show( vector<schoolType> outV );  // show result of search
  bool isDigit( string inS );  // check the string is digit or not
  int updateHeight( node* root );

  public:
    Tree();
    ~Tree();
    bool isEmpty();  //check the tree is empty or not
    void setroot( schoolType school );  // set up root
    void attachLeftSubtree( node* & temp, schoolType school );  // add left child
    void attachRightSubtree( node* & temp, schoolType school );  // add right child

    int getHeight( );  // get tree height
    void createTree_G( vector<schoolType> aList );  // create a tree by graduation number
    void createTree_N( vector<schoolType> aList );  // create a tree by school name
    void searchGraduate( int num, vector<schoolType> & temp, node* root );  // search nodes in tree which graduation number are bigger than input num
    void searchSchool( string name, vector<schoolType> & temp, node* root );  // search nodes in tree which school name are equal to input name
    void getSearch( string num, string name );  // get the result of search
    void getSubTree( vector<schoolType> & temp, node* root );  // get left or right subtree

    bool deleteNode( string name );
    bool deleteNode_G( string name, schoolType deleteItem );
    vector<schoolType> getDeleteV();
    void deleteLeaf( node* & root );
    void deleteOneChild( node* & root, node* & prev, char direction );
    void deleteTwoChildren( node* & root );
    void showDelete();
    void cleanVdelete();
};  // Tree

int main() {
  string command = "";
  string fileName = "";
  bool stop = false;

  SchoolList sList;
  Tree graduteTree;
  Tree NameTree;

  while ( stop != true ) {

    cout << endl << "*** University Graduate Information System ***";
    cout << endl << "* 0. Quit                                    *";
    cout << endl << "* 1. Create Two Binary Search Trees          *";
    cout << endl << "* 2. Search by Number of Graduates           *";
    cout << endl << "* 3. Search by School Name                   *";
    cout << endl << "* 4. Removal by School Name                  *";
    cout << endl << "**********************************************";
    cout << endl << "Input a command(0, 1-4): ";

    cin >> command;

    string inNum = " ";
    string inName = "";

    if ( ( command == "2" || command == "3" || command == "4" ) && sList.isEmpty() ) {
          cout << endl << "Please choose command 1 first!" << endl;
    }  // if

    else if ( command == "1") {
      cout << endl << "Input a file number: ";
      cin >> fileName;

      if ( sList.readFile( "input" + fileName + ".txt" ) ) {
        NameTree.createTree_N( sList.getList() );
        graduteTree.createTree_G( sList.getList() );
      }  // if
      else
        cout << endl << "There is no data!" << endl;

    }  // if

    else if ( command == "2" ) {
      cout << "Input the number of graduates: " ;
      cin >> inNum;
      graduteTree.getSearch( inNum, inName );
    }  // else if

    else if ( command == "3" ) {
      cout << "Input a school name: " ;
      cin >> inName;
      NameTree.getSearch( inNum, inName );
    }  // else if

    else if ( command == "4" ) {
      NameTree.cleanVdelete();
      graduteTree.cleanVdelete();
      cout << "Input a school name: ";
      cin >> inName;
      while ( NameTree.deleteNode( inName ) ) {}
      NameTree.showDelete();
      vector<schoolType> vDelete = NameTree.getDeleteV();
      for ( int i = 0 ; i < vDelete.size() ; i = i + 1 )
        while ( graduteTree.deleteNode_G( inName, vDelete[i] ) ) {}


      cout << "Tree height {school Name} = " << NameTree.getHeight() << endl;
      cout << "Tree height {Number of graduates} = " << graduteTree.getHeight() << endl;
    }  // else if

    else if ( command == "0" ) {
      stop = true;
    }  // else if
    else {
      cout << endl << "Command does not exist!" << endl;
    }  // else

  }  // while

}  // main()

/** class SchoolList **/
// ============================= SchoolList::SchoolList() =============================
SchoolList::SchoolList() {
  reset();
}  // SchoolList::SchoolList()

// ============================ SchoolList::~SchoolList() =============================
SchoolList::~SchoolList() {
  reset();
}  // SchoolList::~SchoolList()

// ================================ SchoolList::reset() ===============================
void SchoolList::reset() {
  fileID = "";
  aList.erase( aList.begin(), aList.end() );
}  // SchoolList::reset()

// =============================== SchoolList::showList() =============================
void SchoolList::showList() {
  cout << endl << fileID << endl;
  for ( int j = 0 ; j < aList.size() ; j = j + 1 ) {
    cout << "[" << j + 1 << "]\t"
          << aList[j].schoolName << "\t" << aList[j].departmentName << "\t" << aList[j].type << "\t"
          << aList[j].level << "\t" << aList[j].numOfStudent << "\t" << aList[j].numOfProfessor << "\t"
          << aList[j].numOfGraduate << endl;
  }  // for

}  // SchoolList::showList()

// ================================ SchoolList::getList() =============================
vector<schoolType> SchoolList::getList() {
  return aList;
}  // SchoolList::getList();
// ============================== SchoolList::readFile() ==============================
bool SchoolList::readFile( string fileName ) {
  ifstream fin;
  schoolType temp;
  string tempS = "";
  bool success = true;

  reset();

  fin.open( fileName );

  if ( fin.fail() ) {
    success = false;
    cout << endl << "### " << fileName << " does not exist! ###" << endl;
  }  // if
  else {
    getline( fin, tempS );  // 大專校院各校科系別概況
    getline( fin, tempS );  // 學年度

    for ( int i = 0 ; fin >> tempS && i < 11 ; i = i + 1 ) {
      if ( i == 1 || ( i <= 8 && i >= 3 ) ) {
        fileID = fileID + "\t" + tempS;
      }  // if

    }  // for

    for ( int j = 1 ; getline( fin, tempS ) ; j = j + 1 ) {
      istringstream line( tempS );  // istringstream -> read string from tempS
      string item;

      for ( int k = 1 ; getline( line, item, '\t' ) ; k = k + 1 ) {

        if ( k % 11 == 2 )
          temp.schoolName = item;
        else if ( k % 11 == 4 )
          temp.departmentName = item;
        else if ( k % 11 == 5 )
          temp.type = item;
        else if ( k % 11 == 6 )
          temp.level = item;
        else if ( k % 11 == 7 ){
          cleanString( item );
          temp.numOfStudent = stoi( item );
        }  // else if
        else if ( k % 11 == 8 ) {
          cleanString( item );
          temp.numOfProfessor = stoi( item );
        }  // else if
        else if ( k % 11 == 9 ) {
          cleanString( item );
          temp.numOfGraduate = stoi( item );
        }  // else if

        if ( k % 11 == 0 )
          aList.push_back(temp);
        }  // for

    }  // for

    if ( aList.empty() ) {
      success = false;
      cout << endl << "### Get nothing from the file " << fileName << " ! ###" << endl;
    }  // if

  }  // else

  fin.close();
  return success;
}  // SchoolList::readFile()


// ============================= SchoolList::cleanString() ============================
void SchoolList::cleanString( string & input ) {
  for ( int i = 0 ; i < input.size() ; i = i + 1 ) {
    if ( input[i] == '\"'|| input[i] == ',' ) {
      input.erase( i, 1 );
    }  // if

  }  // for
}  // SchoolList::cleanString

// =============================== SchoolList::isEmpty() ==============================
bool SchoolList::isEmpty() {
  bool isempty = false;
  if ( aList.empty() ) {
    isempty = true;
  }  // if

  return isempty;
}  // SchoolList::isEmpty()

/** class Tree **/
// =================================== Tree::Tree() ===================================
Tree::Tree() {
  height = 0;
  visit = 0;
  cleanVector();
  root = NULL;
}  // Tree::Tree()

// ================================== Tree::~Tree() ===================================
Tree::~Tree() {
  clean();
}  // Tree::~Tree()

// ================================== Tree::clean() ===================================
void Tree::clean() {
  height = 0;
  visit = 0;
  cleanVector();

  cleanTree( root );
  delete root;
  root = NULL;
}  // Tree::clean()

// ================================ Tree::cleanTree() =================================
void Tree::cleanTree( node* & nodes ) {

  if ( nodes == NULL ) {
    return;
  }  // if

  if ( nodes -> left != NULL ) {
    cleanTree( nodes -> left );
  }  // else

  if ( nodes -> right != NULL ) {
    cleanTree( nodes -> right );
  }  // if

  delete nodes;
  nodes = NULL;

}  // Tree::cleanTree()

// =============================== Tree::cleanVector() ================================
void Tree::cleanVector() {
  vSearchGraduate.erase( vSearchGraduate.begin(), vSearchGraduate.end() );
  vSearchName.erase( vSearchName.begin(), vSearchName.end() );
  vDelete.erase( vDelete.begin(), vDelete.end() );
}  // Tree::cleanVector

// ================================= Tree::isEmpty() ==================================
bool Tree::isEmpty() {
  bool isempty = false;
  if ( root -> left == NULL && root -> right == NULL ) {
    isempty = true;
  }  // if

  return  isempty;
}  // Tree::isEmpty()

// ================================= Tree::setroot() ==================================
void Tree::setroot( schoolType school ) {
  root = new node;
  root -> school = school;
  root -> left = NULL;
  root -> right = NULL;
  height = 1;
}  // Tree::setroot

// ============================ Tree::attachLeftSubtree ===============================
void Tree::attachLeftSubtree( node* & temp, schoolType school ) {
  temp -> left = new node;
  temp = temp -> left;
  temp -> left = NULL;
  temp -> right = NULL;
  temp -> school = school;
}  // Tree::attachLeftSubtree()

// ============================ Tree::attachRightSubtree ==============================
void Tree::attachRightSubtree( node* & temp, schoolType school ) {
  temp -> right = new node;
  temp = temp -> right;
  temp -> left = NULL;
  temp -> right = NULL;
  temp -> school = school;
}  // Tree::attachRightSubtree()

// =================================== Tree::show() ===================================
void Tree::show( node* root, int & num ) {
  node *temp = root;

  cout << "[" << num << "]" << "\t"
        << temp -> school.schoolName << "\t" << temp -> school.departmentName << "\t" << temp -> school.type << "\t"
        << temp -> school.level << "\t" << temp -> school.numOfStudent << "\t" << temp -> school.numOfProfessor << "\t"
        << temp -> school.numOfGraduate << endl;
  num = num + 1;

  if ( temp -> left != NULL )
    show( temp -> left, num );

  if ( temp -> right != NULL )
    show( temp -> right, num );

}  // Tree::show()

// =================================== Tree::show() ===================================
void Tree::show( vector<schoolType> outV ) {
  for ( int j = 0 ; j < outV.size() ; j = j + 1 ) {
    cout << "[" << j + 1 << "]\t"
          << outV[j].schoolName << "\t" << outV[j].departmentName << "\t" << outV[j].type << "\t"
          << outV[j].level << "\t" << outV[j].numOfStudent << "\t" << outV[j].numOfProfessor << "\t"
          << outV[j].numOfGraduate << endl;
  }  // for

}  // Tree::show_G()

// ================================ Tree::getHeight() =================================
int Tree::getHeight( ) {
  return height;
}  // Tree::getHeight()

// =============================== Tree::getSubTree() =================================
void Tree::getSubTree( vector<schoolType> & temp, node* root ) {
  node* tempPtr = root;

  temp.push_back( tempPtr -> school );

  if ( tempPtr -> left != NULL )
    getSubTree( temp, tempPtr -> left );

  if ( tempPtr -> right != NULL )
    getSubTree( temp, tempPtr -> right );

}  // Tree::getSubTree()

// =============================== Tree::createTree_G =================================
void Tree::createTree_G( vector<schoolType> aList ) {
  clean();

  for ( int i = 0 ; i < aList.size() ; i = i + 1 ) {
    if ( root == NULL ) {
      setroot(aList[i]);
    }  // if
    else {
      bool done = false;
      node * temp = root;
      visit = 1;

      do {
        if ( aList[i].numOfGraduate < ( temp -> school.numOfGraduate ) ) {
          if ( temp -> left == NULL ) {
            done = true;
            attachLeftSubtree( temp, aList[i]);
          }  // if
          else {
            temp = temp -> left;
          }  // else

        }  // if
        else if ( aList[i].numOfGraduate >= ( temp -> school.numOfGraduate ) ) {
          if ( temp -> right == NULL ) {
            done = true;
            attachRightSubtree( temp, aList[i]);
          }  // if
          else {
            temp = temp -> right;
          }  // else

        }  // else if

        visit = visit + 1;
      } while ( done == false );

      if ( visit > height )
        height = visit;
    } // else

  }  // for

  cout << "Tree height {Number of graduates} = " << height << endl;
}  // Tree::createTree_G()

// =============================== Tree::createTree_N =================================
void Tree::createTree_N( vector<schoolType> aList ) {
  clean();

  for ( int i = 0 ; i < aList.size() ; i = i + 1 ) {
    if ( root == NULL ) {
      setroot(aList[i]);
    }  // if
    else {
      bool done = false;
      node * temp = root;
      visit = 1;

      do {
        if ( aList[i].schoolName < ( temp -> school.schoolName ) ) {
          if ( temp -> left == NULL ) {
            done = true;
            attachLeftSubtree( temp, aList[i]);
          }  // if
          else {
            temp = temp -> left;
          }  // else

        }  // if
        else if ( aList[i].schoolName >= ( temp -> school.schoolName ) ) {
          if ( temp -> right == NULL ) {
            done = true;
            attachRightSubtree( temp, aList[i]);
          }  // if
          else {
            temp = temp -> right;
          }  // else

        }  // else if

        visit = visit + 1;
      } while ( done == false );

      if ( visit > height )
        height = visit;
    } // else

  }  // for

  cout << "Tree height {School name} = " << height << endl;
  /*
  int num = 1;
  show(root, num);
  */
}  // Tree::createTree_N()

// ============================== Tree::searchGraduate ================================
void Tree::searchGraduate( int num, vector<schoolType> & temp, node* root ) {
  node* tempPtr = root;

  // current num of graduate bigger than input num -> go left subtree find more
  if ( tempPtr -> school.numOfGraduate >= num ) {
    temp.push_back( tempPtr -> school );
    if ( tempPtr -> left != NULL )
      searchGraduate( num, temp, tempPtr -> left );
  }  // if

  // current num of graduate smaller than input num -> go right subtree find more
  if ( tempPtr -> school.numOfGraduate < num ) {
    if ( tempPtr -> right != NULL )
      searchGraduate( num, temp, tempPtr -> right );
  }  // if

  // there are no more num of graduate which will bigger than input num -> get the subtree which is bigger than input0
  if ( tempPtr -> right != NULL && tempPtr -> school.numOfGraduate >= num )
    getSubTree( temp, tempPtr -> right );

}  // Tree::searchGraduate

// ============================== Tree::searchSchool ================================
void Tree::searchSchool( string name, vector<schoolType> & temp, node* root ) {
  node* tempPtr = root;

  // name is small than current school -> go find smaller school
  if ( name < ( tempPtr -> school.schoolName ) ) {
    if ( tempPtr -> left != NULL )
      searchSchool( name, temp, tempPtr -> left );
  }  // if

  if ( tempPtr -> school.schoolName == name ) {
    temp.push_back( tempPtr -> school );
  }  // if

  // name is big than current school -> go find biger school
  if ( name >= ( tempPtr -> school.schoolName ) ) {
    if ( tempPtr -> right != NULL )
      searchSchool( name, temp, tempPtr -> right );
  }  // if

}  // Tree::searchSchool

// ================================ Tree::getSearchG ==================================
void Tree::getSearch( string num, string name ) {
  cleanVector();

  node* temp = root;
  int intNum = 0;

  bool digit = isDigit(num);

  if ( digit == true ) {
    intNum = stoi( num );
  } // if
  else if ( name == "" )
    cout << "### the input string s is not a decimal number! ###" << endl;

  if ( digit == true ) {
    searchGraduate( intNum, vSearchGraduate, temp );

    if ( !vSearchGraduate.empty() ) {
      cout << "Search results: "<< endl;
      show( vSearchGraduate );
    }  // if

  }  // if
  else if ( name != "" ) {
    searchSchool( name, vSearchName, temp );

    if ( !vSearchName.empty() ) {
      cout << "Search results: "<< endl;
      show( vSearchName );
    }  // if

  }  // else if

  if ( ( digit == true && vSearchGraduate.empty() ) ||
        ( name != "" && vSearchName.empty() ) ||
        ( digit == false && name == "" ) )
    cout << "There is no match!" << endl;

}  // Tree::getSearchG

// ================================= Tree::isDigit ====================================
bool Tree::isDigit( string inS ) {
  bool digit = true;

  for ( int i = 0 ; i < inS.size() && digit == true ; i = i + 1 ) {
    if ( isdigit(inS[i] ) == false ) {
      digit = false;
    }  // if

  }  // for

  return digit;
}  // Tree::isDigit

// =============================== Tree::deleteNode ===================================
bool Tree::deleteNode( string name ) {
  bool done = false;
  node* prev = NULL;
  node* temp = root;
  char direction = '0';


  while ( temp != NULL && done == false ) {
    if ( name > temp -> school.schoolName ) {
      prev = temp;
      temp = temp -> right;
      direction = 'r';
    }  // if

    else if ( name == temp -> school.schoolName ) {
      vDelete.push_back( temp -> school );
      if ( temp -> left == NULL && temp -> right == NULL ) {
        deleteLeaf(temp);
        if ( direction == 'r' )
          prev -> right = NULL;
        else if ( direction == 'l' )
          prev -> left = NULL;
        else if ( prev == NULL )
          root = NULL;
      }  // if

      else if ( temp -> left != NULL && temp -> right != NULL )
        deleteTwoChildren(temp);

      else {
        if ( prev == NULL ) {
          temp = root;
          if ( root -> right != NULL )
            root = root -> right;
          else
            root = root -> left;

          delete temp;
          temp = NULL;
        }  // if
        else
          deleteOneChild(temp, prev, direction );
      }  // else

      done = true;
    }  // else if
    else if ( name < temp -> school.schoolName ) {
      prev = temp;
      temp = temp -> left;
      direction = 'l';
    }  // else if

  }  // while

  /*
  cout << "===================== school name ===================" << endl;
  int n = 1;
  show( root, n );
  cout << endl << endl ;
  */
  height = updateHeight(root);
  return done;
}  // Tree::deleteNode


// ============================== Tree::deleteNode_G ==================================
bool Tree::deleteNode_G( string name, schoolType deleteItem ) {
  bool done = false;
  node* prev = NULL;
  node* temp = root;
  char direction = '0';


  while ( temp != NULL && done == false ) {
    if ( ( deleteItem.numOfGraduate > temp -> school.numOfGraduate ) ||
         ( deleteItem.numOfGraduate == temp -> school.numOfGraduate && name != temp -> school.schoolName ) ) {
      prev = temp;
      temp = temp -> right;
      direction = 'r';
    }  // if

    else if ( deleteItem.numOfGraduate == temp -> school.numOfGraduate &&
               name == temp -> school.schoolName ) {
      vDelete.push_back( temp -> school );
      if ( temp -> left == NULL && temp -> right == NULL ) {
        deleteLeaf(temp);
        if ( direction == 'r' )
          prev -> right = NULL;
        else if ( direction == 'l' )
          prev -> left = NULL;
        else if ( prev == NULL )
          root = NULL;
      }  // if

      else if ( temp -> left != NULL && temp -> right != NULL )
        deleteTwoChildren(temp);

      else {
        if ( prev == NULL ) {
          temp = root;
          if ( root -> right != NULL )
            root = root -> right;
          else
            root = root -> left;

          delete temp;
          temp = NULL;
        }  // if
        else
          deleteOneChild(temp, prev, direction );
      }  // else

      done = true;
    }  // else if
    else if ( deleteItem.numOfGraduate < temp -> school.numOfGraduate ) {
      prev = temp;
      temp = temp -> left;
      direction = 'l';
    }  // else if

  }  // while

  /*
  cout << "==================== graduation ====================" << endl;
  int n = 1;
  show( root, n );
  cout << endl << endl ;
  */

  height = updateHeight(root);
  return done;
}  // Tree::deleteNode_G

// =============================== Tree::deleteLeaf ===================================
void Tree::deleteLeaf( node* & root ) {
  delete root;
  root = NULL;
}  // Tree::deleteLeaf

// ============================== Tree::deleteOneChild ================================
void Tree::deleteOneChild( node* & root, node* & prev, char direction ) {

  if ( root -> right != NULL ) {
    if ( direction == 'r' )
    prev -> right = root -> right;
    else if ( direction == 'l' )
    prev -> left = root -> right;
  }  // if
  else {
    if ( direction == 'r' )
    prev -> right = root -> left;
    else if ( direction == 'l' )
    prev -> left = root -> left;
  }  // else

  delete root;
  root = NULL;
}  // Tree::deleteOneChild

// ============================ Tree::deleteTwoChildren ===============================
void Tree::deleteTwoChildren( node* & root ) {
  node* temp = root -> right ;
  node* prev = NULL;

  while ( temp -> left != NULL ) {
    prev = temp;
    temp = temp -> left;
  }  // while

  root -> school = temp -> school;

  if ( temp -> right != NULL && prev != NULL )
    prev -> left = temp -> right;
  else if ( temp -> right == NULL && prev != NULL )
    prev -> left = NULL;
  else if ( temp -> right != NULL && prev == NULL )
    root -> right = temp -> right;
  else if ( temp -> right == NULL && prev == NULL )
    root -> right = NULL;

  delete temp;
  temp = NULL;
}  // Tree::deleteTwoChildren

// ================================ Tree::showDelete ==================================
void Tree::showDelete() {
  cout << "Deleted records:" << endl;
  show(vDelete);
}  // Tree::showDelete

// ================================= Tree::getDeleteV ==================================
vector<schoolType>  Tree::getDeleteV() {
  return vDelete;
}  //  Tree::getDeleteV

// ================================ Tree::cleanVdelete =================================
void Tree::cleanVdelete() {
  vDelete.erase( vDelete.begin(), vDelete.end() );
}  // Tree::cleanVdelete

// ================================ Tree::updateHeight =================================
int Tree::updateHeight( node* root ) {
  node* temp = root;

  if ( root == NULL )
    return 0;
  else {
    int left = updateHeight( temp -> left );
    int right = updateHeight( temp -> right );

    if ( left > right ) {
      return 1 + left;
    }  // if
    else
      return 1 + right;
  }

}  // Tree::updateHeight
