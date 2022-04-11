//this file defines all 9 of our possible operations

using namespace std;

enum opClass { memory, arithmetic, control };
enum memType {load, store};

/////////////////////////////parent struct//////////////////////////////////////
struct instruction {
public:
  size_t address;
  opClass type;
  virtual void prettyPrint() = 0;
};
////////////////////////////////////////////////////////////////////////////////


/////////////////////////////instruction classes////////////////////////////////
struct memoryOp: instruction{
public:
  memType op;
  size_t reg;
  size_t addr;
};
////////////////////////////////////////////////////////////////////////////////


/////////////////////////////actual instructions////////////////////////////////
struct fld: memoryOp{
  fld(size_t instr_addr, size_t reg, size_t data_addr){
    address = instr_addr;
    type = memory;
    op = load;
    reg = reg;
    addr = data_addr;
  }

  void prettyPrint(){
    cout << "fld into " << reg << "from" << addr << "\n";
  }
};
struct fsd: memoryOp{
  fsd (size_t instr_addr, size_t reg, size_t data_addr){
    address = instr_addr;
    type = memory;
    op = store;
    reg = reg;
    addr = data_addr;
  }

  void prettyPrint(){
    cout << "constructing fsd from " << reg << "to" << addr << "\n";
  }
};
////////////////////////////////////////////////////////////////////////////////
