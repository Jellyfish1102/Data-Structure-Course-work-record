// 11127115 陳姿安, 11127117 芦添佑
# include <iostream>
# include <fstream>
# include <sstream>
# include <cstring>
# include <string>
# include <vector>
# include <queue>
# include <iomanip>

using namespace std;

struct student {
  char putID[12];
  char getID[12];
  float weight;
};  // student

class Data {
  private:
    string fileName;
    vector<student> data;

    void showData();

  public:
    bool readBinFile();
    vector<student> getData();
    string getFileName();
};  // class Data

class Graph {
  private:
    struct GetID {
      char getID[12];
      float weight;
      GetID* next;

      GetID() : getID("\0"), weight(-1), next(nullptr) {}
      GetID( char inID[10], float inW ) : weight(inW), next(nullptr) { strcpy( getID, inID ); }
    };  // data

    struct PutID {
      char putID[12];
      int counts;
      GetID * next;

      PutID(): putID("\0"), counts(-1),next(nullptr) {}
    };  // PutID

    int nodesNum;
    void cleanList( GetID* & temp );
    void cleanUp( vector<PutID> & inList );

    vector<PutID> adjacencyList;
    void processPutID( student data );
    void processGetID( student data );
    void putInList( GetID* & inPtr, char id[10], float weight );
    void sortList( vector<PutID> & inList );
    void showList( vector<PutID> inList );

    vector<PutID> connection;
    void BFS( PutID  item, int index_connection );
    int findIndex( char id[10], vector<PutID> inList );
    void markVisit( bool* & visit, char id[10] );
    bool isVisited( bool* visit, char id[10] );

  public:
    bool listIsEmpty();
    void createAdjacencyList( vector<student> data );
    void calculateConnectionCounts();
    void outputFile_adj( string fileName );
    void outputFile_cnt( string fileName );
};  // class Graph

int main() {
  string command = "";
  bool done = false;
  Data data;
  Graph graph;

  while ( done == false ) {
    cout << endl << "**** Graph data manipulation *****";
    cout << endl << "* 0. QUIT                        *";
    cout << endl << "* 1. Build adjacency lists       *";
    cout << endl << "* 2. Compute connection counts   *";
    cout << endl << "**********************************";
    cout << endl << "Input a choice(0, 1, 2): ";
    cin >> command;

    if ( command == "1" ) {
      if ( data.readBinFile() ) {
        graph.createAdjacencyList( data.getData() );
        graph.outputFile_adj( data.getFileName() );
      }  // if

    }  // if
    else if ( command ==  "2" ) {
      if ( graph.listIsEmpty() == false ) {
        graph.calculateConnectionCounts();
        graph.outputFile_cnt( data.getFileName() );
      }  // if
      else
        cout << "### Command 1 first. ###" << endl;

    }  // else if
    else if ( command == "0" ) {
      done = true;
    }  // else if
    else {
      cout << endl << "Command does not exist!" << endl;
    }  // else

    cout << endl;
  }  // while

  return 0;
}  // main

/** class Data **/
// ============================== Data::readBinFile() ============================== //
bool Data::readBinFile( ) {
  ifstream fin;
  string temp;
  bool successful = false;

  student tempS;

  data.erase( data.begin(), data.end() );

  cout << endl << "Input a file number([0]:Quit): ";
  cin >> fileName;
  fin.open( "pairs" + fileName + ".bin", ios::in | ios::binary );

  if ( fin.fail() ) {
    cout << endl << "### pairs" << fileName << ".txt does not exist! ###" << endl;
  }  // if
  else {
    successful = true;

    // move file pointer to the end of file
    fin.seekg( 0, fin.end );

    // use tellg() to get file size
    // divide by the size of structure student to get how many data need to be read
    int dataNum = fin.tellg() / sizeof(student);

    // move file pointer to the begin of file
    fin.seekg( 0, fin.beg );

    for ( int i = 0 ; i < dataNum ; i = i + 1 ) {
      // read data( structure )
      fin.read( (char*)&tempS, sizeof(tempS) );

      data.push_back(tempS);
    }  // for

  }  // else

  // showData();

  fin.close();
  return successful;
}  // Data::readBinFile()

// =============================== Data::showData() ================================ //
void Data::showData() {
  for ( int i = 0 ; i < data.size() ; i = i + 1 ) {
    cout << " =============== " << endl
          << "putID: " << data[i].putID << endl
          << "getID: " << data[i].getID << endl
          << "weight: " << data[i].weight << endl;
  }  // for

}  // Data::showData()

// ================================ Data::getData() ================================ //
vector<student> Data::getData() {
  return data;
}  // Data::getData

// ============================== Data::getFileName() ============================== //
string Data::getFileName() {
  return fileName;
}  // Data::getFileName()

/** class Graph **/
// ============================= Graph::listIsEmpty() ============================== //
bool Graph::listIsEmpty () {
  return adjacencyList.empty();
}  // Graph::listIsEmpty()

// =============================== Graph::cleanUp() ================================ //
void Graph::cleanUp( vector<PutID> & inList ) {
  // clear up linked list
  for ( int i = 0 ; i < inList.size() ; i = i + 1 ) {
    cleanList( inList[i].next );
  }  // for

  // clear up vector
  inList.erase( inList.begin(), inList.end() );
}  // Graph::cleanList()

// ============================== Graph::cleanList() =============================== //
void Graph::cleanList( GetID* & temp ) {
  if ( temp -> next != NULL ) {
    cleanList( temp -> next );
  }  // if

  delete temp;
  temp = NULL;
}  // Graph::cleanList()

/* ========================= Graph::createAdjacencyList() ========================== */
void Graph::createAdjacencyList( vector<student> data ) {
  cleanUp( adjacencyList );
  nodesNum = 0;

  for ( int i = 0 ; i < data.size() ; i = i + 1 ) {
    // process putID
    processPutID( data[i] );

    // precess getID
    processGetID( data[i] );

  }  // for

  sortList( adjacencyList );
  // showList( adjacencyList );
}  // Graph::createAdjacencyList()

// ============================ Graph::processPutID() ============================== //
void Graph::processPutID( student data ) {
  bool found_put = false;  // check putID in vector or not
  int index;

  // check list is empty and check there are such putID in list or not
  // if list is empty -> list size is zero
  for ( index = 0 ; index < adjacencyList.size() && found_put == false ; index = index + 1 ) {
    if ( strcmp( adjacencyList[index].putID, data.putID ) == 0 )
      found_put = true;
  }  // for

  // current list is empty or there are not current putID in list
  if ( found_put == false ) {
    PutID temp_put;
    strcpy( temp_put.putID, data.putID );
    temp_put.next = new GetID( data.getID, data.weight );

    adjacencyList.push_back(temp_put);
  }  // if

  // current putID already in vector -> just put getID and weight in linked list
  else {
    putInList( adjacencyList[index - 1].next, data.getID, data.weight );
  }  // else

  nodesNum = nodesNum + 1;
}  // Graph::processPutID()

// ============================ Graph::processGetID() ============================== //
void Graph::processGetID( student data ) {
  bool found_get = false;  // check getID in vector or not

  // check there are such getID in list
  // int index = findIndex( data.getID, adjacencyList );

  for ( int i = 0 ; i < adjacencyList.size() && found_get == false ; i = i + 1 ) {
    if ( strcmp( adjacencyList[i].putID, data.getID ) == 0 )
      found_get = true;
  }  // for

  // there are not current getID in list
  if ( found_get == false ) {
    PutID temp_put;
    strcpy( temp_put.putID, data.getID );

    adjacencyList.push_back(temp_put);
  }  // if

}  // Graph::processGetID

// ============================== Graph::putInList() =============================== //
void Graph::putInList( GetID* & inPtr, char id[10], float weight ) {
    GetID* tail = inPtr;

    // find the location which item needed be put
    while ( tail != NULL && tail -> next != NULL && strcmp( id, tail -> next-> getID ) > 0 ) {
      tail = tail -> next;
    }  // while

    // put new item into linked list
    GetID* temp = new GetID( id, weight );

    if ( tail != NULL ) {
      // id is small than the smallest id in list -> insert item in the head of list
      if ( tail == inPtr  && strcmp( id, tail-> getID ) < 0 ) {
        temp -> next = inPtr;
        inPtr = temp;
      }  // if

      // id is small than id of tail -> insert item at the next location of tail
      else {
        temp -> next = tail -> next;
        tail -> next = temp;
      }  // else

    }  // if

    // there are no item in list or inID is the bigger than the biggest one of list
    else
      inPtr = new GetID( id, weight );

}  // Graph::putInList()

// ============================== Graph::sortList() ================================ //
void Graph::sortList( vector<PutID> & inList ) {
  for ( int i = 0 ; i < inList.size() ; i = i + 1 ) {
    for ( int j = i ; j < inList.size() ; j = j + 1 ) {
      if ( strcmp( inList[i].putID, inList[j].putID ) > 0 ) {
        swap( inList[i], inList[j] );
      }  // if

    }  // for

  }  // for

}  // Graph::sortList()

// ============================== Graph::showList() ================================ //
void Graph::showList( vector<PutID> inList ) {
  for ( int i = 0 ; i < inList.size() ; i = i + 1 ) {
    GetID* temp = inList[i].next;
    int num = 1;

    cout << "=========================" << endl;
    cout << "[" << i + 1 << "]" << inList[i].putID << "(" << inList[i].counts << "): " << endl;

    while ( temp != NULL ) {
      cout << "\t(" << num << ") " << temp -> getID << ", " << temp -> weight << endl;
      temp = temp -> next;
      num = num + 1;
    }  // while

  }  // for

}  // Graph::showList()

/* ======================= Graph::calculateConnectionCounts() ====================== */
void Graph::calculateConnectionCounts() {
  cleanUp( connection );
  connection = adjacencyList;

  for ( int k = 0 ; k < connection.size() ; k = k + 1 ) {
    connection[k].next = NULL;
  }  // for

  for ( int i = 0 ; i < adjacencyList.size() ; i = i + 1 ) {
    BFS( adjacencyList[i], i );
  }  // for

  // showList(connection);
}  // Graph::calculateConnectionCounts()

// ================================== Graph::BFS() ================================= //
void Graph::BFS( PutID item, int index_connection ) {
  int dataSize = adjacencyList.size();
  bool* visit = new bool[dataSize]();

  int counts = 0;
  queue<char*> q;
  q.push( item.putID );
  markVisit( visit, item.putID );

  while ( !q.empty() ) {
    string temp = q.front();
    char id[10];
    strcpy( id, temp.c_str() );
    q.pop();

    // find the location of current id which pop from queue in adjacency list
    int index = findIndex( id, adjacencyList );

    // put pop id into vector connection
    if ( strcmp( connection[index_connection].putID, id ) != 0 )
      putInList( connection[index_connection].next, id, 0 );

    // put unvisited id which adjacent to current item into queue
    for ( GetID * temp = adjacencyList[index].next ; temp != NULL ; temp = temp -> next ) {

      if ( isVisited( visit, temp -> getID ) == false ) {
        q.push( temp -> getID );
        markVisit( visit, temp -> getID );
        counts = counts + 1;
      }  // if

    }  // for

  }  // while

  connection[index_connection].counts = counts;
  delete[] visit;
}  // Graph::BFS()

// =============================== Graph::findIndex() ============================== //
int Graph::findIndex( char id[10], vector<PutID> inList ) {
  int index = -1;
  bool done = false;
  int beginLocate = 0;
  int endLocate = inList.size() - 1;

  while ( done == false && beginLocate <= endLocate ) {
    int middle = ( endLocate + beginLocate ) / 2;

    if ( strcmp( inList[middle].putID, id ) == 0 ) {
      index = middle;
      done = true;
    }  // if
    else if ( strcmp( inList[middle].putID, id ) > 0 ) {
      endLocate = middle - 1;
    }  // else if
    else if ( strcmp( inList[middle].putID, id ) < 0 ) {
      beginLocate = middle + 1;
    } // else if

  }  // while

  return index;
}  // Graph::findIndex()

// =============================== Graph::markVisit() ============================== //
void Graph::markVisit( bool* & visit, char id[10] ) {
  int index = findIndex( id, adjacencyList );

  visit[index] = true;
}  // Graph::markVisit()

// =============================== Graph::isVisited() ============================== //
bool Graph::isVisited( bool* visit, char id[10] ) {
  int index = findIndex( id, adjacencyList );

  return  visit[index];
}  // Graph::isVisited()

// ============================= Graph::outputFile_adj() =========================== //
void Graph::outputFile_adj( string fileName ) {
  cout << endl << "<<< There are " << adjacencyList.size() << " IDs in total. >>>" << endl;
  cout << endl << "<<< There are " << nodesNum << " nodes in total. >>>" << endl;

  ofstream fout;
  fout.open( "" + fileName + ".adj" );

  fout << "<<< There are " << adjacencyList.size() << " IDs in total. >>>" << endl;

  for ( int i = 0 ; i < adjacencyList.size() ; i = i + 1 ) {
    fout << "[" << setw(3) << i + 1 << "] " << adjacencyList[i].putID << ":" << endl;

    int num = 1;
    for ( GetID* temp = adjacencyList[i].next ; temp != NULL ; temp = temp -> next ) {
      fout << setw(5) << "(" << setw(2) << num << ") " << temp -> getID << "," << setw(6) << temp -> weight;
      num = num + 1;
    }  // for

    fout << endl;
  }  // for

  fout << "<<< There are " << nodesNum << " nodes in total. >>>";
  fout.close();
}  // Graph::outputFile_adj()

// ============================= Graph::outputFile_cnt() =========================== //
void Graph::outputFile_cnt( string fileName ) {
  cout << endl << "<<< There are " << connection.size() << " IDs in total. >>>" << endl;

  ofstream fout;
  fout.open( "" + fileName + ".cnt" );

  fout << "<<< There are " << connection.size() << " IDs in total. >>>" << endl;

  for ( int i = 0 ; i < connection.size() ; i = i + 1 ) {
    fout << "[" << setw(3) << i + 1 << "] " << connection[i].putID << "(" << connection[i].counts << "):" << endl;

    int num = 1;
    for ( GetID* temp = connection[i].next ; temp != NULL ; temp = temp -> next ) {
      fout << setw(5) << "(" << setw(2) << num << ") " << temp -> getID;
      num = num + 1;
    }  // for

    fout << endl;
  }  // for

  fout.close();
}  // Graph::output_cnt())
