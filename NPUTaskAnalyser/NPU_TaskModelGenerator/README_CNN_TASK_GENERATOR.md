# CNN Task Model Generator

This tool extracts a **task-based representation** of a CNN from:
- The source code of a C-based embedded application using a TFLite model
- The `model.data.h` file (or similar) describing the network architecture and operators
- Optional NeutronGraph structures for NPU execution

It generates a `tasks.txt` file that lists CNN tasks with relevant execution properties for real-time analysis.

---

## 🔧 Goal

Generate a `tasks.txt` file automatically from static analysis of:
- CNN layer sequence (Conv, Pool, FC, Softmax, etc.)
- Layer execution mapping (CPU / NPU)
- Layer sizes, MAC counts, or tile requirements
- Precedence between layers
- Static priority (optional default or rule-based)
- Estimated WCET and deadline (if annotated or estimated heuristically)

---

## 📁 Project Structure

```
cnn_task_extractor/
├── include/
│   └── extract.h           # Interface for parsing and modeling
├── src/
│   ├── main.c              # Main program: drives extraction
│   ├── parser.c            # TFLite model/data.h structure parsing
│   ├── cscan.c             # Source code analysis (optional)
│   └── generator.c         # Builds the task model and writes tasks.txt
├── models/
│   └── model.data.h        # Example extracted from TFLite conversion
├── output/
│   └── tasks.txt           # Generated task model
├── Makefile
└── README.md
```

---

## 🧾 Output Format (`tasks.txt`)

Generated file follows the format:

```
CNN_ID TASK_ID PRIORITY NODE SIZE C_i D_i NB_PRED [PRED_IDs...]
```

Each task corresponds to a CNN layer. The model can be used by the real-time analysis tool `cnn_rta`.

---

## 🧠 Extraction Strategy

- **Layer parsing**: Read layer structure from `model.data.h` (flatbuffer or macro-expanded definitions).
- **Node mapping**: Use naming rules or annotation (e.g., `NeutronGraph` → NPU).
- **Size & WCET estimation**: Based on MAC count, kernel size, or predefined profiles.
- **Precedence**: Inferred from layer order.
- **Priority assignment**: Static by order or based on layer type (optional).

---

## 🚀 Usage

After compiling:

```bash
./cnn_task_extractor model.data.h main.c
```

Result:
```
[INFO] Extracting CNN layers...
[INFO] Generating task model...
[OK] tasks.txt written to ./output/tasks.txt
```

---

## 🔍 Dependencies

- Standard C99
- Parser assumes `model.data.h` structure follows a regular TFLite codegen format
- For `NeutronGraph`, assumes known macro or data structure pattern

---

## 📄 License

MIT License – freely reusable for research and academic use.

---

## 👤 Author

Developed by [Votre Nom], as part of research on real-time scheduling of neural networks on embedded NPUs.
