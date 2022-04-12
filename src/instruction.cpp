//this file defines all 9 of our possible operations
//parsing regex including capturing groups
//(\w+:)?\s?(fld|fsd|add|addi|fadd|fsub|fmul|fdiv|bne)\s(?:(\w*)\s*,\s*(?:(\w*)\s*,\s*(-?\w*))|(?:(\w*)\s*,\s*(\d*)\((\w*)\)))
#include <regex>

using namespace std;

enum opClass { memory, arithmetic, control, bad};
enum memType {load, store};
enum arthType {add, addi, fadd, fsub, fmul, fdiv};

string re = R"((\w+:)?\s?(fld|fsd|add|addi|fadd|fsub|fmul|fdiv|bne)\s(?:(\w*)\s*,\s*(?:(\w*)\s*,\s*(-?\w*))|(?:(\w*)\s*,\s*(\d*)\((\w*)\))))";
std::regex instr_regex(re);

/////////////////////////////parent struct//////////////////////////////////////
struct instruction {
public:
  size_t address;
  opClass type;
  virtual void prettyPrint() = 0;
};
////////////////////////////////////////////////////////////////////////////////
struct badOP: instruction{
public:
  badOP(){
    address = 0;
    type = opClass::bad;
  }

  void prettyPrint(){
    std::cout << "this is a bad op" << '\n';
  }
};

/////////////////////////////instruction classes////////////////////////////////
//mem ops will always be register-offset addressing
//they need a destination, and a source register, as well as an offset value
//depending on their mem direction, the offset applies to either src or dest
struct memoryOp: instruction{
public:
  memType op;
  size_t src_reg;
  size_t dst_reg;
  size_t offset;
};

//with the exception of addi, all arithmetic ops are of the form
//op dest, src1,src2
//addi replaces src2 with an immeadiate encoded within the instruction itself
//TODO: how can we represent either a register or an imm for src2. union?
struct arithmeticOp: instruction {
public:
  arthType op;
  size_t dest;
  size_t src1;
  size_t src2;
};

//bne should compare two registers and jump to the specified label if they are
//not equal.  however we will also support comparing against an immeadiate
//TODO: check if we should decompose into two ops when comp against imm
struct branchOp: instruction {
public:
  size_t reg;
  size_t comp;
  size_t label;
};


////////////////////////////////////////////////////////////////////////////////


/////////////////////////////actual instructions////////////////////////////////
struct fld: memoryOp{
  fld(size_t instr_addr, size_t dst_reg, size_t src_reg, size_t offset){
    address = instr_addr;
    type = memory;
    op = memType::load;
    src_reg = src_reg;
    dst_reg = dst_reg;
    offset = offset;
  }

  void prettyPrint(){
    cout << std::hex << address << ": fld " << dst_reg << ", " << offset << "(" << src_reg << ")" << "\n";
  }
};
struct fsd: memoryOp{
  fsd (size_t instr_addr, size_t src_reg, size_t dst_reg, size_t offset){
    address = instr_addr;
    type = memory;
    op = memType::store;
    src_reg = src_reg;
    dst_reg = dst_reg;
    offset = offset;
  }

  void prettyPrint(){
    cout << std::hex << ": fsd " << src_reg << ", " << offset << "(" << dst_reg << ")"<<"\n";
  }
};



struct add:arithmeticOp{
  add(size_t instr_addr, size_t dst_reg, size_t src_reg1, size_t src_reg2){
    address = instr_addr;
    type = arithmetic;
    op = arthType::add;
    dest = dst_reg;
    src1 = src_reg1;
    src2 = src_reg2;

  }
  void prettyPrint(){
    cout << std::hex << address << ": add " << dest << "," << src1 << "," <<  src2 << "\n";
  }
};

struct fadd:arithmeticOp{
  fadd(size_t instr_addr, size_t dst_reg, size_t src_reg1, size_t src_reg2){
    address = instr_addr;
    type = arithmetic;
    op = arthType::fadd;
    dest = dst_reg;
    src1 = src_reg1;
    src2 = src_reg2;

  }
  void prettyPrint(){
    cout << std::hex << address << ": fadd " << dest << "," << src1 << "," <<  src2 << "\n";
  }
};
struct fsub:arithmeticOp{
  fsub(size_t instr_addr, size_t dst_reg, size_t src_reg1, size_t src_reg2){
    address = instr_addr;
    type = arithmetic;
    op = arthType::fsub;
    dest = dst_reg;
    src1 = src_reg1;
    src2 = src_reg2;

  }
  void prettyPrint(){
    cout << std::hex << address << ": fsub " << dest << "," << src1 << "," <<  src2 << "\n";
  }
};
struct fmul:arithmeticOp{
  fmul(size_t instr_addr, size_t dst_reg, size_t src_reg1, size_t src_reg2){
    address = instr_addr;
    type = arithmetic;
    op = arthType::fmul;
    dest = dst_reg;
    src1 = src_reg1;
    src2 = src_reg2;

  }
  void prettyPrint(){
    cout << std::hex << address << ": fmul " << dest << "," << src1 << "," <<  src2 << "\n";
  }
};
struct fdiv:arithmeticOp{
  fdiv(size_t instr_addr, size_t dst_reg, size_t src_reg1, size_t src_reg2){
    address = instr_addr;
    type = arithmetic;
    op = arthType::fdiv;
    dest = dst_reg;
    src1 = src_reg1;
    src2 = src_reg2;

  }
  void prettyPrint(){
    cout << std::hex << address << ": fdiv " << dest << "," << src1 << "," <<  src2 << "\n";
  }
};
struct addi:arithmeticOp{
  addi(size_t instr_addr, size_t dst_reg, size_t src_reg1, size_t src_reg2){
    address = instr_addr;
    type = arithmetic;
    op = arthType::addi;
    dest = dst_reg;
    src1 = src_reg1;
    src2 = src_reg2;

  }
  void prettyPrint(){
    cout << std::hex << address << ": addi " << dest << "," << src1 << "," <<  src2 << "\n";
  }
};
////////////////////////////////////////////////////////////////////////////////

instruction* parse(string op, size_t i_addr){

  cout << "parsing " << op << "\n";
  smatch match;
  if (regex_match(op, match, instr_regex)) {

      cout << termcolor::green << "Match size = " << match.size() << endl <<termcolor::reset;

      for (size_t i = 0; i < match.size(); i++) {
        cout << termcolor::yellow << match.str(i) << endl << termcolor::reset;
      }

  }
  else {
      cout << termcolor::red << "No match is found" << endl << termcolor::reset;
      return new badOP;
  }


  string decodeOP = match.str(2);
  cout << "decoding on " << decodeOP << endl;

  //god i miss rust pattern matching
  if (!decodeOP.compare("fld")) {
    //return new fld(i_addr, match.str(6), match.str(8),match.str(7));
  }
  else if (!decodeOP.compare("fsd")) {
    cout << "fsd op" << endl;
  }
  else if (!decodeOP.compare("add")) {
    cout << "add op" << endl;
  }
  else if (!decodeOP.compare("fadd")) {
    cout << "fadd op" << endl;
  }
  else if (!decodeOP.compare("fsub")) {
    cout << "fsub op" << endl;
  }
  else if (!decodeOP.compare("fmul")) {
    cout << "fmul op" << endl;
  }
  else if (!decodeOP.compare("fdiv")) {
    cout << "fdiv op" << endl;
  }
  else if (!decodeOP.compare("addi")) {
    cout << "addi op" << endl;
  }
  else if (!decodeOP.compare("bne")) {
    cout << "bne op" << endl;
  }
  else{
    cout << "bad op BUT WE SHOULD NEVER GET HERE" << endl;
    return new badOP;
  }

return new badOP;

}
