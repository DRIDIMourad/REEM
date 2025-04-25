
// analysis.c
#include "analysis.h"
#include <yaml.h>  // libyaml parser

// share_tpc: returns 1 if two tasks share at least one TPC
int share_tpc(CNNTask* a, CNNTask* b) {
    for (int i = 0; i < MAX_TPC && a->tpcs[i] != -1; i++) {
        for (int j = 0; j < MAX_TPC && b->tpcs[j] != -1; j++) {
            if (a->tpcs[i] == b->tpcs[j]) return 1;
        }
    }
    return 0;
}

// NTM: network-level model (no internal preemption)
// RT = C_network + B + I
// B: max blocking by any lower-priority task on same TPC
// I: sum of wcet_network of higher-priority tasks on same TPC
float compute_ntm(CNNTask* task, CNNTask* others, int num) {
    float C = task->wcet_network;
    float B = 0.0f, I = 0.0f;
    for (int i = 0; i < num; i++) {
        if (!share_tpc(task, &others[i])) continue;  // skip different TPC
        // blocking by lower-priority
        if (others[i].priority > task->priority) {
            B = fmaxf(B, others[i].wcet_network);
        }
        // interference by higher-priority
        if (others[i].priority < task->priority) {
            I += others[i].wcet_network;
        }
    }
    return C + B + I;
}

// LTM: layer-level model
// Sequential layers, preemptive at layer boundaries
// For each layer: RT_layer = C_layer + I + max(B, RT_prev)
float compute_ltm(CNNTask* task, CNNTask* others, int num) {
    float RT = 0.0f;
    // Iterate layers in network order
    for (int l = 0; l < task->num_layers; l++) {
        float C = task->wcet_layer[l];
        int S = task->size_layer[l];
        float B = 0.0f, I = 0.0f;
        // Check conflicts with other tasks
        for (int i = 0; i < num; i++) {
            if (!share_tpc(task, &others[i])) continue;
            int So = others[i].size_layer[l];
            // if resource sum > capacity, blocking/interference
            if (So + S > TPC_CAPACITY) {
                if (others[i].priority > task->priority) {
                    B = fmaxf(B, others[i].wcet_layer[l]);
                } else if (others[i].priority < task->priority) {
                    I += others[i].wcet_layer[l];
                }
            }
        }
        // accumulate response: include previous RT as precedence
        RT = C + I + fmaxf(B, RT);
    }
    return RT;
}

// KTM: kernel-level model
// Similar to LTM but iterating over kernels
float compute_ktm(CNNTask* task, CNNTask* others, int num) {
    float RT = 0.0f;
    for (int k = 0; k < task->num_kernels; k++) {
        float C = task->wcet_kernel[k];
        int S = task->size_kernel[k];
        float B = 0.0f, I = 0.0f;
        for (int i = 0; i < num; i++) {
            if (!share_tpc(task, &others[i])) continue;
            int So = others[i].size_kernel[k];
            if (So + S > TPC_CAPACITY) {
                if (others[i].priority > task->priority) {
                    B = fmaxf(B, others[i].wcet_kernel[k]);
                } else {
                    I += others[i].wcet_kernel[k];
                }
            }
        }
        RT = C + I + fmaxf(B, RT);
    }
    return RT;
}

// BTM: block-level model
// Finest granularity, iterates over blocks
float compute_btm(CNNTask* task, CNNTask* others, int num) {
    float RT = 0.0f;
    for (int b = 0; b < task->num_blocks; b++) {
        float C = task->wcet_block[b];
        int S = task->size_block[b];
        float B = 0.0f, I = 0.0f;
        for (int i = 0; i < num; i++) {
            if (!share_tpc(task, &others[i])) continue;
            int So = others[i].size_block[b];
            if (So + S > TPC_CAPACITY) {
                if (others[i].priority > task->priority) {
                    B = fmaxf(B, others[i].wcet_block[b]);
                } else {
                    I += others[i].wcet_block[b];
                }
            }
        }
        RT = C + I + fmaxf(B, RT);
    }
    return RT;
}


int load_text_tasks(const char* filename, CNNTask* tasks) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Cannot open task file");
        return 0;
    }
    int count = 0;
    while (count < MAX_TASKS && !feof(file)) {
        CNNTask* ct = &tasks[count];
        // Try to read basic fields first
        if (fscanf(file, "%63s %f %d", ct->name, &ct->wcet_network, &ct->num_layers) != 3)
            break;
        // Read per-layer wcet
        for (int i = 0; i < ct->num_layers; i++) {
            fscanf(file, "%f", &ct->wcet_layer[i]);
        }
        // Read number of kernels and their wcet
        fscanf(file, "%d", &ct->num_kernels);
        for (int i = 0; i < ct->num_kernels; i++) {
            fscanf(file, "%f", &ct->wcet_kernel[i]);
        }
        // Read number of blocks and their wcet
        fscanf(file, "%d", &ct->num_blocks);
        for (int i = 0; i < ct->num_blocks; i++) {
            fscanf(file, "%f", &ct->wcet_block[i]);
        }
        // Read period, deadline, priority, single TPC id
        fscanf(file, "%f %f %d %d", &ct->period, &ct->deadline, &ct->priority, &ct->tpcs[0]);
        ct->tpcs[1] = -1;
        // Skip sizes (optional)
        for (int i = 0; i < ct->num_layers; i++) fscanf(file, "%d", &ct->size_layer[i]);
        for (int i = 0; i < ct->num_kernels; i++) fscanf(file, "%d", &ct->size_kernel[i]);
        for (int i = 0; i < ct->num_blocks; i++) fscanf(file, "%d", &ct->size_block[i]);
        count++;
    }
    fclose(file);
    return count;
}
