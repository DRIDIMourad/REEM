# REEM – Response-time Estimation for Embedded Multi-architectures

**REEM** is a modular framework for modeling and analyzing the real-time performance of AI applications—especially CNN-based tasks—on heterogeneous embedded architectures such as GPUs, NPUs, and Networks-on-Chip (NoCs).

It automatically extracts task models from source code (e.g. CUDA/C++) and performs response-time analysis using specialized models tailored to parallel execution and memory interference.

## Features

- Automatic extraction of task models (e.g., NTM, LTM, KBTM)
- Modular architecture: supports GPU, NPU, and NoC backends
- Real-time response-time analysis for CNNs and AI pipelines
- Visualization of execution timelines and interferences
- Easily extensible for custom hardware platforms or new models

## Use Cases

- Real-time analysis of CNNs deployed on embedded GPUs (e.g. Jetson)
- Evaluation of memory/contention effects in AI-based systems
- Comparative analysis between task models and scheduling strategies
- Integration with real-time system design and validation toolchains

## Architecture

Source Code (CUDA/C++)
│
▼
Task Model Extractor
│
▼
+–––––––––––––+
|  GPU / NPU / NoC Models  |
+–––––––––––––+
│
▼
Response Time Analyzer
│
▼
Output Reports
(Response bounds, logs)

## Future Work

- Support for multi-core CPU task graphs
- Graphical user interface (GUI) and CLI
- Benchmark suite for CNN task model validation
- Integration with scheduling simulation engines

## License
