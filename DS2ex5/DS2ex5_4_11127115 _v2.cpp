// 11127115 ³¯«º¦w
# include <iostream>
# include <fstream>
# include <stdio.h>
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
  int outFileNum;
  int readIndex;
  double internalTime;

  vector<student> data;

  // read data from binary file
  bool readBinFile( FILE * fp, string name );

  // write sorted data into binary file
  void writeBinFile();

  // reset num, internalTime
  void reset();

  public:
    Data();
    ~Data();

    // perform internal sort of input data
    void internalSort();

    // return fileName
    string getFileName();

    // return total file number just write
    int getTotalFileNum();

    // return internal sort executed time
    double getInternalSortTime();
};  // class Data

class ExSort {
  struct file {
    vector<student> data;
    int readIndex;

    file(): readIndex(0) {}
  };  // file

  int inFileNum;
  int outFileNum;
  double externalTime;
  vector<student> sortedData;

  // get data from binary file
  bool getFileData( file & fileData, string fileName );

  // merge two file into one with 2-way merge
  void mergeData( string fileName1, string fileName2 );

  void processUnsorted( file & fileData, string fileName );

  // write data into binary file
  void writeFile( string fileName );

  public:
    // execute external sort
    void sortData( int fileNum, string fileName );

    // return external sort executed time
    double getExternalSortTime();
};  // class Sort

class Hash {
  struct item {
    float weight;
    int offset;
    int hvalue;

    item(): weight(-1), offset(-1), hvalue(-1) {}
  };  // item

  item* table;
  int totalItemNum;

  // put data into hash table
  void insertData( float weight, int offset );

  // calculate hashing value
  int hashing( float weight );

  // initial table, totalItemNum
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
    data.internalSort();

    cout << endl << "The internal sort is completed. Check the initial sorted runs!" << endl;

    exSort.sortData( data.getTotalFileNum(), data.getFileName() );

    cout << endl << "The execution time ..." << endl;
    cout << "Internal Sort = " << data.getInternalSortTime() << " ms" << endl;
    cout << "External Sort = " << exSort.getExternalSortTime() << " ms" << endl;
    cout << "Total Execution Time = " << data.getInternalSortTime() + exSort.getExternalSortTime() << " ms" << endl;

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
  outFileNum = 0;
  readIndex = 0;
  internalTime = 0;
  data.clear();
}  // Data::reset()

// ================================== Data::~Data() ================================= //
Data::~Data() {
  reset();
}  // Data::Data()

// ============================== Data::readBinFile() ============================== //
bool Data::readBinFile( FILE * fp, string name ) {
  bool successful = false;
  student tempS;
  data.clear();

  double start = clock();
  fp = fopen( name.c_str(), "rb" );
  if ( fp != NULL && fileName != "0" ) {

    // move file pointer to the end of file
    fseek( fp, 0, SEEK_END );

    // use tellg() to get file size
    // divide by the size of structure student to get how many data need to be read
    int dataNum = ftell(fp) / sizeof(student);

    // move file pointer to the begin of file
    fseek( fp, readIndex * sizeof(student), SEEK_SET );

    if ( readIndex < dataNum ) {
      successful = true;
    }  // if

    for ( int i = readIndex + 1 ; i <= dataNum && i <= readIndex + 300; i = i + 1 ) {
      // read data( structure )
      fread( (char*)& tempS, sizeof( tempS ), 1, fp );

      data.push_back(tempS);
    }  // for

  }  // else

  // showData();

  fclose(fp);
  double ends = clock();

  if ( successful == true ) {
    readIndex = readIndex + BUFFER_SIZE;
    internalTime = internalTime + ends - start;
  }  // if

  return successful;
}  // Data::readBinFile()

// ============================= Data::internalSort() ============================== //
void Data::internalSort() {
  FILE * fp;
  string name;
  bool haveFile = false;
  reset();

  do {
    cout << endl << "Input the file number: [0]Quit " << endl;
    cin >> fileName;

    name = "pairs" + fileName + ".bin";
    haveFile = readBinFile( fp, name );

    if ( haveFile == false && fileName != "0" ) {
      cout << endl << "pairs" << fileName << ".bin does not exist!!!" << endl;
    }  // if

  } while ( haveFile == false && fileName != "0" );

  if ( fileName != "0" ) {
    do  {
      stable_sort( data.begin(), data.end(), greater<student>() );
      writeBinFile();
    }  while( readBinFile( fp, name ) );

  }  // if

}  // internalSort()

// ============================= Data::writeBinFile() ============================== //
void Data::writeBinFile() {
  // open / create output file
  outFileNum = outFileNum + 1;
  string name;
  if ( outFileNum == 1 )
    name = "order" + fileName + ".bin";
  else
    name = "order" + fileName + "_" + to_string(outFileNum) + ".bin";

  double start = clock();
  FILE * fp;
  fp = fopen( name.c_str(), "wb" );
  //cout << num << " data size = " << data.size() << endl;

  int dataSize = data.size();

  // output data into binary file one by one ( structure )
  for ( int i = 0 ; i < data.size() ; i = i + 1 ) {
    fwrite( (char*)&data[i], sizeof(student), 1, fp );
  }  // for

  fclose(fp);
  double ends = clock();
  internalTime = internalTime + ends - start;
}  // Data::writeBinFIle()

// ============================== Data::getFileName() ============================== //
string Data::getFileName() {
  return fileName;
}  // getFileName()

// ============================ Data::getTotalFileNum() ============================ //
int Data::getTotalFileNum() {
  return outFileNum;
}  // Data::getTotalFileNum()

// ========================== Data::getInternalSortTime() ========================== //
double Data::getInternalSortTime() {
  return internalTime;
}  // Data::getInternalSortTime()

/** class ExSort **/
// ============================= ExSort::getOneData() ============================== //
bool ExSort::getFileData( file & fileData, string fileName ) {
  FILE * fp;
  student tempS;
  bool successful = false;

  fileData.data.clear();

  fp = fopen( fileName.c_str(), "rb" );

  if ( fp != NULL ) {
    // move file pointer to the end of file
    fseek( fp, 0, SEEK_END );

    // use ftell() to get file size
    // divide by the size of structure student to get how many data need to be read
    int dataNum = ftell(fp) / sizeof(student);

    // cout << endl << fileName << " readIndex = " << fileData.readIndex << " dataNum = " << dataNum << endl;

    // move file pointer to the location read next
    fseek( fp, fileData.readIndex * sizeof(student), SEEK_SET );

    // check there still are data to read
    if ( fileData.readIndex < dataNum ) {
      successful = true;
    }  // if

    for ( int i = fileData.readIndex + 1 ; i <= dataNum && i <= fileData.readIndex + 300 ; i = i + 1 ) {
      // read data ( structure )
      fread( (char*)&tempS, sizeof(tempS), 1, fp );

      fileData.data.push_back(tempS);
    }  // for

  }  // else

  fileData.readIndex = fileData.readIndex + BUFFER_SIZE;
  fclose(fp);
  return successful;
}  // ExSort::getFileData()

// =============================== ExSort::sortData() ============================== //
void ExSort::sortData( int fileNum, string fileName ) {
  int alreadyReadFile = 1;
  inFileNum = fileNum;
  outFileNum = 1;
  externalTime = 0;
  sortedData.clear();

  // execute until all files are merge into one
  while ( inFileNum > 1 ) {
    cout << endl << "Now there are " << inFileNum << " runs." << endl;

    // merge file
    while ( alreadyReadFile <= inFileNum ) {
      string dataFile1;

      // determine read file name
      if ( alreadyReadFile == 1 )
        dataFile1 = "order" + fileName + ".bin";
      else
        dataFile1 = "order" + fileName + "_" + to_string( alreadyReadFile ) + ".bin";

      string dataFile2 = "order" + fileName + "_" + to_string( alreadyReadFile + 1 ) + ".bin";

      // read and merge file
      mergeData( dataFile1, dataFile2 );

      // cout << endl << "============================" << endl;

      // delete merged two file
      remove( dataFile1.c_str() );
      remove( dataFile2.c_str() );

      // write sorted file
      writeFile( fileName );

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
void ExSort::mergeData( string fileName1, string fileName2 ) {
  sortedData.clear();
  bool done = false;
  string notSortedFileName;

  file data1;
  file data2;
  file notSortedFile;

  double start = clock();
  // read data from file ( 300 )
  getFileData( data1, fileName1 );
  getFileData( data2, fileName2 );

  while ( done == false ) {

    // if there still have data in both vector -> merge sort
    while ( !data1.data.empty() && !data2.data.empty() ) {
      if ( data1.data[0].weight >= data2.data[0].weight ) {
        sortedData.push_back( data1.data[0] );
        data1.data.erase( data1.data.begin() );
      }  // if
      else {
        sortedData.push_back( data2.data[0] );
        data2.data.erase( data2.data.begin() );
      }  // else

    }  // while

    if ( data1.data.empty() && getFileData( data1, fileName1 ) == false ) {
      // cout << "data1 out" << endl;
      done = true;
      processUnsorted( data2, fileName2 );
    }  // if

    if ( data2.data.empty() && getFileData( data2, fileName2 ) == false ) {
      // cout << "data2 out" << endl;
      done = true;
      processUnsorted( data1, fileName1 );
    }  // if

  }  // while

  double ends = clock();
  externalTime = externalTime + ends - start;

  // cout << "sorted data size = " << sortedData.size() << endl;
}  // ExSort::mergeData()

// =========================== ExSort::processUnsorted() =========================== //
void ExSort::processUnsorted( file & fileData, string fileName ) {
  do {
    while ( !fileData.data.empty() ) {
      sortedData.push_back( fileData.data[0] );
      fileData.data.erase( fileData.data.begin() );
    }  // if

  }  while ( getFileData( fileData, fileName )  == true );

}  // ExSort::processUnsorted()

// ============================== ExSort::writeFile() ============================== //
void ExSort::writeFile( string fileName ) {
  // determine write file name
  string outFileName;
  if ( outFileNum == 1 ) {
    outFileName = "order" + fileName + ".bin";
  }  // if
  else {
    outFileName = "order" + fileName + "_" + to_string(outFileNum) + ".bin";
  }  // else

  double start = clock();
  FILE * fp;

  // open / create output file
  fp = fopen( outFileName.c_str(), "wb" );

  // output data into binary file one by one ( structure )
  for ( int i = 0 ; i < sortedData.size() ; i = i + 1 ) {
    fwrite( (char*)&sortedData[i], sizeof(student), 1, fp );
  }  // for

  fclose(fp);
  double ends = clock();
  externalTime = externalTime + ends - start;
}  // Data::writeBinFIle()

// ========================= ExSort::getExternalSortTime() ========================= //
double ExSort::getExternalSortTime() {
  return externalTime;
}  // ExSort::getInternalSortTime()

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
  FILE * fp;
  reset();

  student tempS;

  string name = "order" + fileName + ".bin";
  fp = fopen( name.c_str(), "rb" );

  if ( fp != NULL ) {
    // move file pointer to the end of file
    fseek( fp, 0, SEEK_END );

    // use tellg() to get file size
    // divide by the size of structure student to get how many data need to be read
    int dataNum = ftell(fp) / sizeof(student);

    // move file pointer to the begin of file
    fseek( fp, 0, SEEK_SET );

    for ( int i = 0 ; i < dataNum ; i = i + 1 ) {
      // read data( structure )
      fread( (char*)&tempS, sizeof(tempS), 1, fp );

      insertData( tempS.weight, i );
    }  // for

  }  // else

  fclose(fp);
}  // Hash::buildHash()

// =============================== Hash::insertData() ============================== //
void Hash::insertData( float weight, int offset ) {
  int hvalue = hashing(weight);

  // check current data is the first offset of current weight
  if ( table[hvalue].hvalue == -1 ) {
    totalItemNum = totalItemNum + 1;
    cout << "[" << totalItemNum << "]" << " (" << weight << ", " << offset << ")" << endl;

    table[hvalue].hvalue = hvalue;
    table[hvalue].weight = weight;
    table[hvalue].offset = offset;
  }  // if

}  // Hash::insertData()

// ================================ Hash::hashing() ================================ //
int Hash::hashing( float weight ) {
  return ( int ) ( weight * 100 * 2 + 3 ) % HASHING_SIZE;
}  // Hash::hashing()
