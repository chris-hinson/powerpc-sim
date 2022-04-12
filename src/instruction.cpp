//this file defines all 9 of our possible operations
//parsing regex including capturing groups
//(\w+:)?\s?(fld|fsd|add|addi|fadd|fsub|fmul|fdiv|bne)\s(?:(\w*)\s*,\s*(?:(\w*)\s*,\s*(-?\w*))|(?:(\w*)\s*,\s*(\d*)\((\w*)\)))\n



using namespace std;

enum opClass { memory, arithmetic, control };
enum memType {load, store};
enum arthType {add, addi, fadd, fsub, fmul, fdiv};

/////////////////////////////parent struct//////////////////////////////////////
struct instruction {
public:
  size_t address;
  opClass type;
  virtual void prettyPrint() = 0;
};
////////////////////////////////////////////////////////////////////////////////


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
    cout << "fld " << dst_reg << ", " << offset << "(" << src_reg << ")" << "\n";
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
    cout << "fsd " << src_reg << ", " << offset << "(" << dst_reg << ")"<<"\n";
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
    cout << "add\n";
  }
};
////////////////////////////////////////////////////////////////////////////////
