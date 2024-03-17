#ifndef EVALUATED_PROGRAM_CPP
#define EVALUATED_PROGRAM_CPP

#include <iostream>
#include <signal.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/time.h>
#include <unistd.h>

#include "include/evaluated_program.h"

using namespace std;


EvaluatedProgram::EvaluatedProgram(int inFileDescriptor, int outFileDescriptor, int errFileDescriptor, char *argv[])
{
    mInFileDescriptor = inFileDescriptor;
    mOutFileDescriptor = outFileDescriptor;
    mErrFileDescriptor = errFileDescriptor; 
    pargv = argv;
}

int EvaluatedProgram::run()
{
    if(int exit_status = redirectIO() != EXIT_SUCCESS){
        return exit_status;
    }
    if(int exit_status = unblockSignals() != EXIT_SUCCESS){
        return exit_status;
    }
    if(int exit_status = updateLimits() != EXIT_SUCCESS){
        return exit_status;
    }
    if(int exit_status = traceMe() != EXIT_SUCCESS){
        return exit_status;
    }
    return executeProgram();
}

int EvaluatedProgram::redirectIO()
{
    if(dup2(mInFileDescriptor, STDIN_FILENO) < 0){
        cout << "Failed to redirect stdin.\n";
        return EXIT_FAILURE;
    }

    if(dup2(mOutFileDescriptor, STDOUT_FILENO) < 0){
        cout << "Failed to redirect stdout.\n";
        return EXIT_FAILURE;
    }

    if(dup2(mErrFileDescriptor, STDERR_FILENO) < 0){
        cout << "Failed to redirect stderr.\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int EvaluatedProgram::unblockSignals()
{
    sigset_t sigmask;
    sigfillset(&sigmask);
    if(pthread_sigmask(SIG_UNBLOCK, &sigmask, NULL) != 0){
        cout << "Failed unblocking signals.\n";
        return EXIT_FAILURE;
    }
    cout << "Unblocked all signals.\n";
    return EXIT_SUCCESS;
}

int EvaluatedProgram::updateLimits()
{
    rlimit rlimval;
    
    // set largest core file to 0
    if (getrlimit(RLIMIT_CORE, &rlimval) != 0)
    {
        cout << "failed to getrlimit(RLIMIT_CORE)\n";
        return EXIT_FAILURE;
    }

    rlimval.rlim_cur = 0;
    if (setrlimit(RLIMIT_CORE, &rlimval) != 0)
    {
        cout << "failed to setrlimit(RLIMIT_CORE)\n";
        return EXIT_FAILURE;
    }
    cout << "RLIMIT_CORE: " << rlimval.rlim_cur << "\n";
     
    // Disk quota (bytes)
    if (getrlimit(RLIMIT_FSIZE, &rlimval) != 0)
    {
        cout << "failed to getrlimit(RLIMIT_FSIZE)\n";
        return EXIT_FAILURE;
    }
    rlimval.rlim_cur = quota_disk_memory;
    if (setrlimit(RLIMIT_FSIZE, &rlimval) != 0)
    {
        cout << "failed to setrlimit(RLIMIT_FSIZE)\n";
        return EXIT_FAILURE;
    }
    cout << "RLIMIT_FSIZE: " << rlimval.rlim_cur << "\n";
    
    // Memory quota (bytes)
    if (getrlimit(RLIMIT_AS, &rlimval) != 0)
    {
        return EXIT_FAILURE;
    }
    rlimval.rlim_cur = quota_ram_memory;
    if (setrlimit(RLIMIT_AS, &rlimval) != 0)
    {
        return EXIT_FAILURE;
    }
    cout << "RLIMIT_AS: " << rlimval.rlim_cur << "\n";
    
    // CPU quota
    itimerval itv;
    itv.it_interval.tv_sec = 0;
    itv.it_interval.tv_usec = 0;
    itv.it_value.tv_sec = quota_cpu_time / 1000;
    itv.it_value.tv_usec = (quota_cpu_time % 1000) * 1000;
    if (setitimer(ITIMER_PROF, &itv, NULL) < 0)
    {
        cout << "failed to setitimer(ITIMER_PROF)\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int EvaluatedProgram::traceMe()
{
    if(ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1){
        cout << "ptrace(PTRACE_TRACEME) failed with " << errno << " status code\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


int EvaluatedProgram::executeProgram()
{
    if(execve("code", pargv, NULL) != 0){
        cout << "execve() failed unexpectedly with errno: " << errno << ".\n";
        return errno;
    }
    // execve should not roturn
    return EXIT_FAILURE;
}

#endif