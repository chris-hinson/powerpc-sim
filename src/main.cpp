#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
//#include <string>
#include "cpu.cpp"
using namespace std;

int main() {



    cpu new_cpu = cpu("../programs/handout.txt");
    new_cpu.prettyPrint();

    return 0;
}
