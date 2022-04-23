//this file represents our register file
//we have 32 32 bit registers
//we need an architected to physcical map table as well as a free list

//using namespace std;

//each register can hold either an int or a float but must be 32 bit
enum reg {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r13,r14,r15,r16,r17,r18,r19,r20,r21,r22,r23,r24,r25,r26,r27,r28,r29,r30,r31,r32,f0,f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31,f32,null};
std::vector<std::string> regNames = {"R0","R1","R2","R3","R4","R5","R6","R7","R8","R9","R10","R11","R12","R13","R14","R15","R16","R17","R18","R19","R20","R21","R22","R23","R24","R25","R26","R27","R28","R29","R30","R31","R32","F0","F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11","F12","F13","F14","F15","F16","F17","F18","F19","F20","F21","F22","F23","F24","F25","F26","F27","F28","F29","F30","F31","F32","null"};

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
