#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task_model.h"

Task tasks[MAX_TASKS];
int num_tasks = 0;

void load_tasks(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    printf("[INFO] Parsing task file: %s\n", filename);

    while (!feof(f)) {
        Task t;
        int read_fields = fscanf(f, "%d %d %d %s %d %d %d %d",
                                 &t.cnn_id, &t.id, &t.priority,
                                 t.node, &t.size, &t.C, &t.D, &t.nb_pred);
        if (read_fields != 8) break;

        for (int i = 0; i < t.nb_pred; i++) {
            if (fscanf(f, "%d", &t.pred_ids[i]) != 1) {
                fprintf(stderr, "[ERROR] Failed to read predecessor ID\n");
                exit(EXIT_FAILURE);
            }
        }

        // Initialize computed fields
        t.R = 0;
        t.B = 0;
        t.I = 0;

        tasks[num_tasks++] = t;

        if (num_tasks >= MAX_TASKS) {
            fprintf(stderr, "[WARNING] Maximum task count (%d) reached.\n", MAX_TASKS);
            break;
        }
    }

    fclose(f);
    printf("[INFO] Loaded %d tasks.\n", num_tasks);
}
