#include <cstddef>
#include <iostream>

#include "../include/sandbox.h"
#include "../include/program_limits.h"

using namespace std;

int main()
{
    char *argv[] = {const_cast<char*>("./submissions/1/code"), NULL};
    char *input = const_cast<char*>("./input/1/1");

    ProgramLimits *limits = new ProgramLimits(30000, 1000, 1024*1024*10, 1024*1024*10);

    Sandbox *sandbox = new Sandbox(argv, input, limits);
    sandbox->start();
    return 0;
}