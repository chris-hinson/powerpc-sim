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
class instruction {
public:
  size_t address;
  opClass type;
  void prettyPrint(){
    cout << "please dont instantiate me\n";
  };

  instruction(size_t addr, opClass opC)
  {
    address = addr;
    type = opC;
  }
};
////////////////////////////////////////////////////////////////////////////////
class badOP: public instruction{
public:
  badOP: instruction(0,opClass::bad){}

  void prettyPrint(){
    std::cout << "this is a bad op" << '\n';
  }
};

/////////////////////////////instruction classes////////////////////////////////
//mem ops will always be register-offset addressing
//they need a destination, and a source register, as well as an offset value
//depending on their mem direction, the offset applies to either src or dest
class memoryOp: instruction{
public:
  memType op;
  size_t src_reg;
  size_t dst_reg;
  int offset;
};

//with the exception of addi, all arithmetic ops are of the form
//op dest, src1,src2
//addi replaces src2 with an immeadiate encoded within the instruction itself
//TODO: how can we represent either a register or an imm for src2. union?
class arithmeticOp: instruction {
public:
  arthType op;
  size_t dest;
  size_t src1;
  size_t src2;
};

//bne should compare two registers and jump to the specified label if they are
//not equal.  however we will also support comparing against an immeadiate
//TODO: check if we should decompose into two ops when comp against imm
class branchOp: instruction {
public:
  size_t reg;
  size_t comp;
  int offset;
};


////////////////////////////////////////////////////////////////////////////////


/////////////////////////////actual instructions////////////////////////////////
class fld: memoryOp{
  fld(size_t instr_addr, size_t dst_reg, size_t src_reg, int offset){
    this->address = instr_addr;
    type = memory;
    op = memType::load;
    src_reg = src_reg;
    dst_reg = dst_reg;
    offset = offset;
  }

  void prettyPrint(){
    //cout << std::hex << address << ": fld " << dst_reg << ", " << offset << "(" << src_reg << ")" << "\n";
    printf("%#lx: fld %ld , %d(%ld)\n",address,dst_reg,offset,src_reg);
  }
};
class fsd: memoryOp{
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



class add:arithmeticOp{
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

class fadd:arithmeticOp{
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
class fsub:arithmeticOp{
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
class fmul:arithmeticOp{
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
class fdiv:arithmeticOp{
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
class addi:arithmeticOp{
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
class bne:branchOp{
  bne(size_t instr_addr, size_t src_reg, size_t comp_reg, long int offset )
  {
    address = instr_addr;
    type = control;
    reg = src_reg;
    comp = comp_reg;
    offset = offset;
  }
  void prettyPrint(){
    cout << std::hex << address << ": bne " << reg << ", " << comp << ", " << offset << "\n";
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
    //  fld(size_t instr_addr, size_t dst_reg, size_t src_reg, size_t offset){
    cout << match.str(6) << "," << match.str(8) << ", " << match.str(7);
    size_t dst = ((int)match.str(6).at(1)) - 48;
    size_t src = ((int)match.str(8).at(1)) - 48;
    int offset = atoi(match.str(7).c_str());

    printf("\n\tinstantiating fld op - dst: %ld src: %ld offset: %d\n",dst,src,offset);

    fld* blech = new fld(i_addr, dst, src, offset);

    return blech;
  }
  else if (!decodeOP.compare("fsd")) {
    //  fsd (size_t instr_addr, size_t src_reg, size_t dst_reg, size_t offset){
    //return new fsd(i_addr, match.str())
  }
  else if (!decodeOP.compare("add")) {
    cout << "add op" << endl;
    //  add(size_t instr_addr, size_t dst_reg, size_t src_reg1, size_t src_reg2){
  }
  else if (!decodeOP.compare("fadd")) {
    //  fadd(size_t instr_addr, size_t dst_reg, size_t src_reg1, size_t src_reg2){
    cout << "fadd op" << endl;
  }
  else if (!decodeOP.compare("fsub")) {
    //  fsub(size_t instr_addr, size_t dst_reg, size_t src_reg1, size_t src_reg2){
    cout << "fsub op" << endl;
  }
  else if (!decodeOP.compare("fmul")) {
    //  fmul(size_t instr_addr, size_t dst_reg, size_t src_reg1, size_t src_reg2){
    cout << "fmul op" << endl;
  }
  else if (!decodeOP.compare("fdiv")) {
    //  fdiv(size_t instr_addr, size_t dst_reg, size_t src_reg1, size_t src_reg2){
    cout << "fdiv op" << endl;
  }
  else if (!decodeOP.compare("addi")) {
    //  addi(size_t instr_addr, size_t dst_reg, size_t src_reg1, size_t src_reg2){
    cout << "addi op" << endl;
  }
  else if (!decodeOP.compare("bne")) {
    //  bne(size_t instr_addr, size_t src_reg, size_t comp_reg, long int offset )
    cout << "bne op" << endl;
  }
  else{
    cout << "bad op BUT WE SHOULD NEVER GET HERE" << endl;
    return new badOP;
  }

return new badOP;

}
