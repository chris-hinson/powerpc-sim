#include <cstddef>
#include <stdlib.h>
#include <unordered_map>
#include "termcolor.hpp"
#include "instruction.cpp"
using namespace std;

class cpu{
public:
  int ticks;
  string prgName;
  vector<instruction> program;
  vector<byte> data;

  //we need a program file to construct our cpu
  cpu(string filename){

    ////////////////////////////////////////////////////////////////////////////
    //parsing our input file
    prgName = filename;
    //open our program file
    fstream progFile;
    progFile.open(prgName,ios::in);
    if (!progFile) {
        cout << "Unable to open file";
        exit(1); // terminate with error
    }

    //read file into a vector for us to parse
    std::vector<string> linebuf;
    string curLine;
    //keep a string,address map for labels
    unordered_map<string,size_t> labels;
    size_t cur_instr_addr = 0;
    while (getline(progFile, curLine)) {
        //if this line has a label, add the label to labels mapping and then add
        //the rest of the line to our buffer vec to be parsed
        size_t hasLabel = curLine.find(":");
        if (hasLabel != string::npos){
          string label = curLine.substr(0, hasLabel);
          curLine = curLine.substr(hasLabel + 1);
          cout << "found label at " << cur_instr_addr  << " " << label;
          cout << " rest of instruction is " << curLine << '\n';
          labels[label] = cur_instr_addr;
        }

        linebuf.push_back(curLine);
        cur_instr_addr +=32;
    }
    progFile.close();

    //now that we have our vector of lines as well as map of labels to addresses
    //lets start parsing
    ////////////////////////////////////////////////////////////////////////////


  }

  void prettyPrint(){
    //DEBUG - print out string vec to make sure we read program correctly
    cout << termcolor::green <<"cpu instruction \"cache\" : \n" << termcolor::reset;
    for (int i = 0; i < program.size(); i++)
        program[i].prettyPrint();

  }
};
