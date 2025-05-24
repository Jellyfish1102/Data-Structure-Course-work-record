// 11127115 陳姿安, 11127117 芦添佑
# include <iostream>
# include <fstream>
# include <sstream>
# include <cstring>
# include <string>
# include <vector>
# include <iomanip>  // setprecision

using namespace std;

struct student {
  char sid[10];
  char sname[10];
  unsigned char sscore[6];
  float mean;
};  // student

class Data {
  private:
    string fileName;
    vector<student> data;

    // transform text file to binary file
    void txtToBin( vector<student> data );

    // show data content
    void showData( vector<student> data );

  public:
    bool readBinFile();
    bool readTxtFile();
    bool isEmpty();
    vector<student> getData();
    string getFileName();
};  // class Data

class Hashing {
  private:
    struct item {
      char sid[10];
      char sname[10];
      float mean;
      int hvalue;

      // constructor of structure
      item():sid("\0"), sname("\0"), mean(0), hvalue(-1) {}
    };

    int tablesize;
    int probNum;

    // check num is prime number or not
    bool isPrime( int num );

    // get the size of hashing table
    int getTableSize( vector<student> data );

    // get the hash value
    int hashing( char id[10] );

    // get the step size of item in double hashing
    int step( int dataSize, char id[10] );

    // get the maximum step size of item in double hashing
    int maximumStep( int dataSize );

    // display items of hashing table on the screen
    void showTable( item* table );

    // display quadratic probing successful/unsuccessful search average comparisons on screen
    void showQuadraticProbing( int dataSize, item* table );

    // display double hashing successful/unsuccessful search average comparisons on screen
    void showDoubleHashing( int dataSize );

    // put item into hashing table
    void putInHashingTable( item & table, student data, int hvalue );

    // output hashing table to text file
    void outputFile( string method, string fileName, item* table );

  public:
    void quadraticProbing( vector<student> data, string fileName );
    void doubleHash( vector<student> data, string fileName );
};

int main() {
  string command = "";
  bool done = false;
  Data data;
  Hashing hashing;

  while ( done == false ) {
    cout << endl << "******* Hash Table *****";
    cout << endl << "* 0. QUIT              *";
    cout << endl << "* 1. Quadratic probing *";
    cout << endl << "* 2. Double hashing    *";
    cout << endl << "************************";
    cout << endl << "Input a choice(0, 1, 2): ";

    cin >> command;

    if ( command == "1" ) {
      if ( data.readBinFile() ) {
        hashing.quadraticProbing( data.getData(), data.getFileName() );
      }  // if

    }  // if
    else if ( command == "2" ) {
      if ( data.isEmpty() == false) {
        hashing.doubleHash( data.getData(), data.getFileName() );
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
bool Data::readBinFile() {
  fstream fin;
  bool success = false;
  bool haveBin = false;
  data.erase( data.begin(), data.end() );

  cout << endl << "Input a file number ([0] Quit) : ";
  cin >> fileName;

  if ( fileName != "0" ) {
    fin.open( "input" + fileName + ".bin", ios::in | ios::binary );

    // if there is no binary file -> read text file and transform to binary
    if ( fin.fail() ) {
      cout << endl << "### input" << fileName << ".bin does not exist! ###" << endl;

      if ( readTxtFile() ) {
        fin.open( "input" + fileName + ".bin", ios::in | ios::binary );
        if ( !fin.fail() )
          haveBin = true;
      }  // if

    }  // if
    else {
      haveBin = true;
    }  // else

    if ( haveBin == true ) {
      success = true;
      student tempS;

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

    }  // if

  }  // if

  fin.close();
  return success;
}  // Data::readBinFile()

// ============================== Data::readTxtFile() ============================== //
bool Data::readTxtFile() {
  ifstream fin;
  bool success = false;
  vector<student> studentData;

  fin.open( "input" + fileName + ".txt" );

  if ( !fin.fail() ) {
    string temp;

    // get a whole line in the file until end of file
    while ( getline( fin, temp ) ) {
      success = true;

      istringstream line(temp);  // get string from temp
      string item;
      student tempS;

      // get item from line ( separate by tab )
      for ( int k = 1 ; getline( line, item, '\t' ) ; k = k + 1 ) {
        // id
        if ( k % 9 == 1 ) {
          strcpy( tempS.sid, item.c_str() );
        }  // if

        // name
        else if ( k % 9 == 2 ) {
          strcpy( tempS.sname, item.c_str() );
        }  // else if

        // score * 6
        else if ( k % 9 >= 3 && k % 9 <= 8) {
          tempS.sscore[k%9-3] = (unsigned char) stoi(item);
        }  // else if

        // mean(average score)
        else if ( k % 9 == 0 ) {
          tempS.mean = stof( item );

          // push current data into student data
          studentData.push_back( tempS );
        }  // else if

      }  // for

    }  // while

    // out put binary file
    txtToBin(studentData);
  }  // if
  else {
    cout << endl << "### input" << fileName << ".txt does not exist! ###" << endl;
  }  // else

  fin.close();
  return success;
}  // Data::readTxtFile()

// =============================== Data::txtToBin() ================================ //
void Data::txtToBin( vector<student> data ) {
  ofstream fout;

  // open / create output file
  fout.open("input" + fileName + ".bin", ios::out | ios::binary );

  int dataSize = data.size();

  // output datas into binary file one by one ( structure )
  for ( int i = 0 ; i < data.size() ; i = i + 1 ) {
    fout.write( (char*)&data[i], sizeof(student) );
  }  // for

  fout.close();
}  // Data::txtToBin()

// ================================ Data::isEmpty() ================================ //
bool Data::isEmpty() {
  return data.empty();
}  // Data::isEmpty()

// ================================ Data::getData() ================================ //
vector<student> Data::getData() {
  return data;
}  // Data::getData()

// ============================== Data::getFileName() ============================== //
string Data::getFileName() {
  return fileName;
}  // Data::getData()

// ================================ Data::isEmpty() ================================ //
void Data::showData( vector<student> data ) {
  cout << data.size() << endl;

  for ( int i = 0 ; i < data.size() ; i = i + 1 ) {
    cout << " =============== " << endl
          << "sid: " << data[i].sid << endl
          << "sname: " << data[i].sname << endl
          << "sscore:" << endl
          << data[i].sscore[0] << "\t" << data[i].sscore[1] << "\t" << data[i].sscore[2] << "\t"
          << data[i].sscore[3] << "\t" << data[i].sscore[4] << "\t" << data[i].sscore[5] << endl
          << "mean: " << data[i].mean << endl;
  }  // for

}  // Data::showData()


/** class Hashing **/
// ========================== Hashing::quadraticProbing() ========================== //
void Hashing::quadraticProbing( vector<student> data, string fileName ) {
  tablesize = getTableSize( data );
  probNum = 0;

  item *table = new item[tablesize];

  for ( int i = 0 ; i < data.size() ; i = i + 1 ) {
    int hvalue = hashing(data[i].sid);
    int location = hvalue;
    int quadraticNum = 0;
    int prob = 1;
    bool done = false;

    do {
      // cout << endl << "hvalue = " << hvalue << " location = " << location << endl;

      // current location is empty -> put item in
      if ( table[location].hvalue == -1 ) {
        done = true;
        putInHashingTable( table[location], data[i], hvalue );
      }  // if

      // find next location ( use quadratic probing )
      else {
        prob = prob + 1;
        quadraticNum = quadraticNum + 1;
        location = ( hvalue + quadraticNum * quadraticNum ) % tablesize;

        /*
        // make sure endless loop don't happen
        if ( location == hvalue ) {
          cout << "no space ! [" << i << "]" << endl;
          prob = 0;
          done = true;
        }  // if
        */

      }  // else

    } while( done == false );

    // calculate probing time
    probNum = probNum + prob;
    // cout << "===============================" << endl;
  }  // for

  // display result on screen
  showQuadraticProbing( data.size(), table );

  // write hashing table into file
  outputFile( "Quadratic probing", fileName, table );

  delete[] table;
}  // Hashing::quadraticProbing()

// ============================= Hashing::doubleHash() ============================= //
void Hashing::doubleHash( vector<student> data, string fileName ) {
  probNum = 0;

  item * table = new item[tablesize];

  for ( int i = 0 ; i < data.size() ; i = i + 1 ) {
    int hvalue = hashing( data[i].sid );
    int location = hvalue;
    int prob = 1;
    bool done = false;

    // calculate how many steps are needed to find next location
    int increaseStep = step( data.size(), data[i].sid );

    do {
      // current location is empty -> put item in
      if ( table[location].hvalue == -1 ) {
        done = true;
        putInHashingTable( table[location], data[i], hvalue );
      }  // if

      // find next location
      else {
        prob = prob + 1;
        location = ( location + increaseStep ) % tablesize;
      }  // else

    } while( done == false );

    // calculate probing time
    probNum = probNum + prob;
  }  // for

  // display result on screen
  showDoubleHashing( data.size() );

  // write hashing table into file
  outputFile( "Double hashing", fileName, table );

  delete[] table;
}  // Hashing::doubleHash()

// ========================== Hashing::putInHashingTable() ========================= //
void Hashing::putInHashingTable( item & table, student data, int hvalue ) {
  strcpy(table.sid, data.sid);
  strcpy(table.sname, data.sname);
  table.mean = data.mean;
  table.hvalue = hvalue;
}  // Hashing::putInHashingTable()

// =============================== Hashing::isPrime() ============================== //
bool Hashing::isPrime( int num ) {
  bool isPrime = true;

  for ( int i = 2 ; i < num && isPrime == true ; i = i + 1 ) {
    if ( num % i == 0 ) {
      isPrime = false;
    }  // if

  }  // for

  return isPrime;
}  // Hashing::isPrime()

// ============================ Hashing::getTableSize() ============================ //
int Hashing::getTableSize( vector<student> data ) {
  int tableSize = 1.15 * data.size();

  while( isPrime( tableSize ) == false ) {
    tableSize = tableSize + 1;
  }  // while

  return tableSize;
}  // Hashing::getTableSize()

// ============================== Hashing::hashing() =============================== //
int Hashing::hashing( char id[10] ) {
  int key = 1;

  for ( int i = 0 ; i < 10 && (int) id[i] != 0 ; i = i + 1 ) {
    key = key * (int) id[i] % tablesize;
  }  // for

  return key;
}  // Hashing::hashing()

// ============================ Hashing::maximumStep() ============================= //
int Hashing::maximumStep( int dataSize ) {
  int maximum_step = dataSize / 5 + 1;

  while ( isPrime( maximum_step ) == false ) {
    maximum_step = maximum_step + 1;
  }  // while

  return maximum_step;
}  // Hashing::maximumStep()

// ================================ Hashing::step() ================================ //
int Hashing::step( int dataSize, char id[10] ) {
  int maxStep = maximumStep( dataSize );
  int key = 1;

  for ( int i = 0 ; i < 10 && (int) id[i] != 0 ; i = i + 1 ) {
    key = key * id[i] % maxStep;
  }  // for

  return maxStep - key;
}  // Hashing::step()

// ============================= Hashing::showTable() ============================== //
void Hashing::showTable( item* table ) {
  for ( int i = 0 ; i < tablesize ; i = i + 1 ) {
    cout << "===============" << endl
          << "[" << i << "]\t"
          << table[i].sid << " " << table[i].sname << " " << table[i].mean << " " << table[i].hvalue << endl;
  }  // for
}  // Hashing::showTable()

// ======================= Hashing::showQuadraticProbing() ========================= //
void Hashing::showQuadraticProbing( int dataSize, item * table ) {
  float unseccessful = 0;

  // calculate the average comparisons of unsuccessful search
  for ( int location = 0 ; location < tablesize ; location = location + 1 ) {
    int searching = location;
    int quadratic = 1;
    while ( table[searching].hvalue != -1 ) {
      unseccessful = unseccessful + 1;
      searching = ( location + quadratic * quadratic ) % tablesize;
      quadratic = quadratic + 1;

      if ( searching == location ) {
        unseccessful = 0;
        break;
      }  // if

    }  // while

  }  // for

  unseccessful = unseccessful / (float) ( tablesize ) ;

  // calculate the average comparisons of successful search
  float seccessful = probNum / (float)dataSize;

  cout << endl << "Hash table has been successfully created by Quadratic probing";
  cout << endl << "unsuccessful search: " << fixed << setprecision(4) << unseccessful << " comparisons on average";
  cout << endl << "successful search: " << fixed << setprecision(4)  << seccessful << " comparisons on average" << endl;
}  // Hashing::showQuadraticProbing()

// ========================= Hashing::showDoubleHashing() ========================== //
void Hashing::showDoubleHashing( int dataSize ) {
  // calculate the average comparisons of successful search
  float seccessful = probNum / (float)dataSize;

  cout << endl << "Hash table has been successfully created by Double hashing";
  cout << endl << "successful search: " << fixed << setprecision(4)  << seccessful << " comparisons on average" << endl;
}  // Hashing::showDoubleHashing()

// ============================== Hashing::outFile() =============================== //
void Hashing::outputFile( string method, string fileName, item* table ) {
  string file;

  // set output file name
  if ( method == "Quadratic probing" )
    file = "quadratic";
  else
    file = "double";

  ofstream fout;

  fout.open( file + fileName + ".txt" );

  // write data in text file
  if ( !fout.fail() ) {
    fout << " --- Hash table created by " << method << " ---" << endl;
    for ( int i = 0 ; i < tablesize ; i = i + 1 ) {
      fout << "[" << setw(3) << i << "] ";
      if ( table[i].hvalue != -1 )
        fout << setw(10) << table[i].hvalue << ", " << setw(10) << table[i].sid << ", "
             << setw(10) << table[i].sname << ", " << setw(10) << table[i].mean << endl;
      else
        fout << endl;

    }  // for

    fout << " ----------------------------------------------------- ";
  }  // if

}  // Hashing::outputFile()
