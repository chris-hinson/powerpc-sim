#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
//#include <string>
#include "cpu.cpp"
using namespace std;


//abandon all hope ye who enter here.
//I do not fully grasp c++. this codebase will look like a godforsaken mess of C
// code with C++ stl usage as close to Rust syntax as possible. I'm truly sorry 
int main(int argc, char *argv[]) {

    if (argc < 2)
    {
      printf("please enter a program to run \n");
      return -1;
    }


    cpu new_cpu = cpu(argv[1]);
    new_cpu.prettyPrint();

    //printf("%spress enter key to begin execution...%s",termcolor::bright_blue,termcolor::reset);
    cout << termcolor::yellow << "press any key to begin execution..." <<termcolor::reset;
    std::cin.ignore();
    cout << endl;

    new_cpu.step();



    return 0;
}
