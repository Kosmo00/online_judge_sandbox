#ifndef PROGRAM_LIMITS_CPP
#define PROGRAM_LIMITS_CPP

#include "../include/program_limits.h"

ProgramLimits::ProgramLimits(int wallclockTime, int cpuTime, long ramMemory, long diskMemory)
{
    quotaWallclockTime = wallclockTime;
    quotaCpuTime = cpuTime;
    quotaRamMemory = ramMemory;
    quotaDiskMemory = diskMemory;
}



#endif