#ifndef SANDBOX_CPP
#define SANDBOX_CPP

#include <iostream>
#include <unistd.h>

#include "include/sandbox.h"
#include "include/evaluated_program.h"

using namespace std;

Sandbox::Sandbox(char *argv[], char *evaluated_program_input_filename)
{
    pipe(mInPipe);
    pipe(mOutPipe);
    pipe(mErrPipe);
    mVeredict = PENDING;
    mStatus = PREPARING;
    pEvaluatedProgram = new EvaluatedProgram(mInPipe[0], mOutPipe[1], mErrPipe[1], argv);

}

int Sandbox::start()
{
    runProgram();
}

int Sandbox::runProgram()
{
    pid_t pid = fork();

    if(pid == -1){
        cout << "Fork() failed failed.\n";
        return EXIT_FAILURE;
    }

    if(pid == 0){
        // Close out pipes
        close(mOutPipe[1]);
        close(mErrPipe[1]);
        return pEvaluatedProgram->run();
    }
    return EXIT_SUCCESS;
}

Sandbox::~Sandbox()
{
    delete(pEvaluatedProgram);
}

#endif