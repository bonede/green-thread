#ifndef GT_H
#define GT_H

#include <stdint.h>

/**
 * @brief Max threads to run(Including bootstrap thread).
 * 
 */
#define MAX_THREADS 4

/**
 * @brief Thread stack size.
 * 
 */
#define STACK_SIZE 1024 * 1024 * 4

/**
 * @brief An item in thread table
 * 
 */
struct thread {
    /**
     * @brief Context the thread(values in CPU registers).
     * 
     */
    struct thread_ctx {
        uint64_t rsp;
        uint64_t r15;
        uint64_t r14;
        uint64_t r13;
        uint64_t r12;
        uint64_t rbx;
        uint64_t rbp;
    } ctx;
    enum {
        /**
         * @brief Thread is unused
         * 
         */
        UNUSED,
        /**
         * @brief Thread is running. At most, only one thread is in this state.
         * 
         */
        RUNNING,
        /**
         * @brief Thread is ready to run
         * 
         */
        READY,
    } state;
};


static struct thread thread_table[MAX_THREADS];
static struct thread *curren_thread;

/**
 * @brief init the thread table
 * 
 */
void gt_init();

/**
 * @brief start a new thread
 * 
 */
int gt_start(void (*f)(void)); 

/**
 * @brief wait current thread to finish
 * 
 */
void gt_ret(int ret);

/**
 * @brief yield to another thread 
 * 
 */
int gt_yield();

#endif /* GT_H */