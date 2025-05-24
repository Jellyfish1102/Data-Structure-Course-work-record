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

class Heap{
  protected:
    struct heapNode {
      int id;
      int studentNum;
    };  // heapNode

    vector<schoolType> data;

    void showData();
    void showHeap( vector<heapNode> heap );

    void cleanString( string & input );

    // check the max heap is legal
    void reHeapUp_max( vector<heapNode> & heap, int place );

    // check the min heap is legal
    void reHeapUp_min( vector<heapNode> & heap, int place );

    // find the leftmost node of the heap
    void findLeftmostBottom( int & leftmost, int range );

    // find max number of node of current level
    void findRightmost( int & rightmost, int range );

    /** =================================================== **/

    // check the max heap is legal
    void reHeapDown_max( vector<heapNode> & heap, int & place, int bottom );

    // check the min heap is legal
    void reHeapDown_min( vector<heapNode> & heap, int & place, int bottom );

  public:
    bool readFile();  // read data from file
};  // class Heap

class MaxHeap: public Heap {
  vector<heapNode> maxHeap;

  void clearUp();

  public:
    MaxHeap();
    ~MaxHeap();
    void show();
    void buildMaxHeap();
};  // class MaxHeap

class DEAP: public Heap {
  struct TopK {
    int id;
    schoolType school;
  };  // TopK

  vector<heapNode> deap;
  vector<TopK> topK;

  void clearUp();

  // compare with the corresponding node of current node
  bool checkCorrespondingPoint( int & place, int middle, int half );

  // compare with the children of corresponding node
  bool checkCorrespondingChild( int & place, int middle, int half );

  // update the leftmost, rightmost, half and middle point of current level
  void updateLevel( int place, int & leftmost, int & rightmost, int & half, int & middle );

  void getBiggest();

  void getSmallest();

  public:
    DEAP();
    ~DEAP();
    void show();
    void buildDEAP();
    void getTopK();
    void showTopK();
};  // class DEAP

int main() {
  string command = "";
  bool done = false;
  MaxHeap max_heap;
  DEAP deap;

  while ( done == false ) {
    cout << endl << "**** Heap Construction ********";
    cout << endl << "* 0. QUIT                     *";
    cout << endl << "* 1. Build a max heap         *";
    cout << endl << "* 2. Build a DEAP             *";
    cout << endl << "* 3. Top-K maximums from DEAP *";
    cout << endl << "*******************************";
    cout << endl << "Input a choice(0, 1, 2, 3): ";
    cin >> command;

    Heap input;

    if ( command == "1" ) {
      if ( max_heap.readFile() ) {
        max_heap.buildMaxHeap();
        max_heap.show();
      }  // if

    }  // if
    else if ( command == "2" ) {
      if ( deap.readFile() ) {
        deap.buildDEAP();
        deap.show();
      }  // if

    }  // else if
    else if ( command == "3" ) {
      deap.getTopK();
    }  // else if
    else if ( command == "0" ) {
      done = true;
    }  // else if
    else {
      cout << endl << "Command does not exist!" << endl;
    }  // else

  }  // while

}  // main

/**class Heap**/
/*================================ Heap::readFile ===============================*/
bool Heap::readFile() {
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
}  // Heap::readFile()

// ============================= Heap::cleanString() ============================*/
void Heap::cleanString( string & input ) {
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
}  // Heap::cleanString

/*============================= Heap::reHeapUp_max() ============================*/
void Heap::reHeapUp_max( vector<heapNode> & heap, int place ) {
  int parent = ( place - 1 ) / 2;

  /**the id of parent should not equal to -1 because root of DEAP is pseudo**/
  while ( parent >= 0 && heap[place].studentNum > heap[parent].studentNum &&
           heap[parent].id > 0 ) {
    swap( heap[parent], heap[place] );

    place = parent;
    parent = ( place - 1 ) / 2;
  }  // while

}  // Heap::reHeapUp_max()

/*============================= Heap::reHeapUp_min() ============================*/
void Heap::reHeapUp_min( vector<heapNode> & heap, int place ) {
  int parent = ( place - 1 ) / 2;

  /**the id of parent should not equal to -1 because root of DEAP is pseudo**/
  while ( parent >= 0 && heap[place].studentNum < heap[parent].studentNum &&
           heap[parent].id != -1 ) {
    swap( heap[parent], heap[place] );

    place = parent;
    parent = ( place - 1 ) / 2;
  }  // while

}  // Heap::reHeapUp_min()

/*============================ Heap::reHeapDown_max() ===========================*/
void Heap::reHeapDown_max( vector<heapNode> & heap, int & place, int bottom ) {
  bool done = false;
  int child = place * 2 + 1;

  while ( child <= bottom && done == false ) {
    int rightChild = child + 1;

    if ( rightChild <= bottom && heap[rightChild].studentNum > heap[child].studentNum ) {
      child = rightChild;
    }  // if

    if ( heap[child].studentNum > heap[place].studentNum ) {
      heapNode temp = heap[child];
      heap[child] = heap[place];
      heap[place] = temp;

      place = child;
      child = place * 2 + 1;
    }  // if
    else {
      done = true;
    }  // else

  }  // while

}  // Heap::reHeapDown_max()

/*============================ Heap::reHeapDown_min() ===========================*/
void Heap::reHeapDown_min( vector<heapNode> & heap, int & place, int bottom ) {
  bool done = false;
  int child = place * 2 + 1;

  while ( child <= bottom && done == false ) {
    int rightChild = child + 1;
    if ( rightChild <= bottom && heap[rightChild].studentNum < heap[child].studentNum ) {
      child = rightChild;
    }  // if

    if ( heap[child].studentNum < heap[place].studentNum) {
      swap( heap[child], heap[place] );

      place = child;
      child = place * 2 + 1;
    }  // if
    else {
      done = true;
    }  // else

  }  // while

}  // Heap::reHeapDown_min()

/*========================== Heap::findLeftmostBottom() =========================*/
void Heap::findLeftmostBottom( int & leftmost, int range ) {
  leftmost = 0;

  // go find the left child until there is no left child
  while ( leftmost <= range ) {
    leftmost = leftmost * 2 + 1;
  }  // while


  leftmost = ( leftmost - 1 ) / 2;
}  // MaxHeap::findLeftmostBottom()

/*========================= Heap::findRightmostBottom() =========================*/
void Heap::findRightmost( int & rightmost, int range ) {
  rightmost = 0;

  // go find the right child until reach the current level
  while ( rightmost < range ) {
    rightmost = rightmost * 2 + 2;
  }  // while

}  // Heap:: findRightmost()

/*============================== Heap::showData() ===============================*/
void Heap::showData() {
  for ( int j = 0 ; j < data.size() ; j = j + 1  ) {
    cout << data[j].schoolName << "\t"
          << data[j].departmentName << "\t"
          << data[j].type << "\t"
          << data[j].level << "\t"
          << data[j].numOfStudent << "\t"
          << data[j].numOfProfessor << "\t"
          << data[j].numOfGraduate << endl;
  }  // for

}  // Heap::showData()

/*============================== Heap::showHeap() ===============================*/
void Heap::showHeap( vector<heapNode> heap ) {
  for ( int j = 0 ; j < heap.size() ; j = j + 1  ) {
    cout << "[" << heap[j].id << "]" << "\t"
          << heap[j].studentNum << endl;
  }  // for

}  // Heap::showHeap()


/**class MaxHeap**/
/*============================= MaxHeap::MaxHeap() ==============================*/
MaxHeap::MaxHeap( ) {
  clearUp();
}  // MaxHeap::MaxHeap()

/*============================ MaxHeap::~MaxHeap() ==============================*/
MaxHeap::~MaxHeap( ) {
  clearUp();
}  // MaxHeap::MaxHeap()

/*============================= MaxHeap::clearUp() ==============================*/
void MaxHeap::clearUp( ) {
  maxHeap.erase( maxHeap.begin(), maxHeap.end() );
}  // MaxHeap::clearUp()

/*=========================== MaxHeap::buildMaxheap() ===========================*/
void MaxHeap::buildMaxHeap() {
  heapNode temp;

  clearUp();

  for ( int i = 0 ; i < data.size() ; i = i + 1 ) {
    temp.id = i + 1;
    temp.studentNum = data[i].numOfStudent;
    maxHeap.push_back(temp);
    reHeapUp_max( maxHeap, maxHeap.size() - 1 );
  }  // for

}  // MaxHeap::buildMaxHeap

/*=============================== MaxHeap::show() ===============================*/
void MaxHeap::show() {
  heapNode root = maxHeap[0];
  heapNode bottom = maxHeap[maxHeap.size() - 1];
  int leftmost;

  findLeftmostBottom( leftmost, maxHeap.size() - 1 );

  cout << "<max heap>";
  cout << endl << "root: [" << root.id << "] " << root.studentNum;
  cout << endl << "bottom: [" << bottom.id << "] " << bottom.studentNum;
  cout << endl << "leftmost bottom: [" << maxHeap[leftmost].id << "] " << maxHeap[leftmost].studentNum;
  cout << endl;
}  // MaxHeap::show()


/**class DEAP**/
/*================================ DEAP::DEAP() =================================*/
DEAP::DEAP() {
  clearUp();
}  // DEAP::DEAP()

/*=============================== DEAP::~DEAP() =================================*/
DEAP::~DEAP() {
  clearUp();
}  // DEAP::DEAP()

/*============================== DEAP::clearUp() ================================*/
void DEAP::clearUp() {
  deap.erase( deap.begin(), deap.end() );
  topK.erase( topK.begin(), topK.end() );
}  // DEAP::clearUp

/*============================== DEAP::buildDEAP() ==============================*/
void DEAP::buildDEAP() {
  heapNode temp;
  int place = 0;
  int rightmost = 0;
  int leftmost = 0;
  int middle = 0;
  int half = 0;

  clearUp();

  // pseudo root
  temp.id = -1;
  temp.studentNum = 0;
  deap.push_back(temp);


  for ( int i = 0 ; i < data.size() ; i = i + 1 ) {
    temp.id = i + 1;
    temp.studentNum = data[i].numOfStudent;
    deap.push_back(temp);

    place = deap.size() - 1;

    // check current level is full or not
    if ( place > rightmost ) {
      // update the information of level
      updateLevel( place, leftmost, rightmost, half, middle );
    }  // if

    // check the item is on max heap or not
    if ( place > middle ) {
      // check the item with its corresponding node
      if ( checkCorrespondingPoint( place, middle, half ) )
        reHeapUp_min( deap, place );
      else
        reHeapUp_max( deap, place );
    }  // if
    else {
      // check the item with its corresponding node
      if ( checkCorrespondingPoint( place, middle, half ) )
        reHeapUp_max( deap, place );
      else
        reHeapUp_min( deap, place );
    }  // else

  }  // for

}  // DEAP::buildDEAP()

/*============================= DEAP::updateLevel() =============================*/
void DEAP::updateLevel( int place, int & leftmost, int & rightmost, int & half, int & middle ) {
  findLeftmostBottom( leftmost, place );
  findRightmost( rightmost, place );
  half = ( rightmost - leftmost + 1 ) / 2;
  middle = rightmost - half;
}  // DEAP::updateLevel()

/*======================= DEAP::checkCorrespondingPoint() =======================*/
bool DEAP::checkCorrespondingPoint( int & place, int middle, int half ) {
  bool change = false;
  int corresponding = 0;

  // new item at min tree
  if ( place <= middle ) {
    corresponding = place + half;

    // if there is no corresponding node at this level -> go find parent of that corresponding node
    if ( corresponding > deap.size() - 1 ) {
      corresponding = ( corresponding - 1 ) / 2;
    }  // if

    // the item at min heap is bigger than the corresponding node which at max heap -> swap
    /** the id of corresponding node should not be -1 ( the id of pseudo root )**/
    if ( ( deap[place].studentNum > deap[corresponding].studentNum ) &&
          deap[corresponding].id != -1 ) {
      heapNode temp = deap[place];
      deap[place] = deap[corresponding];
      deap[corresponding] = temp;

      place = corresponding;
      change = true;
    }  // if

  }  // if

  // new item at max tree
  else if ( place > middle ) {
    corresponding = place - half;

    // the item at max heap is bigger than the corresponding node which at min heap -> swap
    /** the id of corresponding node should not be -1 ( the id of pseudo root )**/
    if ( ( deap[place].studentNum < deap[corresponding].studentNum ) &&
          deap[corresponding].id != -1 ) {
      heapNode temp = deap[place];
      deap[place] = deap[corresponding];
      deap[corresponding] = temp;

      place = corresponding;
      change = true;
    }  // if

  }  // else if

  return change;
}  // DEAP::checkCorrespondingPoint

/*======================= DEAP::checkCorrespondingChild() =======================*/
bool DEAP::checkCorrespondingChild( int & place, int middle, int half ) {
  bool change = false;
  int correspondingChild = ( place - half ) * 2 + 1;

  if ( correspondingChild < deap.size() ) {
    int correspondingRightChild = correspondingChild + 1;
    if ( deap[correspondingRightChild].studentNum > deap[correspondingChild].studentNum ) {
      correspondingChild = correspondingRightChild;
    }  // if

    if ( deap[correspondingChild].studentNum > deap[place].studentNum ) {
      heapNode temp = deap[correspondingChild];
      deap[correspondingChild] = deap[place];
      deap[place] = temp;

      place = correspondingChild;
      change = true;
    }  // if

  }  // if

  return change;
}  // DEAP::checkCorrespondingChild()
/*================================ DEAP::show() =================================*/
void DEAP::show() {
  heapNode bottom = deap[deap.size() - 1];
  int leftmost;

  findLeftmostBottom( leftmost, deap.size() - 1 );

  cout << "<DEAP>";
  cout << endl << "bottom: [" << bottom.id << "] " << bottom.studentNum;
  cout << endl << "leftmost bottom: [" << deap[leftmost].id << "] " << deap[leftmost].studentNum;
  cout << endl;
}  // DEAP::show()

/*============================= DEAP::getBiggest() ==============================*/
void DEAP::getBiggest() {
  int place = 2;
  int rightmost = 2;
  int leftmost = 1;
  int middle = 1;
  int half = 1;
  bool change = false;
  TopK temp;

  if ( deap.size() > 1 ) {
    if ( deap.size() > 2 ) {
      temp.id = deap[2].id;
      temp.school = data[temp.id - 1];
      topK.push_back( temp );
    }  // if
    else {
      temp.id = deap[1].id;
      temp.school = data[temp.id - 1];
      topK.push_back( temp );
    }  // else

    deap[2] = deap[deap.size() - 1];

    deap.pop_back();

    reHeapDown_max( deap, place, deap.size() - 1 );

    updateLevel( place, leftmost, rightmost, half, middle );

    if ( ( place * 2 + 1 ) > deap.size() - 1 )
      change = checkCorrespondingChild( place, middle, half );

    if ( change == false )
      change = checkCorrespondingPoint( place, middle , half );

    if ( change == true )
      reHeapUp_min( deap, place );
  }  // else

}  // DEAP::getBiggest()

/*============================ DEAP::getSmallest() ==============================*/
void DEAP::getSmallest() {
  int place = 1;
  int rightmost = 2;
  int leftmost = 1;
  int middle = 1;
  int half = 1;

  if ( deap.size() <= 1 ) {
    cout << "the DEAP is empty!" << endl;
  }  // if
  else {
    deap[1] = deap[deap.size() - 1];
    deap.pop_back();

    reHeapDown_min( deap, place, deap.size() - 1 );

    updateLevel( place, leftmost, rightmost, half, middle );

    checkCorrespondingPoint( place, middle , half );

  }  // else

}  // DEAP::getSmallest()

/*============================== DEAP::getTopK() ================================*/
void DEAP::getTopK() {
  int k = -1;

  topK.erase( topK.begin(), topK.end() );

  if ( deap.size() > 1 ) {
    cout << endl << "Enter the value of K in [1," << deap.size() - 1 << "]: ";
    cin >> k;

    for ( int i = 0 ; i < k && deap.size() > 1 ; i = i + 1 ) {
      getBiggest();
    }  // for

    showTopK();
  }  // else

}  // DEAP::getTopK

/*============================== DEAP::showTopK() ===============================*/
void DEAP::showTopK() {

  for ( int j = 0 ; j < topK.size() ; j = j + 1  ) {
    cout  << "Top " << right << setw(3) << j + 1 << ": ["
          << topK[j].id << "] "
          << topK[j].school.schoolName
          << topK[j].school.departmentName << ", "
          << topK[j].school.type << ", "
          << topK[j].school.level << ", "
          << topK[j].school.numOfStudent << endl;
  }  // for

}  // DEAP::showTopK()
