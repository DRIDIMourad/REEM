# CNN Response Time Analyzer

A modular C tool for computing the **worst-case response time (WCRT)** of CNN layer tasks on heterogeneous architectures (CPU, NPU), considering:
- Precedence constraints,
- Blocking from lower-priority tasks,
- Interference from higher-priority tasks,
- Node resource contention.

---

## 📦 Project Structure

```
cnn_rta/
├── include/
│   └── task_model.h        # Task model definitions and interface
├── src/
│   ├── main.c              # Program entry point
│   ├── parser.c            # File parsing logic
│   ├── rta.c               # Core response time computation
│   └── utils.c             # Output and display
├── tasks.txt               # Example task input file
├── Makefile                # Build configuration
└── README.md               # You are here
```

---

## 🧾 Input Format (`tasks.txt`)

Each line describes a CNN layer task with this format:

```
CNN_ID TASK_ID PRIORITY NODE SIZE C_i D_i NB_PRED [PRED_ID_1 PRED_ID_2 ...]
```

### Example:
```
0 0 3 NPU 3 10 20 0
0 1 2 NPU 4 12 25 1 0
0 2 1 CPU 2 8  30 1 1
1 0 2 CPU 1 5  15 0
1 1 1 NPU 3 9  20 1 0
```

- `CNN_ID` : identifier of the CNN
- `TASK_ID` : unique ID within the CNN
- `PRIORITY` : static priority (higher value = higher priority)
- `NODE` : either `CPU` or `NPU`
- `SIZE` : resource demand (e.g., compute tiles)
- `C_i` : worst-case execution time
- `D_i` : deadline
- `NB_PRED` : number of direct predecessors
- `[PRED_ID...]` : list of predecessor task IDs

---

## 🛠️ Build Instructions

```bash
make
```

---

## 🚀 Usage

```bash
./cnn_rta
```

The program will automatically read `tasks.txt` from the current directory, compute the response time \( R_i = C_i + B_i + I_i \) for each task, and output results by CNN.

---

## 🧠 Output Example

```
[INFO] Loaded 5 tasks.
CNN 0:
  Task 0: R = 10  (C=10, B=0,  I=0,  D=20)  [OK]
  Task 1: R = 22  (C=12, B=10, I=0,  D=25)  [OK]
  Task 2: R = 34  (C=8,  B=14, I=12, D=30)  [VIOLATED]
CNN 1:
  Task 0: R = 5   (C=5,  B=0,  I=0,  D=15)  [OK]
  Task 1: R = 17  (C=9,  B=5,  I=3,  D=20)  [OK]
```

---

## ✅ Features

- Per-task response time breakdown: computation, blocking, interference.
- Blocking considers non-preemptive lower-priority tasks and precedence.
- Interference considers node contention and resource overuse.
- Reports whether each task meets its deadline.

---

## 🧩 Possible Extensions

- Export results to CSV
- Add YAML or JSON input support
- Support for periodic tasks and jitter
- Graph visualization of CNN task chains

---

## 📄 License

MIT License – free for academic and industrial use.

---

## 👤 Author

Developed by [Votre Nom]. For questions or contributions, open an issue or contact directly.
