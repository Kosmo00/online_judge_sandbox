#ifndef EVALUATED_PROGRAM_H
#define EVALUATED_PROGRAM_H

#include <sys/resource.h>

typedef rlim_t ResourceLimit;

class EvaluatedProgram 
{
    public:
    EvaluatedProgram(int inFileDescriptor, int outFileDescriptor, int errFileDescriptor, char *argv[]);

    int run();

    ~EvaluatedProgram();

    private:
    int mInFileDescriptor;                           
    int mOutFileDescriptor;
    int mErrFileDescriptor;
    ResourceLimit quota_wallclock_time;    // time since the process started (ms)
    ResourceLimit quota_cpu_time;          // cpu time usage by user + kernel (ms)
    ResourceLimit quota_ram_memory;        // ram usage limit (bytes)
    ResourceLimit quota_disk_memory;       // output size limit (bytes)
    char **pargv;

    int redirectIO();
    int unblockSignals();
    int updateLimits();
    int traceMe();
    int executeProgram();
};


#endif