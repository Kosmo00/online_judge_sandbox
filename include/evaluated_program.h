#ifndef EVALUATED_PROGRAM_H
#define EVALUATED_PROGRAM_H

#include "./program_limits.h"

class EvaluatedProgram 
{
    public:
    EvaluatedProgram(int inFileDescriptor, int outFileDescriptor, int errFileDescriptor, char *argv[], ProgramLimits *limits);

    int run();

    ~EvaluatedProgram();

    private:
    int mInFileDescriptor;                           
    int mOutFileDescriptor;
    int mErrFileDescriptor;
    ProgramLimits *pLimits;
    char **pargv;

    int redirectIO();
    int unblockSignals();
    int updateLimits();
    int traceMe();
    int executeProgram();
};


#endif