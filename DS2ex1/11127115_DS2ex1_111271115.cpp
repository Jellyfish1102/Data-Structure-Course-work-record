// 11127115 ³¯«º¦w
# include <iostream>
# include <fstream>    // ifstream
# include <sstream>    // istringstream
# include <algorithm>  // swap
# include <vector>
# include <string>

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

class Heap{
  protected:
    struct heapNode {
      int id;
      int studentNum;
    };  // heapNode

    vector<schoolType> data;

    void showData();
    void showHeap( vector<heapNode> heap );

    // check the max heap is legal
    void reHeapUp_max( vector<heapNode> & heap, int place );

    // check the min heap is legal
    void reHeapUp_min( vector<heapNode> & heap, int place );

    // find the leftmost node of the heap
    void findLeftmostBottom( int & leftmost, int range );

    // find max number of node of current level
    void findRightmost( int & rightmost, int range );

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
  vector<heapNode> deap;

  void clearUp();

  // compare with the corresponding node of current node
  bool checkCorrespondingPoint( int & place, int middle, int half );

  // update the leftmost, rightmost, half and middle point of current level
  void updateLevel( int place, int & leftmost, int & rightmost, int & half, int & middle );

  public:
    DEAP();
    ~DEAP();
    void show();
    void buildDEAP();
};  // class DEAP

int main() {
  string command = "";
  bool done = false;

  while ( done == false ) {
    cout << endl << "**** Heap Construction *****";
    cout << endl << "* 0. QUIT                  *";
    cout << endl << "* 1. Build a max heap      *";
    cout << endl << "* 2. Build a DEAP          *";
    cout << endl << "****************************";
    cout << endl << "Input a choice(0, 1, 2): ";
    cin >> command;

    Heap input;

    if ( command == "1" ) {
      MaxHeap max_heap;
      if ( max_heap.readFile() ) {
        max_heap.buildMaxHeap();
        max_heap.show();
      }  // if

    }  // if
    else if ( command == "2" ) {
      DEAP deap;
      if ( deap.readFile() ) {
        deap.buildDEAP();
        deap.show();
      }  // if

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
        else if ( k % 11 == 7 ){
          tempSchool.numOfStudent = stoi( item );
        }  // else if
        else if ( k % 11 == 8 ) {
          tempSchool.numOfProfessor = stoi( item );
        }  // else if
        else if ( k % 11 == 9 ) {
          tempSchool.numOfGraduate = stoi( item );
        }  // else if

        if ( k % 11 == 0 )
          data.push_back(tempSchool);
        }  // for

    }  // while

  }  // if

  fin.close();
  return success;
}  // IO::readFile()

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

}  // MaxHeap::reHeapUp_max()

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

}  // MaxHeap::reHeapUp_min()

/*========================== Heap::findLeftmostBottom() =========================*/
void Heap::findLeftmostBottom( int & leftmost, int range ) {
  leftmost = 0;

  // go find the left child until there is no left child
  while ( leftmost < range ) {
    leftmost = leftmost * 2 + 1;
  }  // while

  leftmost = ( leftmost - 1 )/ 2;
}  // MaxHeap::findLeftmostBottom()

/*========================= Heap::findRightmostBottom() =========================*/
void Heap::findRightmost( int & rightmost, int range ) {
  rightmost = 0;

  // go find the right child until reach the current level
  while ( rightmost < range) {
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

  findLeftmostBottom( leftmost, maxHeap.size() );

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
}  // DEAP::clearUp

/*============================== DEAP::buildDEAP() ==============================*/
void DEAP::buildDEAP() {
  heapNode temp;
  int place = 0;
  int rightmost = 0;
  int leftmost = 0;
  int middle = 0;
  int half = 0;

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

    // check the item is on max heap or on min heap
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
  findLeftmostBottom( leftmost, place + 1 );
  findRightmost( rightmost, place + 1 );
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
      swap(deap[place], deap[corresponding]);

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
      swap(deap[place], deap[corresponding]);

      place = corresponding;
      change = true;
    }  // if

  }  // else if

  return change;
}  // check

/*================================ DEAP::show() =================================*/
void DEAP::show() {
  heapNode bottom = deap[deap.size() - 1];
  int leftmost;

  findLeftmostBottom( leftmost, deap.size() );

  cout << "<DEAP>";
  cout << endl << "bottom: [" << bottom.id << "] " << bottom.studentNum;
  cout << endl << "leftmost bottom: [" << deap[leftmost].id << "] " << deap[leftmost].studentNum;
  cout << endl;
}  // DEAP::show()
