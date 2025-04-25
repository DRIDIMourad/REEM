// main.c
#include "analysis.h"

// Prototype for text-based loader (see Task definition in analysis.h)
int load_text_tasks(const char* filename, CNNTask* tasks);

int main() {
    const char* path = "tasks.txt";
    CNNTask tasks[MAX_TASKS];
    int num = load_text_tasks(path, tasks);
    if (num <= 0) {
        fprintf(stderr, "Error: unable to load tasks from %s", path);
        return 1;
    }

    // Compute and display RT for each loaded CNNTask
    for (int i = 0; i < num; i++) {
        CNNTask* t = &tasks[i];
        printf("CNN %-8s | NTM: %.2f | LTM: %.2f | KTM: %.2f | BTM: %.2f",
               t->name,
               compute_ntm(t, tasks, num),
               compute_ltm(t, tasks, num),
               compute_ktm(t, tasks, num),
               compute_btm(t, tasks, num)
        );
    }
    return 0;
}
