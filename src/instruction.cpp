//this file defines all 9 of our possible operations
//parsing regex including capturing groups
//(\w+:)?\s?(fld|fsd|add|addi|fadd|fsub|fmul|fdiv|bne)\s(?:(\w*)\s*,\s*(?:(\w*)\s*,\s*(-?\w*))|(?:(\w*)\s*,\s*(\d*)\((\w*)\)))
#include <regex>
using namespace std;

#define STRIP_NON_DIGIT(source) source.erase(std::remove_if(source.begin(), source.end(),[](char c) { return !std::isdigit(c); }),source.end());
//#define STRIP_NON_DIGIT(str) str.erase(remove_if(str.begin(), str.end(), ::isdigit), str.end())
#define TO_NUM(str) atoi(str.c_str())
enum op {bad, fld, fsd, add, addi, fadd, fsub, fmul, fdiv, bne};
const char* opNames[] = {"bad", "fld", "fsd", "add", "addi", "fadd", "fsub", "fmul", "fdiv", "bne"};

string re = R"((\w+:)?\s?(fld|fsd|add|addi|fadd|fsub|fmul|fdiv|bne)\s(?:(\w*)\s*,\s*(?:(\w*)\s*,\s*(-?\w*))|(?:(\w*)\s*,\s*(\d*)\((\w*)\))))";
std::regex instr_regex(re);

////////////////////////////////////////////////////////////////////////////////
//yes this is a stupid way to do this.  OOP kicked my ass so here we are
struct instruction {
  size_t address;
  op opcode;
  vector<int> fields;

  void prettyPrint(){
    printf("%#lx: %s ",address,opNames[opcode]);
    for (int i: fields)
    {
      printf("%d, ",i);
    }
    printf("\n");
  }
};
////////////////////////////////////////////////////////////////////////////////

instruction* parse(string op, size_t i_addr, unordered_map<string,size_t>* lbls){

  //source.erase(std::remove_if(source.begin(), source.end(),[](char c) { return !std::isdigit(c); }),source.end());

  //cout << "parsing " << op << "\n";
  smatch match;
  regex_match(op, match, instr_regex);
  /*if (regex_match(op, match, instr_regex)) {

      cout << termcolor::green << "Match size = " << match.size() << endl <<termcolor::reset;

      for (size_t i = 0; i < match.size(); i++) {
        cout << termcolor::yellow << match.str(i) << endl << termcolor::reset;
      }

  }
  else {
      cout << termcolor::red << "No match is found. bad op?" << endl << termcolor::reset;
      vector<int> v;
      return new instruction{0,op::bad, v};
  }*/


  string decodeOP = match.str(2);
  //cout << "decoding on " << decodeOP << endl;

  //god i miss rust pattern matching
  if (!decodeOP.compare("fld")) {
    string dst = match.str(6);
    string offset = match.str(7);
    string src = match.str(8);

    STRIP_NON_DIGIT(dst);
    STRIP_NON_DIGIT(offset);
    STRIP_NON_DIGIT(src);

    vector<int> v{ TO_NUM(dst), TO_NUM(offset), TO_NUM(src) };
    return new instruction{i_addr,op::fld, v};
  }
  else if (!decodeOP.compare("fsd")) {
    string src = match.str(6);
    string offset = match.str(7);
    string dst = match.str(8);

    STRIP_NON_DIGIT(src);
    STRIP_NON_DIGIT(offset);
    STRIP_NON_DIGIT(dst);

    vector<int> v{ TO_NUM(src), TO_NUM(offset), TO_NUM(dst) };
    return new instruction{i_addr,op::fsd, v};
  }
  else if (!decodeOP.compare("add")) {
    //  add(size_t instr_addr, size_t dst_reg, size_t src_reg1, size_t src_reg2){
    string dst = match.str(3);
    string src1 = match.str(4);
    string src2 = match.str(5);

    STRIP_NON_DIGIT(dst);
    STRIP_NON_DIGIT(src1);
    STRIP_NON_DIGIT(src2);

    vector<int> v{ TO_NUM(dst), TO_NUM(src1), TO_NUM(src2) };
    return new instruction{i_addr,op::add, v};
  }
  else if (!decodeOP.compare("fadd")) {
    string dst = match.str(3);
    string src1 = match.str(4);
    string src2 = match.str(5);

    STRIP_NON_DIGIT(dst);
    STRIP_NON_DIGIT(src1);
    STRIP_NON_DIGIT(src2);

    vector<int> v{ TO_NUM(dst), TO_NUM(src1), TO_NUM(src2) };
    return new instruction{i_addr,op::fadd, v};
  }
  else if (!decodeOP.compare("fsub")) {
    string dst = match.str(3);
    string src1 = match.str(4);
    string src2 = match.str(5);

    STRIP_NON_DIGIT(dst);
    STRIP_NON_DIGIT(src1);
    STRIP_NON_DIGIT(src2);

    vector<int> v{ TO_NUM(dst), TO_NUM(src1), TO_NUM(src2) };
    return new instruction{i_addr,op::fsub, v};
  }
  else if (!decodeOP.compare("fmul")) {
    string dst = match.str(3);
    string src1 = match.str(4);
    string src2 = match.str(5);

    STRIP_NON_DIGIT(dst);
    STRIP_NON_DIGIT(src1);
    STRIP_NON_DIGIT(src2);

    vector<int> v{ TO_NUM(dst), TO_NUM(src1), TO_NUM(src2) };
    return new instruction{i_addr,op::fmul, v};
  }
  else if (!decodeOP.compare("fdiv")) {
    string dst = match.str(3);
    string src1 = match.str(4);
    string src2 = match.str(5);

    STRIP_NON_DIGIT(dst);
    STRIP_NON_DIGIT(src1);
    STRIP_NON_DIGIT(src2);

    vector<int> v{ TO_NUM(dst), TO_NUM(src1), TO_NUM(src2) };
    return new instruction{i_addr,op::fdiv, v};
  }
  else if (!decodeOP.compare("addi")) {
    string dst = match.str(3);
    string src1 = match.str(4);
    int src2 = atoi(match.str(5).c_str());

    STRIP_NON_DIGIT(dst);
    STRIP_NON_DIGIT(src1);

    vector<int> v{ TO_NUM(dst), TO_NUM(src1), src2};
    return new instruction{i_addr,op::addi, v};
  }
  else if (!decodeOP.compare("bne")) {
    string dst = match.str(3);
    string src1 = match.str(4);
    int src2 = (*lbls)[match.str(5)]-i_addr;

    STRIP_NON_DIGIT(dst);
    STRIP_NON_DIGIT(src1);

    vector<int> v{ TO_NUM(dst), TO_NUM(src1), src2};
    return new instruction{i_addr,op::bne, v};
  }
  else{
    cout << "bad op BUT WE SHOULD NEVER GET HERE" << endl;
    vector<int> v;
    return new instruction{0,op::bad, v};
  }

  vector<int> v;
  return new instruction{0,op::bad, v};
}
