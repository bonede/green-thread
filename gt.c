#include "gt.h"
#include <assert.h>
#include <stdlib.h>

void gt_switch(struct thread_ctx *old_ctx, struct thread_ctx *new_ctx);

void gt_init(){
    curren_thread = &thread_table[0];
    curren_thread->state = RUNNING;
}

void __attribute__((noreturn)) gt_ret(int ret)
{
    if (curren_thread != &thread_table[0]) {
        curren_thread->state = UNUSED;
        gt_yield();
        assert(!"reachable");
    }
    while (gt_yield() == 0){}
    exit(ret);
}

int gt_yield(){
    struct thread *p = curren_thread;
    struct thread_ctx *old_ctx, *new_ctx;
    while (p->state != READY) {
        if (++p == &thread_table[MAX_THREADS]){
            p = &thread_table[0];
        }
        if (p == curren_thread){
            return 1;
        }  
    }

    if (curren_thread->state != UNUSED){
        curren_thread->state = READY;
    }
    p->state = RUNNING;
    old_ctx = &curren_thread->ctx;
    new_ctx = &p->ctx;
    curren_thread = p;
    gt_switch(old_ctx, new_ctx);
    return 0;
}

static void gt_stop(void) { gt_ret(0); }

int gt_start(void (*f)(void)){
    struct thread *p = 0;
    for(int i = 0; i < MAX_THREADS; i++){
        if(thread_table[i].state == UNUSED){
            p = &thread_table[i];
        }
    }
    if(p == 0){
        return -1;
    }
    char *stack = malloc(STACK_SIZE);
    if (!stack){
        return -1;
    }
    *(uint64_t *)(stack + STACK_SIZE - 8) = (uint64_t) gt_stop;
    *(uint64_t *)(stack + STACK_SIZE - 16) = (uint64_t) f;
    p->ctx.rsp = (uint64_t) (stack + STACK_SIZE - 16);
    p->state= READY;

    return 0;
}