// 11127115 陳姿安, 11127156 王佩諭
# include <iostream>
# include <fstream>
# include <vector>
# include <cstdlib>
# include <ctime>
# include <iomanip>

using namespace std;

struct JobType{
  int OID;
  int arrival;
  int duration;
  int timeout;
};  // jobType

class JobList {
  vector<JobType> aList;
  string fileID;

  void reset();  // initial set up
  void showjob();  // display a list of job on screen
  void sortByArrival();  // sort all by arrival time
  void putAll( string fileName );  // write all as a file
  void showTime( int reading, int sorting, int writing );  // display time on screen
  void nextJobCheck( JobType & next );  // get next job without remove item

  public:
    JobList();
    ~JobList();
    bool isEmpty();  // check is empty or not
    bool getAll( string fileName );  // read all from input file
    bool getSorted( string fileName );  // read all from input file and sort them
    void nextJob( JobType & next );  // get and remove next job

};  // class JobList

class AnsList {
  struct abortType {
    int OID;
    int CID;
    int abort;
    int delay;
  };  // abortType

  struct doneType {
    int OID;
    int CID;
    int departure;
    int delay;
  };  // doneType

  vector<abortType> abortJobs;
  vector<doneType> doneJobs;
  float avgDelay;
  float successRate;

  void showAbortJobs();  // display all abort jobs on screen
  void showDoneJobs();  // display all done jobs on screen
  void computeStat();  // compute the statistics
  void showStat();  // display statistics on screen

  public:
    AnsList();
    ~AnsList();
    void showAll();  // display all items on screen
    void addAbortJob( int OID, int CID, int abort, int delay );  // add item in abort list
    void addDoneJob( int OID, int CID, int departure, int delay );  // add item in done list
    void putAll( string fileName );  // write as a file
    void putAll_CID( string fileName );

};  // class AnsList

class JobQueue {
  JobType *circularArray;
  int qFront, qBack;
  int qSize, qMax;

  public:
    int avail;  // the time when CPU is available

    JobQueue();
    JobQueue( int maxS );
    ~JobQueue();
    int length();  // get current queue length
    bool isEmpty();  // check the queue is empty or not
    bool isFull();  // check the queue is full or not
    void enQueue( JobType enItem );  // add item in queue
    void getFront( JobType & getfront );  // get the first item in queue
    void deQueue();  // remove the first item in queue
    void deQueue( JobType & getfront );  // remove and get first item int queue
    void clearQ();  // clean up
    void showAll();  // display all on screen

};  // class Queue


class Simulation {
  JobList jobs;
  AnsList answers;

  JobQueue **allQueue;
  int qNum;

  void delQ( int index );  // delete an old job from queue
  void updateQ( int index );  // update each queue
  void finishQ();  // finish the remaining jobs in queue

  public:
    Simulation(JobList aList, int N, int qMax );
    ~Simulation();
    bool SQF( int & index );  // find shortest queue
    void process();  // simulate queue
    void outputFile( string fileName, string command );  // write as a file
    bool chooseQ( int & index, int arrival );
    bool minAvail( int & index );
};  // class Simulation

int main() {
  string command = "";
  string fileName = "";
  int numOfQueue = 1;
  int qMax = 4;
  bool stop = false;

  while ( stop == false ) {
    cout << endl << "**** Simulate FIFO Queues by SQF *****";
    cout << endl << "* 0. Quit                            *";
    cout << endl << "* 1. Sort a file                     *";
    cout << endl << "* 2. Simulate one FIFO queue         *";
    cout << endl << "* 3. Simulate two queues by SQF      *";
    cout << endl << "**************************************";
    cout << endl << "Input a command(0, 1, 2, 3):";

    cin >> command;

    JobList aList;
    if ( command == "1" || ( ( command == "2" || command == "3" ) && fileName == "" ) ) {
      cout << endl << "Input a file number:";
      cin >> fileName;
    }  // if

    if ( command == "1") {

        if ( !aList.getSorted( fileName ) )
          cout << endl << "### Get nothing from the file input" << fileName << ".txt ! ###" << endl;
      }  // if
    else if ( command == "2" ) {
      if ( aList.getAll("sorted" + fileName + ".txt" ) ) {
        Simulation simulate( aList, 1, qMax );
        simulate.process();
        simulate.outputFile("output" + fileName + ".txt", command );
      }  // if

    }  // else if
    else if ( command == "3" ) {
      numOfQueue = 2;
      if ( aList.getAll("sorted" + fileName + ".txt" ) ) {
        Simulation simulate( aList, numOfQueue, qMax );
        simulate.process();
        simulate.outputFile("double" + fileName + ".txt", command );
      }  // if

    }  // else if
    else if ( command == "0" ) {
      stop = true;
    }  // else if
    else {
      cout << endl << "Command does not exist!" << endl;
    }  // else

  }  // while

}  // main()

/** class JobList **/
// ============================== JobList::JobList() ==============================
JobList::JobList() {
  reset();
}  // JobList::JobList()

// ============================= JobList::~JobList() ==============================
JobList::~JobList() {
  reset();
}  // JobList::~JobList()

// ============================== JobList::reset() ================================
void JobList::reset() {
  aList.erase(aList.begin(),aList.end());
  fileID = "";
}  // JobList::reset()

// ============================= JobList::showjob() ===============================
void JobList::showjob() {

  cout << endl << "\t" << fileID;
  for( int i = 0 ; i < aList.size() ; i = i + 1 ) {
    cout << endl << "(" << i + 1 << ")" << "\t" << aList[i].OID << "\t" << aList[i].arrival << "\t" << aList[i].duration << "\t" << aList[i].timeout;
  }  // for

  cout << endl;
}  // JobList::showjob()

// ============================ JobList::showtime() ===============================
void JobList::showTime( int reading, int sorting, int writing ) {
  cout << endl << "Reading data: " << reading << " clocks (" << fixed << setprecision(2) << double(reading) << " ms).";
  cout << endl << "Sorting data: " << sorting << " clocks (" << fixed << setprecision(2) << double(sorting) << " ms).";
  cout << endl << "Writing data: " << writing << " clocks (" << fixed << setprecision(2) << double(writing) << " ms).";
  cout << endl ;
}  // JobList::showtime()

// ========================= JobList::sortByArrival() =============================
void JobList::sortByArrival() {

  for ( int i = aList.size() / 2 ; i > 0 ; i = i / 2 ) {
    for ( int j = i ; j < aList.size() ; j = j + 1 ) {
      int location = j;
      JobType nextItem = aList[j];

      for ( ; ( location >= i ) && ( ( aList[location - i].arrival > nextItem.arrival ) ||
              ( ( aList[location - i].arrival == nextItem.arrival ) && ( aList[location - i].OID > nextItem.OID ) ) )
            ; location = location - i ) {

          aList[location] = aList[location - i];

      }  // for

      aList[location] = nextItem;
    }  // for

  }  // for

}  // JobList::sortByArrival()

// ============================= JobList::putAll() ================================
void JobList::putAll( string fileName ) {
  ofstream fout;

  fout.open( fileName );

  if ( fout.is_open() == false ) {
    cout << "Fail to open file." << endl;
  }  // if
  else {
    fout << fileID << endl;

    for( int i = 0 ; i < aList.size() ; i = i + 1 ) {
      fout << aList[i].OID << "\t" << aList[i].arrival << "\t" << aList[i].duration << "\t" << aList[i].timeout << endl;
    }  // for

  }  // else

  fout.close();
}  // JobList::putAll()

// ============================= JobList::getAll() ================================
bool JobList::getAll( string fileName ) {
  bool success = false;
  JobType temp;

  ifstream fin;

  fin.open(fileName);

  if ( fin.fail() ) {
    cout << endl << "### " << fileName << " does not exist! ###" << endl;
  }  // if
  else{
    success = true;
    getline(fin, fileID);

    while ( fin >> temp.OID >> temp.arrival >> temp.duration >> temp.timeout ) {
      aList.push_back(temp);
    }  // while

  }  // else

  fin.close();
  return success;
}  // JobList::getAll()

// =========================== JobList::getSorted() ===============================
bool JobList::getSorted( string fileName ) {
  clock_t start_t, end_t;
  int reading, sorting, writing;
  bool success = true;

  start_t = clock();
  bool getFile = getAll( "input" + fileName + ".txt" );
  end_t = clock();

  reading = end_t - start_t;


  if ( isEmpty() && getFile == true ) {
    success = false;
  }  // if
  else if ( !isEmpty() && getFile == true ) {

    showjob();

    start_t = clock();
    sortByArrival();
    end_t = clock();

    sorting = end_t - start_t;

    fileName = "sorted" + fileName + ".txt";

    start_t = clock();
    putAll( fileName );
    end_t = clock();

    writing = end_t - start_t;


    showTime(reading, sorting, writing );

    cout << endl << "See " << fileName << endl;
  }  // if

  return success;
}  // JobList::getSorted()

// ============================ JobList::isEmpty() ================================
bool JobList::isEmpty() {
  bool isEmpty = false;

  if ( aList.size() == 0 ) {
    isEmpty = true;
  }  // if

  return isEmpty;
}  // JobList::isEmpty()

// ============================ JobList::nextJob() ================================
void JobList::nextJob( JobType & next ) {
  nextJobCheck(next);
  aList.erase( aList.begin() );
}  // JobList::nextJob()

// ========================== JobList::nextJobCheck() =============================
void JobList::nextJobCheck( JobType & next ) {
  next = aList[0];
}  // JobList::nextJobCheck

/** class AnsList **/
// ============================== AnsList::AnsList() ==============================
AnsList::AnsList():avgDelay(0.0) ,successRate(0.0) {
  abortJobs.erase(abortJobs.begin(),abortJobs.end());
  doneJobs.erase(doneJobs.begin(),doneJobs.end());
}  // AnsList::AnsList()

// ============================= AnsList::~AnsList() ==============================
AnsList::~AnsList() {
  avgDelay = 0.0;
  successRate = 0.0;
  abortJobs.erase(abortJobs.begin(),abortJobs.end());
  doneJobs.erase(doneJobs.begin(),doneJobs.end());
}  // AnsList::~AnsList()

// =========================== AnsList::showAbortJobs() ===========================
void AnsList::showAbortJobs() {

  cout << endl << "\t[Abort Jobs]";
  cout << endl << "\t" << "OID" << "\t" << "Abort" << "\t" << "Delay" ;
  for( int i = 0 ; i < abortJobs.size() ; i = i + 1 ) {
    cout << endl << "[" << i + 1 << "]" << "\t" << abortJobs[i].OID << "\t" << abortJobs[i].abort << "\t" << abortJobs[i].delay ;
  }  // for

}  // AnsList::showAbortJobs()

// =========================== AnsList::showDoneJobs() ============================
void AnsList::showDoneJobs() {

  cout << endl << "\t[Jobs Done]";
  cout << endl << "\t" << "OID" << "\t" << "Departure" << "\t" << "Delay" ;
  for( int i = 0 ; i < doneJobs.size() ; i = i + 1 ) {
    cout << endl << "[" << i + 1 << "]" << "\t" << doneJobs[i].OID << "\t" << doneJobs[i].departure << "\t" << doneJobs[i].delay ;
  }  // for

}  // AnsList::showDoneJobs()

// ============================= AnsList::showStat() ==============================
void AnsList::showStat() {
  cout << endl << "[Average Delay]\t" << fixed << setprecision(2) << avgDelay << " ms";
  cout << endl << "[Success Rate]\t" << fixed << setprecision(2) << successRate << "%" << endl;
}  // AnsList::showStat()

// ============================ AnsList::computeStat() ============================
void AnsList::computeStat() {
  float totalDelay;

  for ( int i = 0 ; i < abortJobs.size() ; i = i + 1 ) {
    totalDelay = totalDelay + abortJobs[i].delay;
  }  // for

  for ( int j = 0 ; j < doneJobs.size() ; j = j + 1 ) {
    totalDelay = totalDelay + doneJobs[j].delay;
  }  // for

  avgDelay = totalDelay / ( abortJobs.size() + doneJobs.size() );
  successRate = float ( doneJobs.size() ) / float( abortJobs.size() + doneJobs.size() ) * 100 ;
}  // AnsList::computeStat()

// ============================ AnsList::showAll() ================================
void AnsList::showAll() {
  showAbortJobs();
  showDoneJobs();
  computeStat();
  showStat();
}  // AnsList::showAll()

// ========================== AnsList::addAbortJob() ==============================
void AnsList::addAbortJob( int OID, int CID, int abort, int delay ) {
  abortType temp;
  temp.OID = OID;
  temp.CID = CID;
  temp.abort = abort;
  temp.delay = delay;

  abortJobs.push_back(temp);
}  // AnsList::addAbortJob()

// =========================== AnsList::addDoneJob() ==============================
void AnsList::addDoneJob( int OID, int CID, int departure, int delay ) {
  doneType temp;
  temp.OID = OID;
  temp.CID = CID;
  temp.departure = departure;
  temp.delay = delay;

  doneJobs.push_back(temp);
}  // AnsList::addDoneJob()

// ============================= AnsList::putAll() ================================
void AnsList::putAll( string fileName ) {
  ofstream fout;

  computeStat();
  fout.open( fileName );

  if ( fout.is_open() == false ) {
    cout << "Fail to open file." << endl;
  }  // if
  else {
    fout << "\t[Abort Jobs]";
    fout << endl << "\t" << "OID" << "\t" << "Abort" << "\t" << "Delay" ;
    for( int i = 0 ; i < abortJobs.size() ; i = i + 1 ) {
      fout << endl << "[" << i + 1 << "]" << "\t" << abortJobs[i].OID << "\t" << abortJobs[i].abort << "\t" << abortJobs[i].delay ;
    }  // for

    fout << endl << "\t[Jobs Done]";
    fout << endl << "\t" << "OID" << "\t" << "Departure" << "\t" << "Delay" ;
    for( int i = 0 ; i < doneJobs.size() ; i = i + 1 ) {
      fout << endl << "[" << i + 1 << "]" << "\t" << doneJobs[i].OID << "\t" << doneJobs[i].departure << "\t" << doneJobs[i].delay ;
    }  // for

    fout << endl << "[Average Delay]\t" << fixed << setprecision(2) << avgDelay << " ms";
    fout << endl << "[Success Rate]\t" << fixed << setprecision(2) << successRate << " %" << endl;
  }  // else

  fout.close();
}  // JobList::putAll()

// ============================= AnsList::putAll() ================================
void AnsList::putAll_CID( string fileName ) {
  ofstream fout;

  computeStat();
  fout.open( fileName );

  if ( fout.is_open() == false ) {
    cout << "Fail to open file." << endl;
  }  // if
  else {
    fout << "\t[Abort Jobs]";
    fout << endl << "\t" << "OID" << "\t" << "CID" << "\t" << "Abort" << "\t" << "Delay" ;
    for( int i = 0 ; i < abortJobs.size() ; i = i + 1 ) {
      fout << endl << "[" << i + 1 << "]" << "\t" << abortJobs[i].OID << "\t" << abortJobs[i].CID << "\t" << abortJobs[i].abort << "\t" << abortJobs[i].delay ;
    }  // for

    fout << endl << "\t[Jobs Done]";
    fout << endl << "\t" << "OID" << "\t" << "CID" << "\t" << "Departure" << "\t" << "Delay" ;
    for( int i = 0 ; i < doneJobs.size() ; i = i + 1 ) {
      fout << endl << "[" << i + 1 << "]" << "\t" << doneJobs[i].OID << "\t" << doneJobs[i].CID << "\t" << doneJobs[i].departure << "\t" << doneJobs[i].delay ;
    }  // for

    fout << endl << "[Average Delay]\t" << fixed << setprecision(2) << avgDelay << " ms";
    fout << endl << "[Success Rate]\t" << fixed << setprecision(2) << successRate << " %" << endl;
  }  // else

  fout.close();
}  // JobList::putAll()

/** class JobQueue **/
// ============================= JobQueue::JobQueue() =============================
JobQueue::JobQueue():circularArray(NULL), qFront(0), qBack(0),
                     qSize(0), qMax(0), avail(0){
}  // JobQueue::JobQueue()

// ============================= JobQueue::JobQueue() =============================
JobQueue::JobQueue( int maxS ):qSize(0), avail(0), qFront(0) {
  qMax = maxS;
  circularArray = new JobType[qMax];
  qBack = qFront;
}  // JobQueue::JobQueue()

// ============================ JobQueue::~JobQueue() =============================
JobQueue::~JobQueue() {
  delete[] circularArray;
}  // JobQueue::~JobQueue()

// ============================== JobQueue::length() ==============================
int JobQueue::length() {
  return qSize;
}  // JobQueue::length()

// ============================= JobQueue::isEmpty() ==============================
bool JobQueue::isEmpty() {
  if ( qFront == qBack ) {
    return true;
  }  // if

  return false;
}  // JobQueue::isEmpty()

// ============================== JobQueue::isFull() ==============================
bool JobQueue::isFull() {
  if ( qFront % qMax == (qBack + 1) % qMax ) {
    return true;
  }  // if

  return false;
}  // JobQueue::isFull()

// ============================= JobQueue::enQueue() ==============================
void JobQueue::enQueue( JobType enItem ) {
  circularArray[ ( qBack + 1 ) % qMax ] = enItem;
  qBack = qBack + 1;
  qSize = qSize + 1;
}  // JobQueue::enQueue()

// ============================= JobQueue::getFront() =============================
void JobQueue::getFront( JobType & getfront ) {
  getfront = circularArray[ ( qFront + 1 ) % qMax ];
}  // JobQueue::getFront()

// ============================= JobQueue::deQueue() ==============================
void JobQueue::deQueue() {
  qFront = qFront + 1;
  qSize = qSize - 1;
}  // JobQueue::deQueue()

// ============================= JobQueue::deQueue() ==============================
void JobQueue::deQueue( JobType & getfront ) {
  getFront(getfront);
  deQueue();
}  // JobQueue::deQueue()

// ============================== JobQueue::clearQ() ==============================
void JobQueue::clearQ() {
  qFront = qBack;
}  // JobQueue::clearQ()

// ============================= JobQueue::showAll() ==============================
void JobQueue::showAll() {
  for ( int i = qFront + 1 ; i <= qBack ; i = i + 1 ) {
    cout << circularArray[ i % 4 ].OID << "\t" << circularArray[ i % 4 ].arrival << "\t" << circularArray[ i % 4 ].duration << "\t" << circularArray[ i % 4 ].timeout << endl;
  }  // for

}  // JobQueue::showAll()

/** class Simulation **/
// =========================== Simulation::Simulation() ===========================
Simulation::Simulation(JobList aList, int N, int qMax):jobs(aList), qNum(N) {
  allQueue = new JobQueue*[qNum];
  for ( int i = 0 ; i < qNum ; i = i + 1 ) {
    allQueue[i] = new JobQueue(qMax);
  }  // for

}  // Simulation::Simulation()

// ========================== Simulation::~Simulation() ===========================
Simulation::~Simulation() {
  for ( int i = 0 ; i < qNum ; i = i + 1 ) {
    delete allQueue[i];
  }  // for

  delete allQueue;
  allQueue = NULL;
}  // Simulation::~Simulation()

// ============================== Simulation::delQ() ==============================
void Simulation::delQ( int index ) {
  allQueue[index] -> deQueue();
}  // Simulation::delQ()

// ============================= Simulation::process() ============================
void Simulation::process() {
  int index = 0;

  while ( !jobs.isEmpty() ) {
    // check time
    JobType job, qFront;
    jobs.nextJob(job);
    bool isempty = false;

    // cout << "next job: " << job.OID << "\t" << job.arrival << "\t" << job.duration << "\t" << job.timeout << endl;

    for ( int i = 0 ; i < qNum ; i = i + 1 ) {
      while ( job.arrival >= allQueue[i] -> avail && !allQueue[i] -> isEmpty() ) {
        updateQ(i);
      }  // while

    }  // for

    if ( chooseQ( index, job.arrival ) && allQueue[index] -> isEmpty() ) {
      answers.addDoneJob( job.OID, index + 1, job.arrival + job.duration, 0 );
      allQueue[index] -> avail = job.arrival + job.duration;
    }  // if
    else if ( SQF(index) ) {
      allQueue[index] -> enQueue( job );
    }  // else if

    // 4.1: no space in queues
    else if ( !SQF( index ) ) {
      // put in abort list
      answers.addAbortJob( job.OID, 0, job.arrival, 0 );
    }  // else if
    else {
      cout << "other suck situation :)" << endl;
    }  // else

    /*
    cout << endl << "Queue1: (" << allQueue[0] -> avail << ") : " << endl;
    allQueue[0] -> showAll();
    cout << endl << "Queue2: (" << allQueue[1] -> avail << ") : " << endl;
    allQueue[1] -> showAll();
    answers.showAll();
    */

  }  // while

  finishQ();


  // answers.showAll();

}  // Simulation::process()

// ============================== Simulation::SQF() ===============================
bool Simulation::SQF( int & index ) {
  int shortest = allQueue[index] -> length();

  for ( int i = 1 ; i < qNum ; i = i + 1 ) {
    if ( allQueue[i] -> length() < shortest ) {
      index = i;
      shortest = allQueue[i] -> length();
    }  // if

  }  // for

  if ( allQueue[index] -> isFull() ) {
    return false;
  }  // if

  return true;
}  // Simulation::SQF()

// ============================= Simulation::updateQ() ============================
void Simulation::updateQ( int index ) {
  JobType qFront;
  allQueue[index] -> getFront( qFront );

  // check there is any job in queue should be delete
  // 4.2: time out when job be taked out from queue
  while ( allQueue[index] -> avail >= qFront.timeout && !allQueue[index] -> isEmpty() ) {
    allQueue[index] -> deQueue();
    answers.addAbortJob( qFront.OID, index + 1, allQueue[index] -> avail, allQueue[index] -> avail - qFront.arrival );
    if ( !allQueue[index] -> isEmpty() )
      allQueue[index] -> getFront( qFront );
  }  // while


  if ( !allQueue[index] -> isEmpty() ) {
    allQueue[index] -> deQueue();

    // 4.3: time out during process
    if ( allQueue[index] -> avail + qFront.duration > qFront.timeout ) {
      answers.addAbortJob( qFront.OID, index + 1, qFront.timeout, qFront.timeout - qFront.arrival );
      allQueue[index] -> avail = qFront.timeout;
    }  // if

    // can finish work
    else {
      answers.addDoneJob( qFront.OID, index + 1, allQueue[index] -> avail + qFront.duration, allQueue[index] -> avail - qFront.arrival );
      allQueue[index] -> avail = allQueue[index] -> avail + qFront.duration;
    }  // else

  }  // if

}  // Simulation::updateQ()

// ============================= Simulation::finishQ() ============================
void Simulation::finishQ() {
    JobType qFront;
    int index = -1;

    if ( qNum > 1 ) {
      // check
      while ( minAvail( index ) ) {
        updateQ(index);
        index = -1;
      }  // while

    }  // if
    else {
      while ( !allQueue[0] -> isEmpty() ) {
        updateQ(0);

        /*
        cout << endl << "Queue: (" << allQueue[0] -> avail << ") : " << endl;
        allQueue[0] -> showAll();
        answers.showAll();
        */
      }  // while

    }  // else

}  // Simulation::finishQ()

// ============================= Simulation::chooseQ() ============================
bool Simulation::chooseQ( int & index, int arrival ) {
  index = 0;

  // find a queue which can operate job
  for ( int i = 0 ; i < qNum ; i = i + 1 ) {
    if ( arrival >= allQueue[i] -> avail ) {
      index = i ;
      return true;
    }  // if

  }  // for

  return false;
}  // chooseQ

// ============================ Simulation::minAvail() ============================
bool Simulation::minAvail( int & index ) {

  // search not empty queue
  for ( int i = 0 ; i < qNum && index == -1 ; i = i + 1 ) {
    if ( !allQueue[i] -> isEmpty() )
      index = i;
  }  // for

  // search the queue which have minimum avail time
  for ( int i = index ; i < qNum && index != -1 ; i = i + 1 ) {
    if ( ( allQueue[i] -> avail < allQueue[index] -> avail ) &&
         ( !allQueue[i] -> isEmpty() ) )
      index = i;
  }  // for

  if ( index == -1 ) {
    return false;
  }  // if

  return true;
} // Simulation:::minAvail()

// =========================== Simulation::outputFile() ===========================
void Simulation::outputFile( string fileName, string command ) {
  if ( command == "2" )
    answers.putAll( fileName );
  else
    answers.putAll_CID( fileName );

  cout << endl << "The simulation is running..." << endl;
  cout << "See " << fileName << endl;
}  // Simulation::outputFile()
