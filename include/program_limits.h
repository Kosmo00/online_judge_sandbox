#ifndef PROGRAM_LIMITS_H
#define PROGRAM_LIMITS_H

#include <sys/resource.h>

typedef rlim_t ResourceLimit;

class ProgramLimits
{
    public:

    ProgramLimits(int wallclockTime, int cpuTime, long ramMemory, long diskMemory);

    ResourceLimit quotaWallclockTime;    // time since the process started (ms)
    ResourceLimit quotaCpuTime;          // cpu time usage by user + kernel (ms)
    ResourceLimit quotaRamMemory;        // ram usage limit (bytes)
    ResourceLimit quotaDiskMemory;       // output size limit (bytes)
};

#endif