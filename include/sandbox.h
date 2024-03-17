#ifndef SANDBOX_H
#define SANDBOX_H



#include "include/evaluated_program.h"


/**
 * @brief Types of sandbox execution status.
 */
typedef enum
{
    PREPARING          = 0,     /*!< Preparing (not ready to execute) */
    READY              = 1,     /*!< Ready (waiting for execution) */
    EXECUTING          = 2,     /*!< Executing (waiting for event) */
    BLOCKED            = 3,     /*!< Blocked (handling event) */
    FINISHED           = 4,     /*!< Finished */
} program_status;


/**
 * @brief Types of sandbox execution result.
 */
typedef enum
{
    PENDING                     =  0,    /*!< Pending */
    ACCEPTED                    =  1,    /*!< Okay */
    RESTRICTED_FUNCTION         =  2,    /*!< Restricted Function */
    MEMORY_LIMIT_EXCEED         =  3,    /*!< Memory Limit Exceed */
    OUTPUT_LIMIT_EXCEED         =  4,    /*!< Output Limit Exceed */
    TIME_LIMIT_EXCEED           =  5,    /*!< Time Limit Exceed */
    RUNTIME_ERROR               =  6,    /*!< Run Time Error (SIGSEGV, SIGFPE, ...) */
    ABNORMAL_TERMINATION        =  7,    /*!< Abnormal Termination */
    INTERNAL_ERROR              =  8,    /*!< Internal Error (of sandbox executor) */
    BAD_POLICY                  =  9,    /*!< Bad Policy */
} program_veredict;



class Sandbox 
{
    public:
    Sandbox(char *argv[], char *evaluated_program_input_filename);
    int start();

    ~Sandbox();


    private:
    program_status mStatus;
    program_veredict mVeredict;
    EvaluatedProgram *pEvaluatedProgram;
    int mInPipe[2];
    int mOutPipe[2];
    int mErrPipe[2];


    int runProgram();
};


#endif