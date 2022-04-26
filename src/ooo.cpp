//this file details the main structures we need for OOO execution,
//namely our reservation stations, ROB, and register status table
//we also have an array of values to imitate our CDB


#include "instruction.cpp"
//#include "registers.cpp"

struct Nothing{};
union actual_val{
  int i;
  float f;
  Nothing n;
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
  hrd_reg dest;
  //this field contains the actual value that will be commited
  actual_val val;

  ROB_entry(){
    busy = false;
    state = rob_status::empty;
    dest = hrd_reg::hrd_null;
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
  //(can be either a reg file hrd_reg or a rr hrd_reg)
  hrd_reg qj;
  hrd_reg qk;
  //destination we will be writing to (should be a physical register)
  hrd_reg dst;
  //address (needed for memory ops)
  int a;

  reservation_station(string iname, rs_ops o){
    name = iname;
    busy = false;
    op = o;
    vj = {.n{}};
    vk = {.n{}};
    qj = hrd_reg::hrd_null;
    qk = hrd_reg::hrd_null;
    dst = hrd_reg::hrd_null;
    a=-1;
  }

  /*void place(instruction i, reg_status rs){

  }*/

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

  //we also want our register status table to keep track of if physical regs are
  //coming out of the ROB
  reg_status status;

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

    status = reg_status();

  }

  //this function will put an instr into a rs if an appropriate one is available,
  //or return false if it cannot currently do so
  bool issue(instruction instr, ROB* ree){
    //first just see if we have a rob slot open and return early if we dont
    bool rob_available = ree->available();
    if (!rob_available)
      return false;

    bool station_available = false;
    reservation_station* found;
    //figure out if theres an appropriate rs for this kind of instr available
    if(instr.opcode == op::add || instr.opcode == op::addi)
    {
      //integer station
      for (reservation_station r: ints)
      {
        if (!r.busy)
        {
          //put our instr in this rs and give it a ROB entry
          station_available = true;
          found = &r;
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
          station_available = true;
          found = &r;
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
          station_available = true;
          found = &r;
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
          station_available = true;
          found = &r;
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
          station_available = true;
          found = &r;
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
          station_available = true;
          found = &r;
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
          station_available = true;
          found = &r;
        }
        //if we didnt find a station, return false
        return false;
      }
    }

    if (!station_available)
      return false;


    //if any of this instructions fields will be coming from a ROB, we need to
    //note that in our reservation station
    vector<hrd_reg> reads = instr.read_hrd_deps();
    vector<hrd_reg> writes = instr.write_hrd_deps();

    //check all the regs we are READING from with this instr
    for (hrd_reg h:reads)
    {
      //if the register status table says this reg is in the rob
      if (status.locs[h] == reg_location::rob)
      {
        //search the ROB to find what entry it is coming from
        for(ROB_entry r: ree->entries)
        {
          if ((hrd_reg)r.dest == h)
          {
            //replace this read with the appropriate renaming register
            h = (hrd_reg)r.dest;
            break;
          }
        }
      }
    }
    //check all the regs we are WRTING from with this instr
    /*for (hrd_reg h:writes)
    {
      //if the register status table says this reg is in the rob
      if (status.locs[h] == reg_location::rob)
      {
        //search the ROB to find what entry it is coming from
        for(ROB_entry r: ree->entries)
        {
          if ((hrd_reg)r.dest == h)
          {
            //replace this read with the appropriate renaming register
            h = (hrd_reg)r.dest;
            break;
          }
        }
      }
    }*/


      //now we finally have all the info we need to insert this instruction
      /*found->busy = true;
      //values of the operators
      found->vj =
      found->vk =
      //locations of the operators
      //(only used if they are in the ROB)
      found->qj =
      found->qk =
      //location we are WRITING to
      found->dst =
      //address field should only be used by
      found->a =*/


  };
};

  struct CDB{
    std::vector<int> cdb;

    CDB(){
      cdb.push_back(0);
      cdb.push_back(0);
      cdb.push_back(0);
      cdb.push_back(0);
    }
  };

  struct speculative{
    bool predictor;
    std::map<int, int> BTB;

    speculative(){
      predictor = false;
    }
  };
