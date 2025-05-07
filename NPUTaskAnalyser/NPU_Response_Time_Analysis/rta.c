#include <string.h>
#include "task_model.h"

int cpu_cap = 4;
int npu_cap = 8;

int capacity_for_node(const char* node) {
    return strcmp(node, "CPU") == 0 ? cpu_cap : npu_cap;
}

int overlaps(Task* t1, Task* t2) {
    return strcmp(t1->node, t2->node) == 0 &&
           (t1->size + t2->size > capacity_for_node(t1->node));
}

int max_pred_R(Task* t) {
    int max = 0;
    for (int i = 0; i < t->nb_pred; i++) {
        int pred_id = t->pred_ids[i];
        for (int j = 0; j < num_tasks; j++) {
            if (tasks[j].cnn_id == t->cnn_id && tasks[j].id == pred_id) {
                if (tasks[j].R > max) max = tasks[j].R;
            }
        }
    }
    return max;
}

int blocking(Task* t) {
    int block = max_pred_R(t);
    int maxC = 0;
    for (int i = 0; i < num_tasks; i++) {
        Task* other = &tasks[i];
        if (other->priority < t->priority &&
            strcmp(other->node, t->node) == 0 &&
            overlaps(t, other)) {
            if (other->C > maxC) maxC = other->C;
        }
    }
    return block + maxC;
}

int interference(Task* t) {
    int I = 0;
    for (int i = 0; i < num_tasks; i++) {
        Task* other = &tasks[i];
        if (other->priority > t->priority &&
            strcmp(other->node, t->node) == 0 &&
            overlaps(t, other)) {
            I += other->C;
        }
    }
    return I;
}

void compute_response_times() {
    for (int i = 0; i < num_tasks; i++) {
        Task* t = &tasks[i];
        t->B = blocking(t);
        t->I = interference(t);
        t->R = t->C + t->B + t->I;
    }
}
