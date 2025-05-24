// 11127115 陳姿安, 11127117 芦添佑
# include <iostream>
# include <fstream>    // ofstream, ifstream
# include <sstream>
# include <cstring>    // strcmp, strcpy
# include <string>
# include <vector>
# include <queue>
# include <map>
# include <set>
# include <iomanip>    // setw
# include <algorithm>  // find
# include <time.h>

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
    // read data from binary file
    bool readBinFile();

    // return data to other class/object
    vector<student> getData();

    // return fileName to other class/object
    string getFileName();
};  // class Data

class Graph {
  private:
    struct GetID {
      char getID[12];
      float weight;

      GetID() : getID("\0"), weight(-1) {}
      GetID( char inID[12], float inW ) : weight(inW) { strcpy( getID, inID ); }
      bool operator <(const GetID & a ) const { return strcmp( getID, a.getID) < 0 ; }
    };  // GetID

    struct PutID {
      char putID[12];
      int counts;
      set< GetID > next;

      PutID(): putID("\0"), counts(-1) {}
    };  // PutID

    int nodesNum;

    map< string, PutID > adjacencyList;

    // clear up the content of adjacency List
    void cleanAdjList( map< string, PutID > & inList );

    // check putID is in adjacency list or not, do the process of putID
    void processPutID( student data );

    // check getID is in adjacency list or not, do the process of getID
    void processGetID( student data );

    // display adjacency list on screen
    void showList( map<int, PutID> inList );

    map< int, map<string, PutID > > connection;

    // clear up the content of connection
    void cleanCnt( map< int, map< string, PutID > > & inList );

    // process BFS
    void BFS( PutID  item, map< string, PutID > & sequence );

  public:
    Graph();
    ~Graph();

    // check the adjacency list is empty or not
    bool listIsEmpty();

    // create adjacency list with data
    void createAdjacencyList( vector<student> data );

    // calculate connection counts of each putID
    void calculateConnectionCounts();

    // output adjacency list as a text file
    void outputFile_adj( string fileName );

    // output connection as a text file
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
        double start = clock();
        graph.calculateConnectionCounts();
        graph.outputFile_cnt( data.getFileName() );
        double ends = clock();

        cout << "time = " << ends - start << " ms" << endl;
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

  if ( fin.fail() && fileName != "0" ) {
    cout << endl << "### pairs" << fileName << ".txt does not exist! ###" << endl;
  }  // if
  else if ( fileName != "0" ) {
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
// ================================ Graph::Graph() ================================= //
Graph::Graph() {
  cleanAdjList( adjacencyList );
  cleanCnt( connection );
}  // Graph::Graph()

// =============================== Graph::~Graph() ================================= //
Graph::~Graph() {
  cleanAdjList( adjacencyList );
  cleanCnt( connection );
}  // Graph::~Graph()

// ============================= Graph::listIsEmpty() ============================== //
bool Graph::listIsEmpty () {
  return adjacencyList.empty();
}  // Graph::listIsEmpty()

// ============================= Graph::cleanAdjList() ============================= //
void Graph::cleanAdjList( map< string, PutID > & inList ) {
  // clear up linked list
  for ( auto & it: inList ) {
    PutID temp = it.second;
    temp.next.clear();
  }  // for

  // clear up vector
  inList.erase( inList.begin(), inList.end() );
}  // Graph::cleanList()

// ============================== Graph::cleanCnt() ================================ //
void Graph::cleanCnt( map< int, map< string, PutID > > & inList ) {
  // clear up linked list
  for ( auto & it: inList ) {
    map< string, PutID > temp = it.second;
    for ( auto & iter: temp ) {
      PutID item = iter.second;
      item.next.clear();

    }  // for

  }  // for

  // clear up vector
  inList.erase( inList.begin(), inList.end() );
}  // Graph::cleanList()

/* ========================= Graph::createAdjacencyList() ========================== */
void Graph::createAdjacencyList( vector<student> data ) {
  cleanAdjList( adjacencyList );
  nodesNum = 0;

  for ( int i = 0 ; i < data.size() ; i = i + 1 ) {
    // process putID
    processPutID( data[i] );

    // precess getID
    processGetID( data[i] );

  }  // for

  // showList( adjacencyList );
}  // Graph::createAdjacencyList()

// ============================ Graph::processPutID() ============================== //
void Graph::processPutID( student data ) {

  // check there are such putID in list
  map< string, PutID >::iterator iter = adjacencyList.find( data.putID );

  GetID temp_get( data.getID, data.weight );

  // there are not current putID in list
  if ( iter == adjacencyList.end() ) {
    PutID temp_put;
    strcpy( temp_put.putID, data.putID );
    temp_put.next.insert(temp_get);

    adjacencyList[data.putID] = temp_put;
  }  // if

  // current putID already in vector -> just put getID and weight in linked list
  else {
    adjacencyList[data.putID].next.insert(temp_get);
  }  // else

  nodesNum = nodesNum + 1;
}  // Graph::processPutID()

// ============================ Graph::processGetID() ============================== //
void Graph::processGetID( student data ) {

  // check there are such getID in list
  map< string, PutID >::iterator iter = adjacencyList.find( data.getID );

  // there are not current getID in list
  if ( iter == adjacencyList.end() ) {
    PutID temp_put;
    strcpy( temp_put.putID, data.getID );

    adjacencyList[data.getID] = temp_put;
  }  // if

}  // Graph::processGetID

// ============================== Graph::showList() ================================ //
void Graph::showList( map<int, PutID> inList ) {
  int index = 1;
  for ( auto & it: inList ) {
    PutID item = it.second;
    set<GetID> temp = item.next;
    int num = 1;

    cout << "=========================" << endl;
    cout << "[" << index << "]" << item.putID << "(" << item.counts << "): " << endl;

    for ( auto iter = temp.begin() ; iter != temp.end() ; iter++ ) {
      cout << "\t(" << num << ") " << iter -> getID << ", " << iter -> weight << endl;
      num = num + 1;
    }  // while

    index = index + 1;
  }  // for

}  // Graph::showList()

/* ======================= Graph::calculateConnectionCounts() ====================== */
void Graph::calculateConnectionCounts() {
  cleanCnt( connection );
  map< string, PutID > sequence;

  for ( auto & it: adjacencyList ) {
    PutID item = it.second;
    BFS( item, sequence );

    PutID temp = sequence[item.putID];

    map<int, map<string, PutID> >::iterator iter = connection.find( temp.counts );

    connection[temp.counts][temp.putID] = temp;
  }  // for

  sequence.clear();
}  // Graph::calculateConnectionCounts()

// ================================== Graph::BFS() ================================= //
void Graph::BFS( PutID item, map< string, PutID > & sequence ) {
  int dataSize = adjacencyList.size();
  set<string> visit;

  PutID connect;
  strcpy( connect.putID, item.putID );
  queue<string> q;
  q.push( item.putID );
  visit.insert( item.putID );

  while ( !q.empty() ) {
    string temp = q.front();
    char id[12];
    strcpy( id, temp.c_str() );
    q.pop();

    // put pop id into vector connection
    if ( strcmp( item.putID, id ) != 0 ) {
      GetID temp_get( id, 0 );
      connect.next.insert(temp_get);
    }  // if

    // put unvisited id which adjacent to current item into queue
    for ( auto temp = adjacencyList[id].next.begin() ; temp != adjacencyList[id].next.end() ; temp++ ) {

      set<string>::iterator it = visit.find( temp -> getID );
      if ( it == visit.end() ) {
        q.push( temp -> getID );
        visit.insert(temp -> getID );

        // check there are already have path of temp in sequence or not
        map< string, PutID>::iterator iter = sequence.find(temp -> getID );
        if ( iter != sequence.end() ) {
          PutID t = sequence[temp -> getID];
          for ( auto & itr : t.next ) {
            GetID tmp = itr;
            visit.insert( tmp.getID );
            if ( strcmp( tmp.getID, connect.putID ) != 0 )
              connect.next.insert( tmp );
          }  // for

        }  // if

      }  // if

    }  // for

  }  // while

  connect.counts = connect.next.size();
  sequence[item.putID] = connect;

  visit.erase( visit.begin(), visit.end() );
}  // Graph::BFS()

// ============================= Graph::outputFile_adj() =========================== //
void Graph::outputFile_adj( string fileName ) {
  ofstream fout;
  fout.open( "pairs" + fileName + ".adj" );

  fout << "<<< There are " << adjacencyList.size() << " IDs in total. >>>" << endl;

  int index = 1;

  // rum all put ids
  for ( auto & it : adjacencyList ) {
    PutID item = it.second;

    // write put id into file
    fout << "[" << setw(3) << index << "] " << item.putID << ": " << endl;

    // write all connected put id into file
    int num = 1;
    for ( auto temp = item.next.begin() ; temp != item.next.end() ; temp++ ) {
      fout << "\t" << "(" << setw(2) << num << ") " << temp -> getID << "," << setw(7) << temp -> weight;
      if ( num % 12 == 0 ) fout << endl;
      num = num + 1;
    }  // for

    fout << endl;
    index = index + 1;
  }  // for

  fout << "<<< There are " << nodesNum << " nodes in total. >>>";

  cout << endl << "<<< There are " << adjacencyList.size() << " IDs in total. >>>" << endl;
  cout << endl << "<<< There are " << nodesNum << " nodes in total. >>>" << endl;
  fout.close();
}  // Graph::outputFile_adj()

// ============================= Graph::outputFile_cnt() =========================== //
void Graph::outputFile_cnt( string fileName ) {
  int totalIDs = 0;
  // calculate there are how many ids in connection list
  for ( auto & it : connection ) {
    map<string, PutID> temp = it.second;
    totalIDs = totalIDs + temp.size();
  }  // for

  ofstream fout;
  fout.open( "pairs" + fileName + ".cnt", ios::out );

  fout << "<<< There are " << totalIDs << " IDs in total. >>>";

  int index = 1;

  // run all connection counts
  map< int, map<string, PutID > >::reverse_iterator re_it;
  for ( re_it = connection.rbegin() ; re_it != connection.rend() ; re_it ++ ) {
    int counts = re_it -> first;
    map< string, PutID > temp = re_it -> second;

    // run all put ids
    for ( auto & iter : temp ) {
      string putID = iter.first;
      PutID item = iter.second;

      // write put id into file
      fout << endl << "[" << setw(3) << index << "] " << putID << "(" << counts << "): " << endl;

      // write connected get id into file
      int num = 1;
      for ( auto ptr = item.next.begin() ; ptr != item.next.end() ; ptr++ ) {
        fout << "\t" << "(" << setw(2) << num << ") " << ptr -> getID ;
        if ( num % 12 == 0 ) fout << endl;
        num = num + 1;
      }  // for

      index = index + 1;
    }  // for

  }  // for

  // show totalIDs on screen
  cout << endl << "<<< There are " << totalIDs << " IDs in total. >>>" << endl;

  fout.close();
}  // Graph::output_cnt())
