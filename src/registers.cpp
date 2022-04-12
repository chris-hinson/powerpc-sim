//this file represents our register file
//we have 32 32 bit registers
//we need an architected to physcical map table as well as a free list

//each register can hold either an int or a float but must be 32 bit
union reg{
  std::uint32_t i;
  float f;
};

reg rf [32];
