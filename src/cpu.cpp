#include <cstddef>
#include <stdlib.h>
#include <unordered_map>
#include "termcolor.hpp"
#include "instruction.cpp"
//#include "registers.cpp"
using namespace std;



class cpu{
public:
  int ticks;
  string prgName;
  vector<instruction*> program;
  vector<char> data;
  size_t PC = 0;

  register_file r;

  //we need a place to store the instructions we have fetched
  vector<instruction> fetch_buffer;
  //this is our actual instruction queue (we get to assume it is unlimited)
  vector<instruction> decode_buffer;

  //we need a program file to construct our cpu
  cpu(string filename){

    ////////////////////////////////////////////////////////////////////////////
    PC = 0;

    r = register_file();

    //intialize 4Kb of 0s
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

    //parsing the file from our buffer
    size_t i = 0;

    for (; i < linebuf.size(); i++) {
      //we can identify data because its lines have a ,
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
    //keep track of the instructino line numbers
    int j = i;
    //we need to update our labels now
    for (auto i: labels) {
      labels[i.first] = i.second-j;
      //cout << "label: " << i.first << ":" << i.second <<endl;
    }

    //now that we have our vector of lines as well as map of labels to addresses
    //lets start parsing
    for (; i < linebuf.size(); i++)
    {
      instruction* instr = parse(linebuf[i], i-j, &labels);
      program.push_back(instr);
    }

    ////////////////////////////////////////////////////////////////////////////


  }


  vector<instruction> fetch()
  {
    vector <instruction> fetched;
    vector<reg> reads;
    vector<reg> writes;

    for (size_t i = 0; i < 4;i++){

      //dont let us segfault trying to fetch off the end of the i-cache
      if (i >= program.size()){
        cout << "cant fetch off end of i cache";
        break;
      }


      instruction cur = *program[PC];
      printf("trying to fetch:");
      cur.prettyPrint();
      vector<reg> cur_read_deps = cur.read_deps();
      vector<reg> cur_write_deps = cur.write_deps();

      //check if we are reading from a value that has already been written in this packet
      for(int j:cur_read_deps){
        if (std::count(writes.begin(), writes.end(), j)) {
          cout << termcolor::red << "RAW found, not pushing this inst, just returning" << termcolor::reset << endl;
          return fetched;
        }
      }

      //if this instruction is ok to push, add its read and write deps to the total read and writes
      reads.insert(reads.end(), cur_read_deps.begin(), cur_read_deps.end());
      writes.insert(writes.end(), cur_write_deps.begin(), cur_write_deps.end());

      //put the fetched instr in the fetched buffer
      fetched.push_back(cur);

      //make sure we update our pc
      PC++;
    }
    return fetched;
  }

  //really what we're doing is turning a list of instructions using architected registers
  //into a list of isntructions using physical registers without and WAWs or WARs
  vector<instruction> decode(vector<instruction> instrs){
    vector<instruction> good;

    for (instruction i: instrs)
    {
      i.rename(&r);
      good.push_back(i);
    }

    return good;
  }

  //step our cpu
  void step(){
    //FETCH
    //fetchup to NF instructions free of RAWs
    vector<instruction> fetched = fetch();
    cout << "we fetched : "<<endl;
    for (instruction i : fetched)
      i.prettyPrint();


      //DEBUGGING ONLY CHRIS YOU NEED TO FUCKING MOVE THIS
      fetch_buffer = fetched;

    //DECODE
    //decode the instructions that were in the feched buffer
    cout << "decoding/renaming" << endl;
    vector<instruction> decoded = decode(fetch_buffer);
    //now that we have decoded the fetch buffer, make sure to move the instructions we fetched this cycle into the fetch buffer

    cout << "renamed: " << endl;
    for(instruction j: decoded)
      j.prettyPrint();

    prettyPrint();


    //ISSUE
    //If a RES station and a ROB are free, issue the instruction to the RES
    //station after reading ready registers and renaming non-ready registers
    //EXECUTION
    //When both operands are ready, then execute; if not ready, watch CDB
    //for result; when both in reservation station, execute (checks RAW)
    //WRITE RESULT(WB)
    //Write on CDB to all awaiting RES stations & send the instruction to the
    //ROB; mark reservation station available.
    //COMMIT (sometimes called graduation)
    //When instruction is at head of ROB, update registers (or memory) with
    //result and free ROB. A miss-predicted branch flushes all non-committed
    //instructions.

  }

  void prettyPrint(){
    //DEBUG - print out string vec to make sure we read program correctly
    cout << termcolor::blue << "PC = " << PC << "\n" << termcolor::reset;
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

    //print our rf state
    r.prettyPrint();
  }
};
