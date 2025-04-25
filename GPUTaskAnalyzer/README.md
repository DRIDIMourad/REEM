GPUTaskAnalyzer

GPUTaskAnalyzer is an automated toolchain for extracting GPU-based real-time task models directly from CNN inference code and computing worst-case response times under multiple abstraction levels (Network, Layer, Kernel, Block).

⸻

Features
	•	Automatic Extraction: Parses C/CUDA source (e.g., Darknet) or simple text inputs to build task descriptors.
	•	Multi-Level Modeling:
	•	NTM: Treats the entire CNN as one task.
	•	LTM: One task per layer, with precedence and resource-sharing analysis.
	•	KTM: One task per CUDA kernel, capturing intra-layer concurrency.
	•	BTM: One task per CUDA block, the finest granularity for resource contention.
	•	Response-Time Analysis: Implements fixed-priority response-time equations including blocking and interference based on shared TPC capacities.
	•	Standalone Generation: generate_tasks.c to produce input files (tasks.txt) from user-defined CNN configurations.

⸻

Repository Structure

REEM/
├── GPUTaskAnalyzer/
│   ├── README.md         # This file
│   ├── analysis.h        # API definitions and data structures
│   ├── analysis.c        # RTA implementations for NTM, LTM, KTM, BTM
│   ├── main.c            # YAML/text parser + invocation of analysis
│   ├── generate_tasks.c  # Helper to generate sample tasks.txt
│   ├── tasks.yaml        # Example YAML input
│   └── tasks.txt         # Generated text-based input
├── other_modules/        # (future) NPU, NoC modules
└── README.md             # Root-level overview



⸻

Getting Started

Prerequisites
	•	GCC or Clang (C99)
	•	libyaml development headers (for YAML parsing)

Build

cd GPUTaskAnalyzer
gcc analysis.c main.c -o gputaskanalyzer -lyaml -lm
gcc generate_tasks.c -o generate_tasks

Usage
	1.	Generate sample input (optional):

./generate_tasks
# Produces tasks.txt in current folder


	2.	Run analysis:

# Using YAML input
./gputaskanalyzer tasks.yaml

# Or using text input (tasks.txt)
./gputaskanalyzer tasks.txt



Output will list each CNN task with its NTM, LTM, KTM, and BTM response times.

⸻

Configuration
	•	tasks.yaml: define cnn_tasks sequence with fields:
name, wcet_network, num_layers, wcet_layer array, num_kernels, wcet_kernel array, num_blocks, wcet_block array, period, deadline, priority, tpcs, size_layer, size_kernel, size_block.
	•	tasks.txt: flat text format (one line per CNN) matching generate_tasks.c output.

⸻

Contributing

Contributions are welcome! Please fork the repo and submit pull requests.

⸻

License

MIT License © Mourad Dridi