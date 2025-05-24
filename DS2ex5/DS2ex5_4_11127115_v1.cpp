// 11127115 ³¯«º¦w
# include <iostream>
# include <fstream>
# include <sstream>
# include <algorithm>
# include <string>
# include <vector>
# include <utility>  // pair
# include <ctime>

# define BUFFER_SIZE 300
# define HASHING_SIZE 151

using namespace std;

struct student{
  char putID[10];
  char getID[10];
  float weight;

  bool operator >(const student & a ) const { return weight > a.weight; }
};

class Data {
  string fileName;
  int num;
  double internalTime;

  vector<student> data;

  void showData();
  void internalSort();
  void writeBinFile();
  void reset();

  public:
    Data();
    ~Data();
    bool readBinFile();
    string getFileName();
    int getTotalFileNum();
    double getInternalSortTime();
};  // class Data

class File {
  vector<student> fileData;
  int readIndex = 0;

  public:
    File();
    ~File();
    void reset();
    bool getFileData( string fileName );
    vector<student> getData();
    student getFirstItem();
    void deleteFirstItem();
    bool fileDataIsEmpty();
};  // class File

class ExSort {
  struct file {
    vector<student> fileData;
    int readIndex;
  };  // file

  int inFileNum;
  int outFileNum;
  double externalTime;
  vector<student> sortedData;

  void mergeData( File & data1, File & data2, string fileName1, string fileName2 );
  void writeFile( string fileName );

  public:
    void sortData( int fileNum, string fileName );
};  // class Sort

class Hash {
  struct item {
    pair< float, int > label;
    int hvalue;

    item(): label(-1, -1), hvalue(-1) {}
  };  // item

  item* table;
  int totalItemNum;

  void insertData( float weight, int offset );
  int hashing( float weight );
  void reset();

  public:
    Hash();
    ~Hash();
    void buildHash( string fileName );
};  // class BPlusTree

int main() {
  Data data;
  ExSort exSort;
  Hash hashing;
  string command = "";

  while ( command != "0" ) {
    cout << endl << "*** The buffer size is 300";
    cout << endl << "***********************************************";
    cout << endl << " On-machine Exercise 05                       *";
    cout << endl << " Mission 1: External Merge Sort on a Big File *";
    cout << endl << " Mission 2: Construction of Primary Index     *";
    cout << endl << "***********************************************";
    cout << endl << "########################################################";
    cout << endl << "Mission 1: External merge sort ";
    cout << endl << "########################################################";

    cout << endl;
    data.readBinFile();

    cout << endl << "The internal sort is completed. Check the initial sorted runs!" << endl;

    double start = clock();
    exSort.sortData( data.getTotalFileNum(), data.getFileName() );
    double ends = clock();

    cout << endl << "The execution time ..." << endl;
    cout << "Internal Sort = " << data.getInternalSortTime() << " ms" << endl;
    cout << "External Sort = " << ends - start << " ms" << endl;
    cout << "Total Execution Time = " << data.getInternalSortTime() + ends - start << " ms" << endl;

    cout << endl;

    cout << endl << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
    cout << "Mission 2: Build the primary index " << endl;
    cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
    cout << endl << "<Primary index>: (key, offset)" << endl;

    hashing.buildHash( data.getFileName() );

    cout << endl << "[0]Quit or [Any other key]continue?" << endl;
    cin >> command;
  }  // while

  return 0;
}  // main()

/** class Data **/
// ================================== Data::Data() ================================= //
Data::Data() {
  reset();
}  // Data::Data()

// ================================= Data::reset() ================================= //
void Data::reset() {
  num = 0;
  internalTime = 0;
  data.clear();
}  // Data::reset()

// ================================== Data::~Data() ================================= //
Data::~Data() {
  reset();
}  // Data::Data()

// ============================== Data::readBinFile() ============================== //
bool Data::readBinFile() {
  ifstream fin;
  bool successful = false;
  reset();

  student tempS;

  data.erase( data.begin(), data.end() );

  do {
    cout << endl << "Input the file number: [0]Quit " << endl;
    cin >> fileName;
    fin.open( "pairs" + fileName + ".bin", ios::in | ios::binary );

    if ( fin.fail() && fileName != "0" ) {
      cout << endl << "pairs" << fileName << ".bin does not exist!!!" << endl;
    }  // if

  } while ( fin.fail() && fileName != "0" );


  double start = clock();
  if ( fileName != "0" ) {
    successful = true;

    // move file pointer to the end of file
    fin.seekg( 0, fin.end );

    // use tellg() to get file size
    // divide by the size of structure student to get how many data need to be read
    int dataNum = fin.tellg() / sizeof(student);

    // move file pointer to the begin of file
    fin.seekg( 0, fin.beg );

    for ( int i = 1 ; i <= dataNum ; i = i + 1 ) {
      // read data( structure )
      fin.read( (char*)&tempS, sizeof(tempS) );

      data.push_back(tempS);

      if ( i % BUFFER_SIZE == 0 ) {
        internalSort();
        data.clear();
      }  // if

    }  // for

    internalSort();
  }  // else

  // showData();

  fin.close();
  double ends = clock();
  internalTime = internalTime + ends - start;
  return successful;
}  // Data::readBinFile()

// ============================= Data::internalSort() ============================== //
void Data::internalSort() {
  stable_sort( data.begin(), data.end(), greater<student>() );

  writeBinFile();
}  // internalSort()

// ============================= Data::writeBinFile() ============================== //
void Data::writeBinFile() {
  ofstream fout;

  // open / create output file
  num = num + 1;
  if ( num == 1 )
    fout.open("order" + fileName + ".bin", ios::out | ios::binary );
  else
    fout.open("order" + fileName + "_" + to_string(num) + ".bin", ios::out | ios::binary );

  //cout << num << " data size = " << data.size() << endl;
  //showData();

  int dataSize = data.size();

  // output data into binary file one by one ( structure )
  for ( int i = 0 ; i < data.size() ; i = i + 1 ) {
    /*
    fout << " =============== " << endl
          << "putID: " << data[i].putID << "\t"
          << "getID: " << data[i].getID << "\t"
          << "weight: " << data[i].weight << endl;
    */
    fout.write( (char*)&data[i], sizeof(student) );
  }  // for

  fout.close();
}  // Data::writeBinFIle()

// =============================== Data::showData() ================================ //
void Data::showData() {
  for ( int i = 0 ; i < data.size() ; i = i + 1 ) {
    cout << " =============== " << endl
          << "putID: " << data[i].putID << "\t"
          << "getID: " << data[i].getID << "\t"
          << "weight: " << data[i].weight << endl;
  }  // for

}  // Data::showData()

// ============================== Data::getFileName() ============================== //
string Data::getFileName() {
  return fileName;
}  // getFileName()

// ============================ Data::getTotalFileNum() ============================ //
int Data::getTotalFileNum() {
  return num;
}  // Data::getTotalFileNum()

// ========================== Data::getInternalSortTime() ========================== //
double Data::getInternalSortTime() {
  return internalTime;
}  // Data::getInternalSortTime()

/** class File **/
// ================================== File::File() ================================= //
File::File() {
  reset();
}  // File::File()

// ================================= File::reset() ================================= //
void File::reset() {
  readIndex = 0;
  fileData.clear();
}  // File::reset()

// ================================== File::~File() ================================= //
File::~File() {
  reset();
}  // File::File()

// ============================== File::getOneData() =============================== //
bool File::getFileData( string fileName ) {
  ifstream fin;
  bool successful = false;

  student tempS;

  fileData.erase( fileData.begin(), fileData.end() );

  fin.open( fileName, ios::in | ios::binary );

  if ( !fin.fail() ) {
    // move file pointer to the end of file
    fin.seekg( 0, fin.end );

    // use tellg() to get file size
    // divide by the size of structure student to get how many data need to be read
    int dataNum = fin.tellg() / sizeof(student);

    // move file pointer to the begin of file
    fin.seekg( 0, fin.beg );

    cout << endl << fileName << " readIndex = " << readIndex << " dataNum = " << dataNum << endl;

    fin.seekg( readIndex * sizeof(student) );

    if ( readIndex < dataNum ) {
      successful = true;
    }  // if

    for ( int i = readIndex + 1 ; i <= dataNum && i <= readIndex + 300 ; i = i + 1 ) {
      // read data ( structure )
      fin.read( (char*)&tempS, sizeof(tempS) );

      fileData.push_back(tempS);
    }  // for

  }  // else

  // showData();
  readIndex = readIndex + BUFFER_SIZE;
  fin.close();
  return successful;
}  // File::getFileData()

// ============================== File::getFileData() ============================== //
vector<student> File::getData() {
  return fileData;
}  // File::getData()

// ============================ File::deleteFirstItem() ============================ //
void File::deleteFirstItem() {
  fileData.erase( fileData.begin() );
}  // File::deleteFirstItem()

// ============================= File::getFirstItem() ============================== //
student File::getFirstItem() {
  return fileData[0];
}  // File::getFirstItem()

// ============================ File::fileDataIsEmpty() ============================ //
bool File::fileDataIsEmpty() {
  return fileData.empty();
}  // File::fileDataIsEmpty()

/** class ExSort **/
// =============================== ExSort::sortData() ============================== //
void ExSort::sortData( int fileNum, string fileName ) {
  int alreadyReadFile = 1;
  inFileNum = fileNum;
  outFileNum = 1;
  sortedData.clear();

  while ( inFileNum > 1 ) {
    cout << endl << "Now there are " << inFileNum << " runs." << endl;

    /*
    string temp;
    cin >> temp;
    */

    while ( alreadyReadFile <= inFileNum ) {
      File fileData1;
      File fileData2;
      string dataFile1;

      if ( alreadyReadFile == 1 )
        dataFile1 = "order" + fileName + ".bin";
      else
        dataFile1 = "order" + fileName + "_" + to_string( alreadyReadFile ) + ".bin";

      string dataFile2 = "order" + fileName + "_" + to_string( alreadyReadFile + 1 ) + ".bin";

      fileData1.getFileData( dataFile1 );
      fileData2.getFileData( dataFile2 );

      mergeData( fileData1, fileData2, dataFile1, dataFile2 );

      // cout << endl << "============================" << endl;

      // delete merged two file
      remove( dataFile1.c_str() );
      remove( dataFile2.c_str() );

      // write sorted file
      if ( outFileNum == 1 ) {
        writeFile( "order" + fileName + ".bin" );
      }  // if
      else {
        writeFile( "order" + fileName + "_" + to_string(outFileNum) + ".bin" );
      }  // else

      alreadyReadFile = alreadyReadFile + 2;
      outFileNum = outFileNum + 1;
    }  // while

    inFileNum = outFileNum - 1;
    outFileNum = 1;
    alreadyReadFile = 1;
  }  // while

  cout << endl << "Now there are " << inFileNum << " runs." << endl;
}  // ExSort::sortData()

// ============================== ExSort::mergeData() ============================== //
void ExSort::mergeData( File & data1, File & data2, string fileName1, string fileName2 ) {
  sortedData.clear();
  bool done = false;
  File notSortedFile;
  string notSortedFileName;

  while ( done == false ) {

    while ( !data1.fileDataIsEmpty() && !data2.fileDataIsEmpty() ) {
      if ( data1.getFirstItem().weight >= data2.getFirstItem().weight ) {
        sortedData.push_back( data1.getFirstItem() );
        data1.deleteFirstItem();
      }  // if
      else if ( data1.getFirstItem().weight < data2.getFirstItem().weight ) {
        sortedData.push_back( data2.getFirstItem() );
        data2.deleteFirstItem();
      }  // else

    }  // while

    // cout << "end merge" << endl;

    if ( data1.fileDataIsEmpty() && data1.getFileData( fileName1 ) == false ) {
      // cout << "data1 out" << endl;
      done = true;
      notSortedFile = data2;
      notSortedFileName = fileName2;
    }  // if

    if ( data2.fileDataIsEmpty() && data2.getFileData( fileName2 ) == false ) {
      // cout << "data2 out" << endl;
      done = true;
      notSortedFile = data1;
      notSortedFileName = fileName1;
    }  // if

  }  // while

    do {
      while ( !notSortedFile.fileDataIsEmpty() ) {
        sortedData.push_back( notSortedFile.getFirstItem() );
        notSortedFile.deleteFirstItem();
      }  // if

    }  while ( notSortedFile.getFileData( notSortedFileName ) == true );

    cout << "sorted data size = " << sortedData.size() << endl;
}  // ExSort::mergeData()

// ============================= ExSort::writeFile() ============================== //
void ExSort::writeFile( string fileName ) {
  ofstream fout;

  // open / create output file
  fout.open( fileName, ios::out | ios::binary | ios::app );

  int dataSize = sortedData.size();

  // output data into binary file one by one ( structure )
  for ( int i = 0 ; i < sortedData.size() ; i = i + 1 ) {
    fout.write( (char*)&sortedData[i], sizeof(student) );
  }  // for

  fout.close();
}  // Data::writeBinFIle()


/** class Hash **/
// ================================= Hash::Hash() ================================== //
Hash::Hash() {
  table = new item[HASHING_SIZE];
  totalItemNum = 0;
}  // Hash::Hash()

// ================================= Hash::~Hash() ================================= //
Hash::~Hash() {
  delete[] table;
  totalItemNum = 0;
}  // Hash::~Hash()

// ================================= Hash::reset() ================================= //
void Hash::reset() {
  delete[] table;
  table = new item[HASHING_SIZE];
  totalItemNum = 0;
}  // Hash::reset();

// =============================== Hash::buildHash() =============================== //
void Hash::buildHash( string fileName ) {
  ifstream fin;
  reset();

  student tempS;

  fin.open( "order" + fileName + ".bin", ios::in | ios::binary );

  if ( !fin.fail() ) {
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

      insertData( tempS.weight, i );
    }  // for

  }  // else

  fin.close();
}  // Hash::buildHash()

// =============================== Hash::insertData() ============================== //
void Hash::insertData( float weight, int offset ) {
  int hvalue = hashing(weight);
  if ( table[hvalue].hvalue == -1 ) {
    totalItemNum = totalItemNum + 1;
    cout << "[" << totalItemNum << "]" << " (" << weight << ", " << offset << ")" << endl;

    table[hvalue].hvalue = hvalue;
    table[hvalue].label.first = weight;
    table[hvalue].label.second = offset;
  }  // if

}  // Hash::insertData()

// ================================ Hash::hashing() ================================ //
int Hash::hashing( float weight ) {
  return ( int ) ( weight * 100 * 2 + 3 ) % HASHING_SIZE;
}  // Hash::hashing()
