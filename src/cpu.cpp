#include <cstddef>
#include <stdlib.h>
#include <unordered_map>
#include "termcolor.hpp"
#include "instruction.cpp"
#include "registers.cpp"
using namespace std;

class cpu{
public:
  int ticks;
  string prgName;
  vector<instruction*> program;
  vector<char> data;

  //we need a program file to construct our cpu
  cpu(string filename){

    ////////////////////////////////////////////////////////////////////////////
    //data(4096,0);
    data.resize(4096,0);

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

        //if this line starts with a %, skip it
        size_t isComment = curLine.find("%");
        if (isComment != string::npos){
          continue;
        }

        //if this line has a label, add the label to labels mapping and then add
        //the rest of the line to our buffer vec to be parsed
        size_t hasLabel = curLine.find(":");
        if (hasLabel != string::npos){
          string label = curLine.substr(0, hasLabel);
          curLine = curLine.substr(hasLabel + 1);
          //cout << "found label at " << cur_instr_addr  << " " << label;
          //cout << " rest of instruction is " << curLine << '\n';
          labels[label] = cur_instr_addr;
        }

        linebuf.push_back(curLine);
        cur_instr_addr +=1;
    }
    progFile.close();

    size_t i = 0;

    for (; i < linebuf.size(); i++) {
      size_t split = linebuf[i].find(",");

      //break on the first line we dont find a , so we can start parsing instrs
      if (split == string::npos)
        break;

      int addr = atoi(linebuf[i].substr(0,split).c_str());
      string str = linebuf[i].substr(split+1);
      str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
      int d = atoi(str.c_str());
      //printf("addr: %d, data:%d\n",addr,(char)d);
      data[addr] = (char)d;
    }

    //skip the blank line
    i++;
    //now that we have our vector of lines as well as map of labels to addresses
    //lets start parsing
    for (; i < linebuf.size(); i++)
    {
      instruction* instr = parse(linebuf[i], i, &labels);
      program.push_back(instr);
    }

    ////////////////////////////////////////////////////////////////////////////


  }

  void prettyPrint(){
    //DEBUG - print out string vec to make sure we read program correctly
    cout << termcolor::green <<"cpu instruction \"cache\" : \n" << termcolor::reset;
    for (instruction* i: program)
        i->prettyPrint();
    cout << termcolor::green << "cpu data \"cache\" : \n" << termcolor::reset;

    //lets print 16 * 16 = 256 bytes
    for (size_t i = 0; i < 16;i++)
    {
      printf("0x%02lx: ", i);
        for (size_t j = 0; j< 16;j++)
        {
          char d = data[16*i + j];
          if (!d){
            printf("%02x ", d);
          }else{
            cout << termcolor::red;
            printf("%02x ", d);
            cout << termcolor::reset;
          }
        }
      printf("\n");
    }


  }
};
