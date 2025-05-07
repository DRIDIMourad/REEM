#include <stdio.h>
#include "task_model.h"

/*
 * CNN Response Time Analyzer
 * Author: Mourad DRIDI
 * License: MIT
 * Description:
 *   - Reads task model from "tasks.txt" in the current directory
 *   - Computes response time R_i = C_i + B_i + I_i for each task
 *   - Handles precedence and resource contention
 */

int main(void) {
    const char* filename = "tasks.txt";
    printf("[INFO] Loading task model from %s...\n", filename);

    load_tasks(filename);
    compute_response_times();
    print_by_cnn();

    return 0;
}
