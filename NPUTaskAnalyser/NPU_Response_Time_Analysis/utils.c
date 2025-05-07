#include <stdio.h>
#include "task_model.h"

void print_by_cnn() {
    int max_cnn = -1;
    for (int i = 0; i < num_tasks; i++) {
        if (tasks[i].cnn_id > max_cnn) max_cnn = tasks[i].cnn_id;
    }
    for (int c = 0; c <= max_cnn; c++) {
        printf("CNN %d:\n", c);
        for (int i = 0; i < num_tasks; i++) {
            if (tasks[i].cnn_id == c) {
                Task* t = &tasks[i];
                printf("  Task %d: R = %d  (C=%d, B=%d, I=%d, D=%d)  [%s]\n",
       t->id, t->R, t->C, t->B, t->I, t->D,
       t->R <= t->D ? "OK" : "VIOLATED");
            }
        }
    }
}
