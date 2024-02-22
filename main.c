#include <stdio.h>
#include "gt.h"

void f(void){
    static int x;
    int i, id;

    id = ++x;
    for (i = 0; i < 10; i++) {
        printf("thread %d: %d\n", id, i);
        gt_yield();
    }
}

int main(void){
    gt_init();
    gt_start(f);
    gt_start(f);
    gt_ret(1);
}