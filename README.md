# Number System Converter & Arithmetic Calculator
**Activity No. 1 — System Development Activity (C++ Implementation)**

A clean, robust, and student-friendly console application developed in **standard C++ (C++11/C++17)** designed to convert multiple numbers across **Binary (Base 2)**, **Octal (Base 8)**, **Decimal (Base 10)**, and **Hexadecimal (Base 16)** with real-time character validation, formatted tabular outputs, and step-by-step mathematical proofs.

---

## 🌟 Key Features

1. **Simple, Clean Console UI / UX**:
   - Clean ASCII banners, menus, and formatted tabular outputs.
   - Robust input handling (no crashes or infinite loops on invalid inputs).

2. **Multi-Input Acceptance**:
   - Accepts **at least three (3) input numbers** (user can specify any count $N \ge 3$).
   - Independent base selection for each input (Binary, Octal, Decimal, Hexadecimal).

3. **Strict Real-Time Validation**:
   - Validates every character against the chosen base.
   - Provides clear, friendly error feedback (e.g. `'2'` in binary, `'8'` in octal, `'G'` in hex).
   - Supports both integers and radix point fractions (e.g. `101.101`, `15.75`).

4. **Simultaneous 4-Base Conversion**:
   - Converts each input number into **Binary**, **Octal**, **Decimal**, and **Hexadecimal**.

5. **Step-by-Step Mathematical Solutions**:
   - Lets the student view the mathematical expansion using positional notation ($N \to 10$) and conversion to target bases ($10 \to M$).

6. **1-Click Required Test Presets**:
   - Built-in test suite to instantly run the required combinations:
     - `Combination 1: Binary + Octal + Decimal`
     - `Combination 2: Binary + Decimal + Hexadecimal`
     - `Combination 3: Octal + Decimal + Hexadecimal`
     - `Combination 4: Binary + Octal + Hexadecimal`

7. **Prepared for Phase 2 (Arithmetic Calculator)**:
   - All inputs are normalized into unified high-precision decimal values, ready for the mixed-base arithmetic calculator in the next session.

---

## 🚀 How to Compile & Run

### Option 1: Double-Click `compile_and_run.bat` (Recommended on Windows)
Simply double-click **`compile_and_run.bat`** in the project folder. It will automatically compile `main.cpp` using Dev-C++'s MinGW compiler (or GCC in PATH) and launch the program.

---

### Option 2: Dev-C++ / Code::Blocks IDE
1. Open **Dev-C++** or **Code::Blocks**.
2. Open `main.cpp`.
3. Press **F9** (or **F11** in Dev-C++) to Compile & Run.

---

### Option 3: Terminal / Command Prompt
```cmd
g++ -std=c++11 -Wall -Wextra main.cpp -o converter.exe
.\converter.exe
```

---

## 📂 Project Structure

```
Number-System-Converter-and-Arithmetic-Calculator/
├── main.cpp              # Complete C++ source code (C++11 compatible)
├── converter.exe         # Compiled Windows executable (ready to run)
├── compile_and_run.bat   # 1-click batch compiler and launcher
├── docs/
│   ├── SYSTEM_SPECS.md   # System Requirements, Algorithms, Pseudocode & Mermaid Flowchart
│   └── TEST_CASES.md     # Test Case Matrices, Console Output Captures & Arithmetic Prep
└── README.md             # Project user manual & overview
```

---

## 📑 Academic Deliverables

- **System Requirements, Algorithms, Pseudocode & Flowchart**: [docs/SYSTEM_SPECS.md](docs/SYSTEM_SPECS.md)
- **Test Cases & Console Output Matrix**: [docs/TEST_CASES.md](docs/TEST_CASES.md)
