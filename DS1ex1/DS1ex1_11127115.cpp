// 11127115, ³¯«º¦w
# include <iostream>
# include <fstream>
# include <vector>
# include <cstdlib>
# include <string.h>
# include <cctype>

using namespace std;

enum Mode {
  ONEGOAL,
  MOREGOALS
};  // Mode

enum Direction {
  RIGHT = 0,
  DOWN = 1,
  LEFT = 2,
  UP = 3
};  // Dorection

struct PassedG {
  int x;
  int y;
};  // passedG

class Process {
  int x;  // store current x coordinates
  int y;  // store current y coordinates
  int numG;  // store the number of passed G
  Direction direction;  // store current direction

  public:
    Process( int inx, int iny, Direction inDir );

    void setXY( int inx, int iny );  // set the coordinates
    void setDir( int dir );  // set the direction
    int getX();  // get X coordinates
    int getY();  // get Y coordinates
    Direction getDir();  // get current coordinate
    int getNextX( Direction direction );  // get the x coordinates of next step
    int getNextY( Direction direction );  // get the y coordinates of next step
    bool inRange( int maxX, int maxY );  // check the coordinates is in the range or not
    bool match( int inX, int inY );  // check current x, y match input x, y or not

};  // class Process

class Maze {
  public:
    bool load( string fileName, Mode mode ); // read a text file to store a maze
    bool findGoals( Maze & inMaze, Process mouse );  // find the goals in the maze
    void show();                    // show on the screen the maze
    void Vsetter( int x, int y );  // change the content of the maze to 'V'
    void Rsetter( int x, int y );  // change the content of the maze to 'R'
    void setNumOfGoals( int updateNum );  // set the desired number of goals
    void setPassedG( int x, int y );  // store the found goals in a vector
    void mazeCopy( Maze oriMaze );  // copy the maze to another
    void clearMaze();               // clear the content of the maze
    vector< vector<char> > getMaze();  // get the content of the maze
    int getTotalX();  // get the total x size of the maze
    int getTotalY();  // get the total y size of the maze
    int getNumOfGoals();  // get the desired number of goals
    char getContent( int x, int y );  // get the content of coordinates
    bool checkNextStep( Process mouse );  // check next step of current direction is not passed or is a obstacle
    bool checkPassed( Process mouse );  // check currently encountered G has passed or not
    bool checkRoute( Process mouse, Process & nextMouse );  // check there is a way at four directions
    void fixedRRoute( );  // fixed R route of G
    void revertToR( ) ;  // change F to R
    bool inNumIsDigit( string inNum );  // check the content of input string is digit or not

  private:
    int totalX;  // store the total x size of the maze
    int totalY;  // store the total y size of the maze
    int numOfGoals;  // store the desired number of goals
    vector < vector<char> > maze;
    vector< PassedG > passedG;

};  // class Maze

int main() {
  Maze maze;
  Maze copyMaze;
  Process mouse( 0, 0, RIGHT );

  bool ended = false;
  char command = '\0';
  Mode mode = ONEGOAL;
  string fileName = "";

  while ( ended == false ) {
    cout << endl << "*** Path Finding ***";
    cout << endl << "* 0. Quit          *";
    cout << endl << "* 1. One goal      *";
    cout << endl << "* 2. More goals    *";
    cout << endl << "********************";
    cout << endl << "Input a command(0, 1, 2) :";

    cin >> command;  // get command

    if ( command == '1' || command == '2' ) {
      cout << endl << "Input a file number:";
      cin >> fileName;

      if ( command == '2' ) {
        mode = MOREGOALS;  // one or more goals
      }  // if

      if ( maze.load( fileName, mode ) ) {  // load the content of the maze
        copyMaze.mazeCopy(maze);  // a copy to store the passed route
        bool success = copyMaze.findGoals( maze, mouse );

        mode = ONEGOAL;  // reset the mode

        copyMaze.show();  // show on screen the passed route
        cout << endl;

        if ( success == true ) {  // check the goals are reached or not
          maze.revertToR();  // set the content of route of get goals to 'R'

          maze.show();  // show on screen the route to goals
          cout << endl;
        }  // if

        copyMaze.clearMaze();  // clear up the copy
      }  // if

      maze.clearMaze();  // clear up the original maze

      // reset the coordinates and direction of the mouse
      mouse.setXY( 0, 0 );
      mouse.setDir( RIGHT );
    }  // if
    else if ( command == '0' ) {
      ended = true;
    }  // else
    else {
      cout << endl << "Command does not exist!" << endl;
    }  // else

  }  // while

}  // main()

/** ********************* class Process function ***************** **/

// ========================= Process:: Process ==========================
Process::Process( int inx, int iny, Direction inDir ) {
  x = inx;
  y = iny;
  direction = inDir;
}  // Process::Process()

// ========================== Process:: setXY ===========================
void Process::setXY( int inx, int iny ) {
  x = inx;
  y = iny;
}  // Process::setXY()

// ========================== Process:: setDir ==========================
void Process::setDir( int dir ) {

  if ( dir % 4 == 0 ) {  // right
    Process::direction = RIGHT;
  }  // if
  else if ( dir % 4 == 1 ) {  // down
    Process::direction = DOWN;
  }  // else if
  else if ( dir % 4 == 2 ) {  // left
    Process::direction = LEFT;
  }  // else if
  else if ( dir % 4 == 3 ) {  // up
    Process::direction = UP;
  }  // else

}  // Process::setDir()

// =========================== Process:: getX ===========================
int Process::getX() {
  return x;
}  // Process::getX()

// =========================== Process:: getY ===========================
int Process::getY() {
  return y;
}  // Process::getY()

// ========================== Process:: getDir ==========================
Direction Process::getDir() {
  return direction;
}  // Process::getY()

// ========================= Process:: nextX ============================
int Process::getNextX( Direction currentDirection ) {
  int nextX = x;

  if ( currentDirection % 4 == RIGHT )
    nextX = nextX + 1;
  else if ( currentDirection % 4 == LEFT )
    nextX = nextX - 1;

  return nextX;
}  // Process::nextXY

// ========================= Process:: nextY ============================
int Process::getNextY( Direction currentDirection ) {
  int nextY = y;

  if ( currentDirection % 4 == DOWN )
    nextY = nextY + 1;
  else if ( currentDirection % 4 == UP )
    nextY = nextY - 1;

  return nextY;
}  // Process::nextXY

// ========================= Process:: inRange ==========================
bool Process::inRange( int maxX, int maxY ) {
  bool isInRange = false;

  if ( x < maxX && y < maxY && x >= 0 && y >= 0 ) {
    isInRange = true;
  }  // if

  return isInRange;
}  // inRange()

// ========================== Process:: match ===========================
bool Process::match( int inX, int inY ) {
  bool isMatch = false;

  if ( x == inX && y == inY ) {
    isMatch = true;
  }  // if

  return isMatch;
}  // Process::match()

/** ************************************************************** **/


/** *********************** class Maze function ****************** **/

// ========================== Maze::mazeCopy ===========================
void Maze::mazeCopy( Maze oriMaze ) {
  // copy totalX, totalY and numOfGoals
  totalX = oriMaze.getTotalX();
  totalY = oriMaze.getTotalY();
  numOfGoals = oriMaze.getNumOfGoals();

  // copy maze
  vector< vector<char> > tempMaze = oriMaze.getMaze();
  maze = tempMaze ;

}  // mazeCopy()

// =========================== Maze::getMaze ===========================
vector< vector<char> > Maze::getMaze() {
  return maze;
}  // Maze::getMaze()

// ========================== Maze::getTotalX ==========================
int Maze::getTotalX() {
  return totalX;
}  // Maze::getTotalX()

// ========================== Maze::getTotalX ==========================
int Maze::getTotalY() {
  return totalY;
}  // Maze::getTotalX()

// ======================== Maze::getNumOfGoals ========================
int Maze::getNumOfGoals() {
  return numOfGoals;
}  // Maze::getNumOfGoals()

// ========================== Maze::getContent =========================
char Maze::getContent( int x, int y ) {
  return Maze::maze[y][x];
}  // Maze::getContent()

// ============================ Maze::load =============================
bool Maze::load( string fileName, Mode mode ) {
  ifstream fin;
  ofstream fout;
  vector <char> tempVector;

  char tempChar = '\0';
  bool success = false;  // read a file successfully or not

  fileName = "input" + fileName + ".txt";
  fin.open( fileName );

  // check the file exist or not
  if ( fin.fail() ) {
    cout << endl << fileName << " does not exist!" << endl;
  }  // if
  else {
    success = true;
    Maze::numOfGoals = 1;

    if ( mode == MOREGOALS ) {
      string inNum = "";

      // check inNum meet the requirements or not
      bool isNum = false;

      do {
        cout << endl << "Number of G (goals):";
        cin >> inNum;

        // check input string is a number or not
        isNum = inNumIsDigit( inNum );

        if ( isNum == true ) {
          numOfGoals = stoi( inNum );

          // check the range of input num
          if ( ( numOfGoals > 100 || numOfGoals < 1 ) ) {
            cout << endl << "### The number must be in [1,100] ###" << endl;
          }  // if

        }  // if

      }  while ( isNum != true || numOfGoals > 100 || numOfGoals < 1 );

    }  // if

    // input the size of maze
    fin >> totalX >> totalY;

    // clear the content of Maze::maze
    Maze::maze.clear();

    // set maze content
    for ( int i = 0 ; i < totalY ; i = i + 1 ) {
      // clear the content of tempVector
      tempVector.clear();

      for ( int j = 0 ; j < totalX ; j = j + 1 ) {
        fin >> tempChar;
        tempVector.push_back(tempChar);
      }  // for

      Maze::maze.push_back(tempVector);
    }  // for

  }  // else

  fin.close();
  fout.close();
  return success;
}  // Maze::load()

// =========================== Maze::Vsetter ===========================
void Maze::Vsetter( int x, int y ) {

  if ( Maze::maze[y][x] == 'E' ) {
    Maze::maze[y][x] = 'V';
  }  // if

}  // Maze::Vsetter()

// =========================== Maze::Rsetter ===========================
void Maze::Rsetter( int x, int y ) {

  if ( Maze::maze[y][x] == 'E' ) {
    Maze::maze[y][x] = 'R';
  }  // if
  else if ( Maze::maze[y][x] == 'R' ) {
    Maze::maze[y][x] = 'E';
  }  // else if

}  // Maze::Vsetter()

// ======================== Maze::setNumOfGoals ========================
void Maze::setNumOfGoals( int updateNum ) {
  Maze::numOfGoals = updateNum;
}  // Maze::setNumOfGoals

// ========================= Maze::checkPassed =========================
bool Maze::checkPassed( Process mouse ) {
  bool passed = false;

  for ( int i = 0 ; i < passedG.size() ; i = i + 1 ) {
    if ( mouse.match( passedG[i].x, passedG[i].y ) == true ) {
      passed = true;
    }  // if

  }  // for

  return passed;
}  // Maze::checkPassed()
// ========================= Maze::setPassedG ==========================
void  Maze::setPassedG( int x, int y ) {
  PassedG temp;
  temp.x = x;
  temp.y = y;

  passedG.push_back( temp );
}  //  Maze::setPassedG()

// ========================= Maze::findGoals ===========================
bool Maze::findGoals( Maze & inMaze, Process mouse ) {
  bool success = false;
  bool check = false;
  bool goAhead = true;

  // mark current step
  inMaze.Rsetter( mouse.getX(), mouse.getY() );
  Maze::Vsetter( mouse.getX(), mouse.getY() );

  // found goals
  if ( Maze::getContent( mouse.getX(), mouse.getY() ) == 'G' ) {

    if ( passedG.empty() == true || checkPassed( mouse ) == false ) {
      setNumOfGoals( getNumOfGoals() - 1 );
      setPassedG( mouse.getX(), mouse.getY());
      inMaze.fixedRRoute();
    }  // if

    if ( getNumOfGoals() == 0 ) {
      return true;
    }  // if

  }  // if

  // find the next step of current direction
  int nextX = mouse.getNextX( mouse.getDir() );
  int nextY = mouse.getNextY( mouse.getDir() );

  // create an object to store the coordinates of next possible step
  Process nextMouse( nextX, nextY, mouse.getDir() );

  do {
    // check mouse can keep go ahead or not
    check = checkRoute( mouse, nextMouse );

    if ( check == false ) {
      inMaze.Rsetter( mouse.getX(), mouse.getY() );
      return false;
    }  // if
    else {
      goAhead = Maze::findGoals( inMaze, nextMouse );
    }  // else

  }  while ( goAhead == false ) ;

  success = goAhead;
  return success;
}  // Maze::findGoals()

// ============================ Maze::show =============================
void Maze::show() {
  for ( int i = 0 ; i < totalY ; i = i + 1 ) {
    for ( int j = 0 ; j < totalX ; j = j + 1 ) {
      cout << Maze::maze[i][j];
    }  // for

    cout << endl;
  }  // for

}  // Maze::show()

// ========================= Maze::clearMaze ===========================
void Maze::clearMaze() {
  // clear the content of the maze
  Maze::maze.clear();

  // clear the content of the coordinates of passed G
  passedG.clear();
}  // Maze::clearMaze()

// ========================= Maze::checkNextStep =======================
bool  Maze::checkNextStep( Process mouse ) {
  bool canMove = true;

  // check the next step is not out of maze size, not have obstacle or passed
  if ( mouse.inRange( getTotalX(), getTotalY() ) == false ||
       ( getContent( mouse.getX(), mouse.getY() ) != 'E' &&
         getContent( mouse.getX(), mouse.getY() ) != 'G' ) ||
         checkPassed( mouse ) == true ) {
    canMove = false;
  }  // if

  return canMove;
}  //  Maze::checkNextStep()

// ========================== Maze::checkRoute =========================
bool Maze::checkRoute( Process mouse, Process & nextMouse ) {
  bool keepGoing = true;
  int changeDirTimes = 0;

  while ( checkNextStep( nextMouse ) == false && keepGoing == true ) {
    // change direction and count the change times
    changeDirTimes = changeDirTimes + 1;
    nextMouse.setDir( nextMouse.getDir() + 1 );

    // already check 4 directions, but still can't keep moving ahead
    if ( changeDirTimes == 4 ) {
      keepGoing = false;
    } // if
    else {
      // set the next coordinates
      nextMouse.setXY( mouse.getNextX( nextMouse.getDir() ), mouse.getNextY( nextMouse.getDir() ) );
    }  // while

  } // while

  return keepGoing;
} // Maze::checkRoute();

// ========================= Maze::fixedRRoute ========================
void Maze::fixedRRoute( ) {

  for ( int i = 0 ; i < getTotalY() ; i = i + 1 ) {
    for ( int j = 0 ; j < getTotalX() ; j = j + 1 ) {
      if ( Maze::maze[i][j] == 'R' ) {
        Maze::maze[i][j] = 'F';
      }  // if

    }  // for

  }  // for

}  // Maze::fixedRRoute()

// ========================== Maze::revertToR =========================
void Maze::revertToR( ) {

  for ( int i = 0 ; i < getTotalY() ; i = i + 1 ) {
    for ( int j = 0 ; j < getTotalX() ; j = j + 1 ) {
      if ( Maze::maze[i][j] == 'F' ) {
        Maze::maze[i][j] = 'R';
      }  // if

    }  // for

  }  // for

}  // Maze::revertToR()

// ======================== Maze::inNumIsDigit ========================
bool Maze::inNumIsDigit( string inNum ) {
  bool isDigit = true;
  for ( int i = 0 ; i < inNum.size() ; i = i + 1 ) {
    if ( isdigit( inNum[i] ) == false ) {
      isDigit = false;
    }  // if

  }  // for

  return isDigit;
}  // Maze::inNumIsDigit()

/** ************************************************************** **/
