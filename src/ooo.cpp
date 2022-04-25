//this file details the main structures we need for OOO execution,
//namely our reservation stations, ROB, and register status table

#include "instruction.cpp"
//#include "registers.cpp"

struct Nothing{};
union actual_val{
  int i;
  float f;
  Nothing n;
};

enum rob_status{empty,issued,executing,committed};

struct ROB_entry{
  //is this entry occupied?
  bool busy;
  //what instructino is this entry keeping track of for us
  instruction* inst;
  //what stage is this instruction at?
  rob_status state;
  //ok this one is important:
  //keeps track of what PHYSICAL register this instruction will update when it commits
  //this means that when an instruction gets put into a reservation station,
  //if its using that reg, it must wait for this entry to commit
  int dest;
  //this field contains the actual value that will be commited
  actual_val val;

  ROB_entry(){
    busy = false;
    state = rob_status::empty;
    dest = 0;
    val = {.n{}};
  }
};

//the ROB is a circular buffer of ROB entries.
struct ROB{
  std::vector<ROB_entry> entries;

  ROB(){
    for(size_t i = 0 ; i < 16;i++)
      entries.push_back(ROB_entry());
  }

  //returns whether there is an available rob entry
  bool available(){
    for (ROB_entry r: entries)
    {
      if (r.state == rob_status::empty){
        return true;
      }
    }
    return false;
  }

  //inserts an instruction in the first available ROB slot
  void insert()
  {
    for (ROB_entry r: entries)
    {
      if (r.state == rob_status::empty)
      {
        //insert here
      }
    }
  }

  //void commit()

};



enum rs_ops {integer,load,store,fp_add,fp_mult,fp_div,bu};

struct reservation_station{
  //the literal name of the station (add1,add2,load1 etc)
  string name;
  //is this reservation station in use?S
  bool busy;
  //what operation this fu performs
  rs_ops op;
  //values for the actual op -
  actual_val vj;
  actual_val vk;
  //locations values are coming from
  int qj;
  int qk;
  //destination we will be writing to
  int dst;
  //address (needed for memory ops)
  int a;

  reservation_station(string iname, rs_ops o){
    name = iname;
    busy = false;
    op = o;
    vj = {.n{}};
    vk = {.n{}};
    qj = -1;
    qk = -1;
    dst = -1;
    a=-1;
  }

};

//this struct represents all of our functional units' reservation stations
struct rs_file{
  //4 int
  std::vector<reservation_station> ints;
  //2 load
  std::vector<reservation_station> loads;
  //2 store
  std::vector<reservation_station> stores;
  //3 fpadd
  std::vector<reservation_station> fpadds;
  //4 fpmult
  std::vector<reservation_station> fpmults;
  //2 fpdiv
  std::vector<reservation_station> fpdivs;
  //1 branch
  std::vector<reservation_station> branch;

  rs_file(){
    ints.push_back(reservation_station("int1",rs_ops::integer));
    ints.push_back(reservation_station("int2",rs_ops::integer));
    ints.push_back(reservation_station("int3",rs_ops::integer));
    ints.push_back(reservation_station("int4",rs_ops::integer));

    loads.push_back(reservation_station("load1",rs_ops::load));
    loads.push_back(reservation_station("load2",rs_ops::load));

    stores.push_back(reservation_station("store1",rs_ops::store));
    stores.push_back(reservation_station("store2",rs_ops::store));

    fpadds.push_back(reservation_station("fpadd1",rs_ops::fp_add));
    fpadds.push_back(reservation_station("fpadd2",rs_ops::fp_add));
    fpadds.push_back(reservation_station("fpadd3",rs_ops::fp_add));

    fpmults.push_back(reservation_station("fpmult1",rs_ops::fp_mult));
    fpmults.push_back(reservation_station("fpmult2",rs_ops::fp_mult));
    fpmults.push_back(reservation_station("fpmult3",rs_ops::fp_mult));
    fpmults.push_back(reservation_station("fpmult4",rs_ops::fp_mult));

    fpdivs.push_back(reservation_station("fpdiv1",rs_ops::fp_div));
    fpdivs.push_back(reservation_station("fpdiv2",rs_ops::fp_div));

    branch.push_back(reservation_station("branch1",rs_ops::bu));


  }

  //this function will put an instr into a rs if an appropriate one is available,
  //or return false if it cannot currently do so
  bool issue(instruction instr, ROB* r){
    //first just see if we have a rob slot open and return early if we dont
    bool rob_available = r->available();
    if (!rob_available)
      return false;


    //if any of this instructions fields will be coming from a ROB, we need to know


    //now we need to check for an open rs of an appropriate kind for this instr
    if(instr.opcode == op::add || instr.opcode == op::addi)
    {
      //integer station
      for (reservation_station r: ints)
      {
        if (!r.busy)
        {
          //put our instr in this rs and give it a ROB entry
          return true;
        }
        //if we didnt find a station, return false
        return false;
      }
    }
    else if (instr.opcode == op::fld)
    {
      //load station
      for (reservation_station r: loads)
      {
        if (!r.busy )
        {
          //put our instr in this rs
          return true;
        }
        //if we didnt find a station, return false
        return false;
      }
    }
    else if (instr.opcode == op::fsd)
    {
      //store station
      for (reservation_station r: stores)
      {
        if (!r.busy )
        {
          //put our instr in this rs
          return true;
        }
        //if we didnt find a station, return false
        return false;
      }
    }
    else if (instr.opcode == op::fadd || instr.opcode == op::fsub)
    {
      //fp add
      for (reservation_station r: fpadds)
      {
        if (!r.busy)
        {
          //put our instr in this rs
          return true;
        }
        //if we didnt find a station, return false
        return false;
      }
    }
    else if (instr.opcode == op::fmul)
    {
      //fp mul
      for (reservation_station r: fpmults)
      {
        if (!r.busy)
        {
          //put our instr in this rs
          return true;
        }
        //if we didnt find a station, return false
        return false;
      }
    }
    else if (instr.opcode == op::fdiv)
    {
      //fp div
      for (reservation_station r: fpdivs)
      {
        if (!r.busy)
        {
          //put our instr in this rs
          return true;
        }
        //if we didnt find a station, return false
        return false;
      }
    }
    else
    {
      //branch
      for (reservation_station r: branch)
      {
        if (!r.busy)
        {
          //put our instr in this rs
          return true;
        }
        //if we didnt find a station, return false
        return false;
      }
    }
  }
};

//keep track of where our physical regs "are"
//- i.e. when we read them where are we reading from
//can be in the rf, rob, or a cdb
enum reg_location{rf,rob};
struct reg_status{
  std::map<hrd_reg,reg_location> locs;

  reg_status(){
    for (size_t i=0;i<32;i++)
    {
      locs[(hrd_reg)i] = reg_location::rf;
    }
  }
};
