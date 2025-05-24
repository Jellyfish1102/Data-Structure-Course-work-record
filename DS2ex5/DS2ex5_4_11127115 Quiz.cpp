// 11127115 ³¯«º¦w
# include <iostream>
# include <fstream>
# include <stdio.h>
# include <sstream>
# include <algorithm>
# include <string>
# include <vector>
# include <map>
# include <chrono>

# define HASHING_SIZE 151

using namespace std;
using namespace chrono;

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
  int bufferSize;
  double internalTime;

  vector<student> data;

  // perform internal sort of input data
  void internalSort();

  // write sorted data into binary file
  void writeBinFile();

  // reset num, internalTime
  void reset();

  public:
    Data();
    ~Data();

    // read data from binary file
    bool readBinFile();

    // return fileName
    string getFileName();

    // return total file number just write
    int getTotalFileNum();

    // return internal sort executed time
    double getInternalSortTime();

    void setBufferSize();
};  // class Data

class ExSort {
  int inFileNum;
  int outFileNum;
  double externalTime;
  vector<student> sortedData;

  // merge two file into one with 2-way merge
  void mergeData( string fileName1, string fileName2 );

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
    double weight;
    int offset;
    int hvalue;

    item(): weight(-1), offset(-1), hvalue(-1) {}
  };  // item

  map<float, int, greater<float> > mainIndex;
  int totalItemNum;

  // put data into hash table
  void insertData( float weight, int offset );

  // initial table, totalItemNum
  void reset();

  void processSearch( float threshold, string fileName );

  public:
    Hash();
    ~Hash();
    void buildHash( string fileName );
    void searching( string fileName );
};  // class BPlusTree

int main() {
  Data data;
  ExSort exSort;
  Hash hashing;
  int bufferSize;
  string command = "";

  while ( command != "0" ) {
    data.setBufferSize( );

    cout << endl << "***********************************************";
    cout << endl << " On-machine Exercise 05                       *";
    cout << endl << " Mission 1: External Merge Sort on a Big File *";
    cout << endl << " Mission 2: Construction of Primary Index     *";
    cout << endl << " Mission 3: Range Search by Primary Index     *";
    cout << endl << "***********************************************";
    cout << endl << "########################################################";
    cout << endl << "Mission 1: External merge sort ";
    cout << endl << "########################################################";

    cout << endl;
    if ( data.readBinFile() ) {
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

      cout << endl;
      cout << endl << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
      cout << "Mission 3: Threshold search on primary index" << endl;
      cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;

      hashing.searching( data.getFileName() );
    }  // if

    cout << endl << "[0]Quit or [Any other key]continue?" << endl;
    cin >> command;
  }  // while

  return 0;
}  // main()

/** class Data **/
// ================================== Data::Data() ================================= //
Data::Data() {
  bufferSize = 300;
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
bool Data::readBinFile() {
  FILE * fp;
  bool successful = false;
  reset();

  student tempS;
  string name;

  data.erase( data.begin(), data.end() );

  do {
    cout << endl << "Input the file number: [0]Quit " << endl;
    cin >> fileName;
    name = "pairs" + fileName + ".bin";
    fp = fopen( name.c_str(), "rb" );

    if ( fp == NULL && fileName != "0" ) {
      cout << endl << "pairs" << fileName << ".bin does not exist!!!" << endl;
    }  // if

  } while ( fp == NULL && fileName != "0" );


  auto start = steady_clock::now();
  if ( fileName != "0" ) {
    successful = true;

    // move file pointer to the end of file
    fseek( fp, 0, SEEK_END );

    // use tellg() to get file size
    // divide by the size of structure student to get how many data need to be read
    int dataNum = ftell(fp) / sizeof(student);

    // move file pointer to the begin of file
    fseek( fp, 0, SEEK_SET );

    for ( int i = 1 ; i <= dataNum ; i = i + 1 ) {
      // read data( structure )
      fread( (char*)&tempS, sizeof(tempS), 1, fp );

      data.push_back(tempS);

      if ( i % bufferSize == 0 ) {
        internalSort();
        data.clear();
      }  // if

    }  // for

    internalSort();
  }  // else

  // showData();

  fclose(fp);
  auto duration = steady_clock::now() - start;
  internalTime = internalTime + (double) duration_cast<std::chrono::microseconds>(duration).count() / 1000;
  return successful;
}  // Data::readBinFile()

// ============================= Data::internalSort() ============================== //
void Data::internalSort() {
  stable_sort( data.begin(), data.end(), greater<student>() );

  writeBinFile();
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

  FILE * fp;
  fp = fopen( name.c_str(), "wb" );
  //cout << num << " data size = " << data.size() << endl;

  // output data into binary file one by one ( structure )
  fwrite( &data[0], sizeof(student), data.size(), fp );
  fclose(fp);
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

// ============================= Data::setBufferSize() ============================= //
void Data::setBufferSize( ) {
  string buffer_s;
  bool allInterger = true;
  cout << endl << "*** The buffer size is " << bufferSize;

  do {
    allInterger = true;
    if ( bufferSize > 60000 ) {
      cout << endl << "### It is NOT in [1,60000] ###" << endl;
    }  // if

    cout << endl << "Input a number in [1, 60000]: ";
    cin >> buffer_s;

    for ( int i = 0 ; i < buffer_s.length() ; i = i + 1 ) {
      if ( (int) buffer_s[i] < 48 || (int) buffer_s[i] > 57 )
        allInterger = false;
    }  // for

    if ( allInterger )
      bufferSize = stoi(buffer_s);

  } while ( bufferSize < 1 || bufferSize > 60000 || allInterger == false );

}  // Data::setBufferSize()

/** class ExSort **/
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

      //  cout << endl << "============================" << endl;

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

  FILE * fp1;
  FILE * fp2;
  int offset1 = 0;
  int dataNum1 = 0;
  int offset2 = 0;
  int dataNum2 = 0;
  student st1;
  student st2;

  auto start = steady_clock::now();
  // check there there are such file named fileName1/ fileName2
  fp1 = fopen( fileName1.c_str(), "rb" );
  if ( fp1 != NULL ) {
    fseek( fp1, 0, SEEK_END );
    dataNum1 = ftell(fp1) / sizeof(student);
    fseek( fp1, 0, SEEK_SET );

    fread( (char*)&st1, sizeof(st1), 1, fp1 );
  }  // if

  fp2 = fopen( fileName2.c_str(), "rb" );
  if ( fp2 != NULL ) {
    fseek( fp2, 0, SEEK_END );
    dataNum2 = ftell(fp2) / sizeof(student);
    fseek( fp2, 0, SEEK_SET );

    fread( (char*)&st2, sizeof(st2), 1, fp2 );
  }  // if


  // do merge sort (stable)
  while ( offset1 < dataNum1 && offset2 < dataNum2 ) {
    if ( st1.weight >= st2.weight ) {
      sortedData.push_back( st1 );
      fread( (char*)&st1, sizeof(st1), 1, fp1 );
      offset1 = offset1 + 1;
    }  // if
    else {
      sortedData.push_back( st2 );
      fread( (char*)&st2, sizeof(st2), 1, fp2 );
      offset2 = offset2 + 1;
    }  // else

  }  // while

  // put unread data from file and put them into sortedData
  if ( offset1 != dataNum1 ) {
    sortedData.push_back( st1 );
    for ( int i = offset1 + 1 ; i < dataNum1 ; i = i + 1 ) {
      fread( (char*)&st1, sizeof(st1), 1, fp1 );
      sortedData.push_back( st1 );
    }  // for

  }  // if
  else {
    sortedData.push_back( st2 );
    for ( int i = offset2 + 1 ; i < dataNum2 ; i = i + 1 ) {
      fread( (char*)&st2, sizeof(st2), 1, fp2 );
      sortedData.push_back( st2 );
    }  // for

  }  // else

  // close file
  fclose( fp1 );
  fclose( fp2 );
  auto duration = steady_clock::now() - start;
  externalTime = externalTime + (double) duration_cast<std::chrono::microseconds>(duration).count() / 1000;

  // cout << "sorted data size = " << sortedData.size() << endl;
}  // ExSort::mergeData()

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

  auto start = steady_clock::now();
  FILE * fp;

  // open / create output file
  fp = fopen( outFileName.c_str(), "wb" );

  // output data into binary file ( structure )
  fwrite( &sortedData[0], sizeof(student), sortedData.size(), fp );

  fclose(fp);
  auto duration = steady_clock::now() - start;
  externalTime = externalTime + (double) duration_cast<std::chrono::microseconds>(duration).count() / 1000;
}  // Data::writeBinFIle()

// ========================= ExSort::getExternalSortTime() ========================= //
double ExSort::getExternalSortTime() {
  return externalTime;
}  // ExSort::getInternalSortTime()

/** class Hash **/
// ================================= Hash::Hash() ================================== //
Hash::Hash() {
  reset();
}  // Hash::Hash()

// ================================= Hash::~Hash() ================================= //
Hash::~Hash() {
  reset();
}  // Hash::~Hash()

// ================================= Hash::reset() ================================= //
void Hash::reset() {
  mainIndex.clear();
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

  auto checkPair = mainIndex.find( weight );

  if ( checkPair == mainIndex.end() ) {
    mainIndex.insert( pair< float, int> ( weight, offset ) );

    totalItemNum = totalItemNum + 1;
    cout << "[" << totalItemNum << "]" << " (" << weight << ", " << offset << ")" << endl;
  }  // if

}  // Hash::insertData()

// =============================== Hash::searching() =============================== //
void Hash::searching( string fileName ) {
  float threshold;
  string command;

  do {
    cout << endl << "Please input a threshold in the range [0,1]: " << endl;
    cin >> threshold;

    if ( threshold <= 0 || threshold > 1 ) {
      cout << endl << "The threshold is not in the range [0,1]!" << endl;
    }  // if
    else {
      processSearch( threshold, fileName );
    }  // else

    cout << endl << "[0]Quit or [Any other key]continue?" << endl;
    cin >> command;
  }  while ( command != "0" );

}  // Hash::searching()

// ============================= Hash::processSearch() ============================= //
void Hash::processSearch( float threshold, string fileName ) {
  vector<student> searched;
  vector<int> offsets;
  student tempS;
  string rName = "order" + fileName + ".bin";
  FILE * fpr = fopen( rName.c_str(), "rb" );

  auto start = steady_clock::now();
  for ( auto it = mainIndex.begin() ; it != mainIndex.end() && ( it -> first ) >= threshold ; it ++  ) {
    fseek( fpr, it -> second * sizeof(student), SEEK_SET );
    fread( (char*)&tempS, sizeof(tempS), 1, fpr );
    searched.push_back(tempS);
    offsets.push_back(it -> second);
  }  // for

  int dataNum;
  if ( threshold <= 0.01 ) {
    fseek( fpr, 0, SEEK_SET );
    fseek( fpr, 0, SEEK_END );
    dataNum = ftell(fpr) / sizeof(student);
  }  // if
  else {
    auto it = mainIndex.find( threshold - 0.01 );
    dataNum = it -> second;
  }  // else

  auto duration = steady_clock::now() - start;
  double searchTime = (double) duration_cast<std::chrono::microseconds>(duration).count() / 1000;

  ofstream fout("order" + fileName + ".txt");
  fout << "Search " << threshold << ": " << dataNum << " records, " << searchTime << " ms";
  for ( int i = 0 ; i < searched.size() ; i = i + 1 ) {
    fout << endl
         << "[" << offsets.at(i) + 1 << "]" << " "
         << searched.at(i).putID << " "
         << searched.at(i).getID << " "
         << searched.at(i).weight;
  }  // for

  fclose(fpr);
  fout.close();
}  // Hash::processSearch()
