// analysis.h
#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Maximum dimensions and constants
#define MAX_LAYERS   16       // Max CNN layers
#define MAX_KERNELS  32       // Max kernels per CNN
#define MAX_BLOCKS   64       // Max blocks per kernel
#define MAX_TASKS    8        // Max number of CNN tasks
#define MAX_TPC      4        // Max TPC IDs per task
#define TPC_CAPACITY 2048     // Max threads supported per TPC

// CNNTask: holds all WCET values and resource sizes at multiple granularity levels
// - NTM uses wcet_network
// - LTM uses wcet_layer[] and size_layer[]
// - KTM uses wcet_kernel[] and size_kernel[]
// - BTM uses wcet_block[] and size_block[]
typedef struct {
    char name[64];                 // Task identifier
    float wcet_network;            // WCET for whole network (NTM)
    float wcet_layer[MAX_LAYERS];  // Per-layer WCET (LTM)
    float wcet_kernel[MAX_KERNELS];// Per-kernel WCET (KTM)
    float wcet_block[MAX_BLOCKS];  // Per-block WCET (BTM)
    float period;                  // Activation period
    float deadline;                // Relative deadline
    int priority;                  // Priority (lower value = higher priority)
    int num_layers;                // Actual layer count
    int num_kernels;               // Total kernels count
    int num_blocks;                // Total blocks count
    int tpcs[MAX_TPC];             // Assigned TPC IDs, terminated by -1
    int size_layer[MAX_LAYERS];    // Thread usage per layer
    int size_kernel[MAX_KERNELS];  // Thread usage per kernel
    int size_block[MAX_BLOCKS];    // Thread usage per block
} CNNTask;

// Load tasks from a YAML file into tasks[] array
// Returns number of tasks loaded (<= max_tasks)
int load_tasks(const char* filename, CNNTask* tasks, int max_tasks);

// Compute worst-case response times using different models
float compute_ntm(CNNTask* task, CNNTask* others, int num);
float compute_ltm(CNNTask* task, CNNTask* others, int num);
float compute_ktm(CNNTask* task, CNNTask* others, int num);
float compute_btm(CNNTask* task, CNNTask* others, int num);

#endif // ANALYSIS_H