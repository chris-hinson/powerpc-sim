#include <cstddef>
#include <stdlib.h>
#include "termcolor.hpp"
#include "instruction.cpp"
using namespace std;

class cpu{
public:
  int ticks;
  string prgName;
  vector<string> program;
  vector<byte> data;

  //we need a program file to construct our cpu
  cpu(string filename){
    prgName = filename;
    //open our program file
    fstream progFile;
    progFile.open(prgName,ios::in);
    if (!progFile) {
        cout << "Unable to open file";
        exit(1); // terminate with error
    }

    //read file into our program vec
    string curLine;
    while (getline(progFile, curLine)) {
        //does this line contain a label?
        size_t hasLabel = curLine.find(":");
        if (hasLabel != string::npos){
          cout << termcolor::red <<"WE FOUND A LABEL: " << curLine << "\n" << termcolor::reset;
          //remove the label before adding the instruction
          curLine = curLine.substr(hasLabel + 1);
        }

        program.push_back(curLine);
    }
    progFile.close();

    cout << termcolor::yellow << "constructed cpu" << "\n" << termcolor::reset;

    //DEBUG - print out string vec to make sure we read program correctly
    // Declaring iterator to a vector
    vector<string>::iterator ptr;
    // Displaying vector elements using begin() and end()
    cout << termcolor::green <<"cpu instruction \"cache\" : \n" << termcolor::reset;
    for (ptr = program.begin(); ptr < program.end(); ptr++)
        cout << *ptr << "\n";
  }
};
