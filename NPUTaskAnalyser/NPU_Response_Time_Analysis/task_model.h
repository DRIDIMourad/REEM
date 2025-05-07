#ifndef TASK_MODEL_H
#define TASK_MODEL_H

#define MAX_TASKS 100
#define MAX_PREDS 10

typedef struct {
    int cnn_id;
    int id;
    int priority;
    char node[4]; // "CPU" or "NPU"
    int size;
    int C;
    int nb_pred;
    int pred_ids[MAX_PREDS];
    int R;
    int B; // NEW: blocking time
    int I; // NEW: interference time
    int D; // Deadline

} Task;

// Capacities
int capacity_for_node(const char* node);
extern int cpu_cap;
extern int npu_cap;

// Globals
extern Task tasks[MAX_TASKS];
extern int num_tasks;

// Core logic
void compute_response_times(void);

// Utils
void print_by_cnn(void);
void load_tasks(const char* filename);

#endif
