// 11127115, ³¯«º¦w
# include <stdlib.h>
# include <iostream>
# include <string>
# include <sstream>

#define MAX_LEN 255

using namespace std;

class STACK{
  struct stackNodes {
    string value;
    stackNodes* next;
  };

  stackNodes* topPtr;

  public:
    STACK();
    ~STACK();
    bool isEmpty();  // check the stack is empty or not
    void push( string inValue);  // push the item in stack
    void getTop( string &outValue);  // get the top item of the stack
    void pop();  // delete the top item of the stack
    void pop( string &outValue);  // get and delete the top item of the stack
};  // class STACK

class LIST {
  struct listNodes {
    string value;
    listNodes* next;
  };

  listNodes *head, *tail;

  public:
    LIST();
    ~LIST();
    bool isEmpty();  // check the list is empty or not
    void ins( string inValue );  // input an item to the list
    void getHead( string & outValue );  // get the item that head pointer point
    void del();  // delete the item that head pointer point
    void del( string & outValue );  // get and delete the item that head pointer point
    void clearUp();  // clean up whole linked list
    void show();  // display whole linked list
};  // class LIST

bool IsLegalInfixExpression( string inS );  // check the input string is legal infix expression or not
bool IsExp( string inS );  // check there are illegal operator or not
bool IsBalance( string inS );  // check the parentheses is balance or not
bool IsLegal( string inS );  // check the infix expression is legal or not
bool infixToPostfix( LIST & postfix, string infix );  // translate infix expression to postfix expression
string RemoveSpace( string inString );  // remove the blank space from string

void Calculator( LIST & postfix );
string Calculate( string operand1, string operand2, string operators );

int main() {
  int command = 1;
  string ans = "";
  string inputInfix = "";

  while ( command != 0 ) {
    cout << endl << "* Arithmetic Expression Evaluator *";
    cout << endl << "* 0. QUIT                         *";
    cout << endl << "* 1. Infix2postfix Transformation *";
    cout << endl << "***********************************";
    cout << endl << "Input a choice(0, 1): ";

    cin >> command;

    cin.ignore( MAX_LEN, '\n' );  // clear the enter key

    if ( command != 1 && command != 0 ) {
      cout << endl << "Command does not exist!" << endl;
    }  // if
    else if ( command == 1 ) {
      cout << endl << "Input an infix expression: ";
      getline( cin, inputInfix );  // input string
      inputInfix = RemoveSpace( inputInfix );

      if ( inputInfix.length() != 0 ) {

        if ( IsLegalInfixExpression( inputInfix ) ) {  // check the input is infix expression or not
          cout << "It is a legitimate infix expression." << endl;

          LIST postfix;

          infixToPostfix( postfix, inputInfix );  // translate infix expression to postfix expression
          cout << "Postfix expression: ";
          postfix.show();

          Calculator( postfix );
        }  // if

      }  // if
      else {
        cout << "Error: input infix expression is empty." << endl;
      }  // else

    }  // else if

  }  // while

}  // main()
// ============================= IsLegalInfixExpression ==============================
bool IsLegalInfixExpression( string inS ) {
  bool isLegalInfix = true;

  if ( !IsExp( inS ) ) {
    isLegalInfix = false;
  }  // if
  else if ( !IsBalance( inS ) ) {
    isLegalInfix = false;
  }  // else if
  else if ( !IsLegal( inS ) ) {
    isLegalInfix = false;
  } // else if

  return isLegalInfix;
}  // isLegalInfixExpression()

// ====================================== IsExp ======================================
bool IsExp( string inS ) {
  bool isExp = true;
  int index = 0;

  while ( index < inS.length() && isExp == true ) {

    if ( ( (int) inS[index] < (int)'0' || (int) inS[index] > (int)'9' ) &&  // check current char is number or not
         inS[index] != ' ' ) {  // check current char is blank space or not

      // check operator
      if ( inS[index] != '*' && inS[index] != '/' &&
           inS[index] != '+' && inS[index] != '-' &&
           inS[index] != '(' && inS[index] != ')' ) {
        isExp = false;
        cout << "Error 1: " << inS[index] << " is not a legitimate character." << endl;
      }  // if

    }  // if

    index = index + 1;
  }  // while

  return isExp;
}  // isExp()

// ==================================== IsBalance ====================================
bool IsBalance( string inS ) {
  bool isBalance = true;
  int index = 0;
  string inValue = "";
  STACK stackOfParenthesis;

  while ( index < inS.length() && isBalance == true ) {

    if ( inS[index] == '(' ) {
      inValue = inS[index];
      stackOfParenthesis.push( inValue );
    }  // if
    else if ( inS[index] == ')' ) {
      if ( stackOfParenthesis.isEmpty() ) {
        isBalance = false;
        cout << "Error 2: there is one extra close parenthesis." << endl;
      }  // if
      else {
        stackOfParenthesis.pop();
      }  // else

    }  // else

    index = index + 1;
  }  // while

  if ( isBalance == true && stackOfParenthesis.isEmpty() == false ) {  // still have open parenthesis in stack
    isBalance = false;
    cout << "Error 2: there is one extra open parenthesis." << endl;
  }  // if

  stackOfParenthesis.~STACK();
  return isBalance;
}  // isBalance()

// ===================================== IsLegal =====================================
bool IsLegal( string inS ) {
  bool isLegal = true;
  int index = 0;
  char prevItem = '\0';

  while ( index < inS.length() && isLegal == true ) {

    // current char is number
    if ( (int) inS[index] >= (int) '0' && ( int ) inS[index] <= (int) '9' ) {
      if ( prevItem == ')' ) {
        isLegal = false;
        cout << "Error 3: there is one extra operand." << endl;
      }  // if

    }  // if

    // current char is operater
    else if ( inS[index] == '*' || inS[index] == '/' || inS[index] == '+' || inS[index] == '-' ) {
      if ( prevItem == '*' || prevItem == '/' || prevItem == '+' || prevItem == '-' ||
           prevItem == '\0' ||
           index == inS.length() - 1 ) {
        isLegal = false;
        cout << "Error 3: there is one extra operator." << endl;
      }  // if
      else if ( prevItem == '(' ) {
        isLegal = false;
        cout << "Error 3: it is not infix in the parentheses." << endl;
      }  // else if

    }  // else if

    // current char is open parenthesis
    else if ( inS[index] == '(' ) {
      if ( ( (int) prevItem >= (int) '0' && ( int ) prevItem <= (int) '9' ) ||
           prevItem == ')' ) {
        isLegal = false;
        cout << "Error 3: there is one extra operand." << endl;
      }  // if

    }  // else if ()

    // current char is close parenthesis
    else if ( inS[index] == ')' ) {
      if ( prevItem == '*' || prevItem == '/' || prevItem == '+' || prevItem == '-'  ) {
        isLegal = false;
        cout << "Error 3: there is one extra operator." << endl;
      }  // if
      else if ( prevItem == '(' ) {
        isLegal = false;
        cout << "Error 3: it is not infix in the parentheses." << endl;
      }  // else if

    }  // else if

    prevItem = inS[index];
    index = index + 1;
  }  // while

  return isLegal;
}  // isLegal()

// =================================== removeSpace ===================================
string RemoveSpace( string inString ) {
  string temp = "";

  for ( int i = 0 ; i < inString.length() ; i = i + 1 ) {
    if ( inString[i] != ' ' ) {
      temp = temp + inString[i];
    }  // if

  }  // for

  return temp;
}  // removeSpace();

// ================================== infixToPostfix =================================
bool infixToPostfix( LIST & postfix, string infix ) {
  bool success = true;
  string operand = "";
  string operators = "";
  string getValue = "";
  int index = 0;
  STACK operatorStack;

  while ( index < infix.length() ) {
    // current char is number
    if ( (int) infix[index] >= (int) '0' && (int) infix[index] <= (int) '9' ) {
      operand = operand + infix[index];
    }  // if

    // current char is operator
    else if ( (int) infix[index] < (int) '0' || (int) infix[index] > (int) '9' ) {
      if ( operand != "" ) {
        postfix.ins( operand );
        operand = "";
      }  // if

      operators = infix[index];

      if ( operators == "(" ) {
        operatorStack.push( operators );
      }  // if
      else if ( operators == ")" ) {
        operatorStack.pop( getValue );

        // pop items until meet open parenthesis
        while ( getValue != "(" ) {
          postfix.ins(getValue);
          operatorStack.pop( getValue );
        }  // while

      }  // else if
      else if ( ( operators == "*" || operators == "/" )  ) {
        if ( !operatorStack.isEmpty() ) {
          operatorStack.getTop( getValue );
        }  // if

        // check previous item is * or /
        if ( getValue == "*" || getValue == "/" ) {
          operatorStack.pop( getValue );  // get and delete the top item of stack
          postfix.ins( getValue );  // input getValue to list
        }  // if

        operatorStack.push( operators );  // push operator to stack
        getValue = "";  // initialize getValue
      }  // else if
      else if ( operators == "+" || operators == "-" ) {
        if ( !operatorStack.isEmpty() ) {
          operatorStack.getTop( getValue );
        }  // if

        // pop item until meet open parenthesis or stack is empty
        while ( getValue != "(" && !operatorStack.isEmpty() ) {
          operatorStack.pop( getValue );
          postfix.ins(getValue);
        }  // while

        operatorStack.push( operators );
        getValue = "";  // initialize getValue
      }  // else if

    }  // else if

    index = index + 1;
  }  // while

  // check there still have operand or not
  if ( operand != "" ) {
    postfix.ins( operand );
  }  // if

  // check stack is empty
  while ( !operatorStack.isEmpty() ) {
    operatorStack.pop( getValue );
    postfix.ins( getValue );
  }  // while

  operatorStack.~STACK();
  return success;
}  // infixToPostfix()

// ==================================== Calculator ===================================
void Calculator( LIST & postfix ) {
  STACK calculator;
  string operators = "";
  string operand1 = "";
  string operand2 ="";
  string ans = "";

  while ( !postfix.isEmpty() ) {

    postfix.del( operators );

    if ( operators != "+" && operators != "-" && operators != "*" && operators != "/" ) {
      calculator.push(operators);
    }  // if
    else {
      calculator.pop(operand2);
      calculator.pop(operand1);
      if ( operand2 == "0" && operators == "/" ) {
        cout << "### Error: Divided by ZERO! ###" << endl;
        cout << "### It cannot be successfully evaluated! ###" << endl;
        ans = "";
      }  // if
      else {
        ans = Calculate( operand1, operand2, operators );
      }  // else

      calculator.push( ans );
    } // else

  }  // while

  if ( !calculator.isEmpty()) {
    calculator.pop(ans);

    if ( ans != "" )
      cout << "Answer: " << ans << endl;
  }  // if


  calculator.~STACK();
}  // Calculate()

// ==================================== Calculate ====================================
string Calculate( string operand1, string operand2, string operators ) {
  int num1 = stoi( operand1 );
  int num2 = stoi( operand2 );
  int ans = 0;
  string ansString = "";

  if ( operators == "+" ) {
    ans = num1 + num2;
  }  // if
  else if ( operators == "-" ) {
    ans = num1 - num2;
  }  // else if
  else if ( operators == "*" ) {
    ans = num1 * num2;
  }  // else if
  else if ( operators == "/" ) {
    ans = num1 / num2;
  }  // else if

  ansString = to_string(ans);

  return ansString;
}  // Calculate

/**========== class STACK ==========**/
// ================================== STACK::STACK ===================================
STACK::STACK() {
  topPtr = NULL;
}  //STACK::STACK()

// ================================== STACK::~STACK ==================================
STACK::~STACK() {
  while ( !isEmpty() ) {
    pop();
  }  // while

  topPtr = NULL;
}  // STACK::d~STACK()

// ================================== STACK::isEmpty =================================
bool STACK::isEmpty() {
  bool isEmp = false;

  if ( topPtr == NULL ) {
    isEmp = true;
  }  // if

  return isEmp;
}  // STACK::isEmpty()

// =================================== STACK::push ===================================
void STACK::push( string inValue) {
  try {
    stackNodes* temp = new stackNodes;

    temp -> value = inValue;

    if ( topPtr == NULL ) {
      temp -> next = NULL;
    }  // if
    else {
      temp -> next = topPtr;
    }  // else

    topPtr = temp;
    temp = NULL;
  }  // try
  catch ( std::bad_alloc& ba ) {
    std::cerr << endl << "bad_alloc on stack caught." << ba.what() << endl;
  }  // catch

}  // STACK::push()

// ================================== STACK::getTop ==================================
void STACK::getTop( string &outValue) {
  outValue = topPtr -> value;
}  // STACK::getTop()

// =================================== STACK::pop ====================================
void STACK::pop() {

  if ( !isEmpty() ) {
    stackNodes* temp = topPtr;

    topPtr = topPtr -> next;

    temp -> next = NULL;
    delete temp;
    temp = NULL;
  }  // if

}  //STACK::pop()

// ---------------------------------------------
void STACK::pop( string &outValue) {
  getTop( outValue );
  pop();
}  // STACK::pop()

/**========== class LIST ==========**/
// ================================= LIST::LIST ==================================
LIST::LIST() {
  head = NULL;
  tail = NULL;
}  // LIST::LIST()

// ================================ LIST::~LIST ==================================
LIST::~LIST() {
  clearUp();

  head = NULL;
  tail = NULL;
}  // LIST::~LIST()

// ================================ LIST::isEmpty ================================
bool LIST::isEmpty() {
  bool isEmp = false;

  if ( head == NULL ) {
    isEmp = true;
  }  // if

  return isEmp;
}  // LIST::isEmpty()

// ================================= LIST::ins ===================================
void LIST::ins( string inValue ) {
  try {
    listNodes* temp = new listNodes;
    temp -> value = inValue;
    temp -> next = NULL;

    if ( head == NULL ) {
      head = temp;
      tail = temp;
    }  // if
    else {
      tail -> next = temp;
      tail = temp;
    }  // else

    temp = NULL;
  }  // try
  catch ( std::bad_alloc& ba ) {
    std::cerr << endl << "bad_alloc on stack caught." << ba.what() << endl;
  }  // catch

}  // LIST::ins()

// ================================ LIST::getHead ================================
void LIST::getHead( string & outValue ) {
  outValue = head -> value;
}  // LIST::getHead()

// ================================== LIST::del ==================================
void LIST::del() {
  listNodes* temp = head;

  head = head -> next;

  temp -> next = NULL;
  delete temp;
  temp = NULL;
}  // LIST::del()

// ================================= LIST::del ===================================
void LIST::del( string & outValue ) {
  getHead( outValue );
  del();
}  // LIST::del()

// =============================== LIST::clearUp =================================
void LIST::clearUp() {
  while ( !isEmpty() ) {
    del();
  }  // while

}  // LIST::clearUp()

// ================================ LIST::show ===================================
void LIST::show() {
  listNodes* temp = head;

  while ( temp != NULL ) {
    cout << temp -> value;

    if ( temp -> next != NULL ) {
      cout << ", ";
    }  // if

    temp = temp -> next;
  }  // while

  temp = NULL;
  cout << endl;
}  // LIST::show()
