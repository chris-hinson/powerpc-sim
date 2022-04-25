//this file represents our register file
//we have 32 32 bit registers
//we need an architected to physcical map table as well as a free list


//each register can hold either an int or a float but must be 32 bit
union val{
  int i;
  float f;
};

//architected regs
enum reg {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r13,r14,r15,r16,r17,r18,r19,r20,r21,r22,r23,r24,r25,r26,r27,r28,r29,r30,r31,r32,f0,f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,null};
std::vector<std::string> regNames = {"R0","R1","R2","R3","R4","R5","R6","R7","R8","R9","R10","R11","R12","R13","R14","R15","R16","R17","R18","R19","R20","R21","R22","R23","R24","R25","R26","R27","R28","R29","R30","R31","R32","F0","F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11","F12","F13","F14","F15","F16","F17","F18","F19","F20","F21","F22","F23","F24","F25","F26","F27","F28","F29","F30","F31","F32","null"};

//this encompasses our register file regs, as well as our
enum hrd_reg {p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12,p13,p14,p15,p16,p17,p18,p19,p20,p21,p22,p23,p24,p25,p26,p27,p28,p29,p30,p31,p32,hrd_null};
std::vector<std::string> hrd_regNames = {"p0","p1","p2","p3","p4","p5","p6","p7","p8","p9","p10","p11","p12","p13","p14","p15","p16","p17","p18","p19","p20","p21","p22","p23","p24","p25","p26","p27","p28","p29","p30","p31","p32","hrd_null"};


reg str_to_reg(std::string s){
  for (size_t i = 0 ; i < regNames.size();i++)
  {
    if (!s.compare(regNames[i]))
    {
      return (reg)i;
    }
  }

  std::cout << termcolor::red << "RETURNING A NULL REG THIS WILL PROBABLY BREAK SOMETHING" << termcolor::reset << std::endl;
  return reg::null;
}




struct register_file{
  //our actual register file. associates a hardware register with a val (this val is a union of int and float)
  //initial state = full zeros (int mode bc fuck it i guess)
  std::map<hrd_reg,val>rf;
  //our map table, architected registers to their current hardware register
  //initial state: every architected reg goes to physcical null register
  std::map<reg,std::vector<hrd_reg>> mapping;
  //free list - list of currently unused hardware regs
  //initial state: every hardware register is free
  std::vector<hrd_reg> free_list;

  register_file(){
    //give every register a 0
    for (size_t i = 0 ; i < 32;i++)
      rf[(hrd_reg)i] = {0};
    //give every entry in our map table an empty vector containing just the nul hardware reg
    std::vector<hrd_reg> empty{hrd_reg::hrd_null};
    for (size_t j = 0; j < 64;j++)
      mapping[(reg)j] = empty;
    //every hardware register is free to start
    for (size_t k = 0;k<32;k++)
      free_list.push_back((hrd_reg)k);
    //congratulations we have a register file
  };

  //translate an architected register to a physical one according to our mapping and free list
  //we need to know if this is a write so we know if we need to update the mapping
  hrd_reg get_reg(reg r, bool is_write){
    //get the current mapping
    hrd_reg h = mapping[r].back();

    //if the mapping is null, we need to give it an actual mapping
    if (h == hrd_reg::hrd_null){
      //put the reg from the front of the free list at the end of the map list
      mapping[r].push_back(free_list.front());
      //make sure to remove the reg from the free list
      free_list.erase(free_list.begin());
      h =  mapping[r].back();
    }
    //if this is a write, we need to put a new mapping in the list for next time
    if (is_write){
      mapping[r].push_back(free_list.front());
      free_list.erase(free_list.begin());
    }
    return h;
  }

  void prettyPrint(){
    std::cout <<termcolor::green << "register file" << termcolor::reset << std::endl;

    for(size_t l =0 ; l < 16; l++)
    {
      //printf("%s:%d/%f %s:%d/%f\t%s:%s%s:%s %s:%s%s:%s",
      std::cout <<
      hrd_regNames[l] << ":" << rf[(hrd_reg)l].i << "/" << rf[(hrd_reg)l].f << "  "<<
      hrd_regNames[l+16]<< ":" << rf[(hrd_reg)(l+16)].i << "/" << rf[(hrd_reg)(l+16)].f << "        " <<
      regNames[l] << ":" << hrd_regNames[(int)mapping[(reg)l].back()] << " " <<
      regNames[l+15]<< ":" << hrd_regNames[(int)mapping[(reg)(l+15)].back()]<< " " <<
      regNames[l+32]<< ":" << hrd_regNames[(int)mapping[(reg)(l+32)].back()]<< " " <<
      regNames[l+48]<< ":" << hrd_regNames[(int)mapping[(reg)(l+48)].back()] << std::endl;
      //);
    }

    std::cout << "free list: ";
    for(auto i:free_list)
      std::cout<< hrd_regNames[i] << ", ";
    std::cout << std::endl;

  }

};
