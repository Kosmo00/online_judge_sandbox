#ifndef SANDBOX_CPP
#define SANDBOX_CPP

#include <iostream>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../include/sandbox.h"
#include "../include/evaluated_program.h"

using namespace std;

Sandbox::Sandbox(char *argv[], char *evaluated_program_input_filename, ProgramLimits *limits)
{
    pipe(mInPipe);
    pipe(mOutPipe);
    pipe(mErrPipe);
    
    writeInputPipe(evaluated_program_input_filename);

    mVeredict = PENDING;
    mStatus = PREPARING;
    pEvaluatedProgram = new EvaluatedProgram(mInPipe[0], mOutPipe[1], mErrPipe[1], argv, limits);
}

int Sandbox::start()
{
    runProgram();
    runMainTracer();

    return EXIT_SUCCESS;
}

int Sandbox::runProgram()
{
    pid_t pid = fork();

    mChildPid = pid; // temp

    if(pid == -1){
        cout << "Fork() failed.\n";
        return EXIT_FAILURE;
    }

    if(pid == 0){
        // Close out pipes
        close(mOutPipe[0]);
        close(mErrPipe[0]);
        return pEvaluatedProgram->run();
    }
    return EXIT_SUCCESS;
}

int Sandbox::writeInputPipe(char *evaluated_program_input_filename)
{
    // Getting data from in file
    FILE *f = fopen(evaluated_program_input_filename, "r");
    char *buffer = new char[1024 * 1024 * 10];
    fgets(buffer, sizeof(buffer), f);
    fclose(f);
    // Putting input data into input pipe
    ssize_t bytes_written = write(mInPipe[1], buffer, strlen(buffer));
    if(bytes_written == -1) {
        return EXIT_FAILURE;
    }
    close(mInPipe[1]);

    return EXIT_SUCCESS;
}

int Sandbox::runMainTracer()
{
    // Close input section for output pipes
    close(mOutPipe[1]);
    close(mErrPipe[1]);
    char *buffer = new char[1024 * 1024 * 10];
    ssize_t nbytes = read(mOutPipe[0], buffer, sizeof(buffer));
    
    if (nbytes == -1) {
        perror("Error reading tracee program output");
        return 1;
    }

    // Wait for the children
    cout << buffer << " ";
    int status;
    waitpid(mChildPid, &status, 0);

    
    // Close output section for output pipes
    close(mOutPipe[0]);
    close(mErrPipe[0]);

    return EXIT_SUCCESS;
}

// Sandbox::~Sandbox()
// {
    
// }


#endif