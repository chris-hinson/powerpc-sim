//this file defines all 9 of our possible operations
//parsing regex including capturing groups
//(\w+:)?\s?(fld|fsd|add|addi|fadd|fsub|fmul|fdiv|bne)\s(?:(\w*)\s*,\s*(?:(\w*)\s*,\s*(-?\w*))|(?:(\w*)\s*,\s*(\d*)\((\w*)\)))
#include <regex>

using namespace std;

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
    // fld(size_t instr_addr, size_t dst_reg, size_t src_reg, size_t offset){
    int dst = ((int)match.str(6).at(1)) - 48;
    int offset = atoi(match.str(7).c_str());
    int src = ((int)match.str(8).at(1)) - 48;

    vector<int> v{ dst, offset, src };
    return new instruction{i_addr,op::fld, v};
  }
  else if (!decodeOP.compare("fsd")) {
    //  fsd (size_t instr_addr, size_t src_reg, size_t dst_reg, size_t offset){
    int src = ((int)match.str(6).at(1)) - 48;
    int offset = atoi(match.str(7).c_str());
    int dst = ((int)match.str(8).at(1)) - 48;

    vector<int> v{ src, offset, dst };
    return new instruction{i_addr,op::fsd, v};
  }
  else if (!decodeOP.compare("add")) {
    //  add(size_t instr_addr, size_t dst_reg, size_t src_reg1, size_t src_reg2){
    int dst = ((int)match.str(3).at(1)) - 48;
    int src1 = ((int)match.str(4).at(1)) - 48;
    int src2 = ((int)match.str(5).at(1)) - 48;

    vector<int> v{ dst, src1, src2 };
    return new instruction{i_addr,op::add, v};
  }
  else if (!decodeOP.compare("fadd")) {
    //  fadd(size_t instr_addr, size_t dst_reg, size_t src_reg1, size_t src_reg2){
    int dst = ((int)match.str(3).at(1)) - 48;
    int src1 = ((int)match.str(4).at(1)) - 48;
    int src2 = ((int)match.str(5).at(1)) - 48;

    vector<int> v{ dst, src1, src2 };
    return new instruction{i_addr,op::fadd, v};
  }
  else if (!decodeOP.compare("fsub")) {
    //  fsub(size_t instr_addr, size_t dst_reg, size_t src_reg1, size_t src_reg2){
    int dst = ((int)match.str(3).at(1)) - 48;
    int src1 = ((int)match.str(4).at(1)) - 48;
    int src2 = ((int)match.str(5).at(1)) - 48;

    vector<int> v{ dst, src1, src2 };
    return new instruction{i_addr,op::fsub, v};
  }
  else if (!decodeOP.compare("fmul")) {
    //  fmul(size_t instr_addr, size_t dst_reg, size_t src_reg1, size_t src_reg2){
    int dst = ((int)match.str(3).at(1)) - 48;
    int src1 = ((int)match.str(4).at(1)) - 48;
    int src2 = ((int)match.str(5).at(1)) - 48;

    vector<int> v{ dst, src1, src2 };
    return new instruction{i_addr,op::fmul, v};
  }
  else if (!decodeOP.compare("fdiv")) {
    //  fdiv(size_t instr_addr, size_t dst_reg, size_t src_reg1, size_t src_reg2){
    int dst = ((int)match.str(3).at(1)) - 48;
    int src1 = ((int)match.str(4).at(1)) - 48;
    int src2 = ((int)match.str(5).at(1)) - 48;

    vector<int> v{ dst, src1, src2 };
    return new instruction{i_addr,op::fdiv, v};
  }
  else if (!decodeOP.compare("addi")) {
    //  addi(size_t instr_addr, size_t dst_reg, size_t src_reg1, size_t src_reg2){
    int dst = ((int)match.str(3).at(1)) - 48;
    int src1 = ((int)match.str(4).at(1)) - 48;
    int src2 = atoi(match.str(5).c_str());

    vector<int> v{ dst, src1, src2 };
    return new instruction{i_addr,op::addi, v};
  }
  else if (!decodeOP.compare("bne")) {
    //  bne(size_t instr_addr, size_t src_reg, size_t comp_reg, long int offset )
    int dst = ((int)match.str(3).at(1)) - 48;
    int src1 = ((int)match.str(4).at(1)) - 48;
    //int src2 = ((int)match.str(5).at(0)) - 48;
    int src2 = (*lbls)[match.str(5)]-i_addr;

    vector<int> v{ dst, src1, src2 };
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
