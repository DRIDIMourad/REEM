// generate_tasks.c
// Generates 'tasks.txt' with one line per CNN:
// name wcet_network num_layers [wcet_layer...] num_kernels [wcet_kernel...] num_blocks [wcet_block...] period deadline priority tpc_id [size_layer...] [size_kernel...] [size_block...]

#include <stdio.h>
#include <stdlib.h>

#define MAX_LAYERS   16
#define MAX_KERNELS  16
#define MAX_BLOCKS   16
#define NUM_CNN      3

// Configuration for each CNN
typedef struct {
    const char* name;
    float wcet_network;
    int num_layers;
    float wcet_layer[MAX_LAYERS];
    int num_kernels;
    float wcet_kernel[MAX_KERNELS];
    int num_blocks;
    float wcet_block[MAX_BLOCKS];
    float period;
    float deadline;
    int priority;
    int tpc_id;
    int size_layer[MAX_LAYERS];
    int size_kernel[MAX_KERNELS];
    int size_block[MAX_BLOCKS];
} CNNConfig;

int main() {
    CNNConfig configs[NUM_CNN] = {
        {
            "cnn1",
            100.0f,
            2, {40.0f, 60.0f},
            2, {40.0f, 60.0f},
            5, {20.0f,20.0f,20.0f,20.0f,20.0f},
            200.0f, 200.0f,
            1, 0,
            {1500,1700},
            {1400,1600},
            {512,512,512,512,512}
        },
        {
            "cnn2",
            80.0f,
            2, {30.0f, 50.0f},
            2, {30.0f, 50.0f},
            5, {15.0f,15.0f,15.0f,15.0f,15.0f},
            150.0f, 150.0f,
            2, 0,
            {600,1000},
            {500,1000},
            {256,256,256,256,256}
        },
        {
            "cnn3",
            120.0f,
            3, {50.0f, 40.0f, 30.0f},
            3, {50.0f, 40.0f, 30.0f},
            6, {10.0f,10.0f,10.0f,10.0f,10.0f,10.0f},
            250.0f, 250.0f,
            3, 1,
            {1600,1400,1200},
            {1500,1300,1100},
            {256,256,256,256,256,256}
        }
    };

    FILE* fout = fopen("tasks.txt", "w");
    if (!fout) {
        perror("Failed to open tasks.txt");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < NUM_CNN; i++) {
        CNNConfig* c = &configs[i];
        // name, wcet_network
        fprintf(fout, "%s %.1f ", c->name, c->wcet_network);
        // num_layers and wcet_layer[]
        fprintf(fout, "%d", c->num_layers);
        for (int j = 0; j < c->num_layers; j++) fprintf(fout, " %.1f", c->wcet_layer[j]);
        fprintf(fout, " ");
        // num_kernels and wcet_kernel[]
        fprintf(fout, "%d", c->num_kernels);
        for (int j = 0; j < c->num_kernels; j++) fprintf(fout, " %.1f", c->wcet_kernel[j]);
        fprintf(fout, " ");
        // num_blocks and wcet_block[]
        fprintf(fout, "%d", c->num_blocks);
        for (int j = 0; j < c->num_blocks; j++) fprintf(fout, " %.1f", c->wcet_block[j]);
        fprintf(fout, " ");
        // period, deadline, priority, tpc_id
        fprintf(fout, "%.1f %.1f %d %d ",
            c->period, c->deadline, c->priority, c->tpc_id);
        // size_layer[]
        for (int j = 0; j < c->num_layers; j++) fprintf(fout, "%d ", c->size_layer[j]);
        // size_kernel[]
        for (int j = 0; j < c->num_kernels; j++) fprintf(fout, "%d ", c->size_kernel[j]);
        // size_block[]
        for (int j = 0; j < c->num_blocks; j++) {
            fprintf(fout, "%d", c->size_block[j]);
            if (j < c->num_blocks-1) fprintf(fout, " ");
        }
        fprintf(fout, "\n");
    }

    fclose(fout);
    printf("tasks.txt generated successfully.\n");
    return 0;
}
