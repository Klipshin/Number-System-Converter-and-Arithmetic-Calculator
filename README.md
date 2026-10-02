# Number System Converter & Arithmetic Calculator

**Activity No. 1 — System Development Activity (Standard C++11 Implementation)**

A high-performance, modular, console-based software system developed in standard **C++11** with **zero external dependencies**. It accepts multiple numbers across four numeral systems (Binary, Octal, Decimal, Hexadecimal), performs simultaneous 4-base conversions, displays step-by-step mathematical proofs, evaluates mixed-base arithmetic and custom algebraic expressions, and provides dedicated modules for **Binary Complements (1's & 2's)** and **Binary-Coded Decimal (BCD) Operations (9's & 10's Complements, Addition, Subtraction)**.

---

## 🌟 Key Features

### 1. Multi-Input Acceptance ($N \ge 3$) & Independent Bases
- Accepts **at least three (3) input numbers** (user can specify any count $N$ from $3$ up to $20$).
- Each input independently selects its own numeral system:
  - **Binary** (Base 2)
  - **Octal** (Base 8)
  - **Decimal** (Base 10)
  - **Hexadecimal** (Base 16)

### 2. Strict Real-Time Validation & Radix Fraction Support
- Character-by-character radix validation against the chosen base (e.g., rejects `'2'` in binary, `'8'` in octal, `'G'` in hexadecimal).
- Full support for both **integers** and **radix-point fractional numbers** (e.g., `101010.101` BIN, `75.4` OCT, `42.75` DEC, `2A.C` HEX).
- Friendly error feedback with immediate re-prompts without crashes or infinite loops.

### 3. Simultaneous 4-Base Conversion & Tabular Display
- Converts every input into all 4 numeral systems simultaneously.
- Results are presented in an aligned, color-coded ASCII table with original base tags and decimal equivalents.

### 4. Step-by-Step Mathematical Solutions
- Inspect the exact mathematical proof for any entered input:
  - **Base $N \to 10$**: Expanded positional-notation formula showing integer powers ($\sum d_i \cdot b^i$) and fractional negative powers ($\sum d_{-j} \cdot b^{-j}$).
  - **Base $10 \to M$**: Successive division by target radix for integer parts and successive multiplication for fractional parts.

### 5. Mixed-Base Arithmetic Calculator
- Supports **Addition (+)**, **Subtraction (-)**, **Multiplication ($\times$)**, and **Division (/)** across all $N$ inputs.
- All numbers are converted to Decimal (Base 10) as the common internal representation before computing.
- Outputs the original mixed-base expression, decimal calculation, and the final result formatted in Binary, Octal, Decimal, and Hexadecimal.
- Robust division-by-zero protection.

### 6. Custom Infix Algebraic Expression Evaluator
- Allows typing complex expressions using variables `a`, `b`, `c`, ... mapped to Inputs 1..$N$ (e.g., `(a + b) * c - d`).
- **Full Operator Precedence & Associativity**: `*` and `/` evaluate before `+` and `-`. Parentheses override precedence.
- **Implicit Multiplication**: Automatically inserts multiplication for adjacent terms (e.g., `a(b+c)` $\to$ `a * (b+c)`, `(a+b)(c+d)` $\to$ `(a+b) * (c+d)`).
- Implemented via a custom **Shunting-Yard algorithm (Infix to Postfix/RPN)** and stack evaluator.

### 7. Binary Complement Operations (1's & 2's Complement)
- **1's Complement (Diminished Radix)**: Bitwise inversion $\sim X$ (or $(2^n - 1) - X$) with automated bit-width selection ($4, 8, 12, 16, 32$ bits) and zero-padding.
- **2's Complement (Radix Complement)**: $(1\text{'s complement} + 1)$ with full carry propagation.
- **Complement Table View**: Displays original value, bit-width, padded binary, 1's complement, and 2's complement in Binary, Octal, Decimal, and Hexadecimal.
- **Complement Subtraction ($A - B$)**:
  - **1's Complement Subtraction**: Uses the end-around carry method; shows full intermediate addition steps and carry adjustment.
  - **2's Complement Subtraction**: Uses the discard-carry method; handles positive results and negative results (re-complemented for true magnitude).

### 8. Dedicated Binary-Coded Decimal (BCD) Module
- **BCD Encoding**: Converts integer inputs to standard 8421 BCD, formatting each decimal digit as a distinct 4-bit nibble (e.g., `42` $\to$ `0100 0010`).
- **9's & 10's Complement Table**:
  - **9's Complement (Diminished Radix)**: Computes $(9 - d)$ for each decimal digit.
  - **10's Complement (Radix Complement)**: Adds $1$ to the 9's complement with carry propagation.
  - Displays outputs in Decimal, BCD, Binary, Octal, and Hexadecimal.
- **BCD Addition ($A + B$)**: Digit-by-digit addition with automatic $+6$ (`0110`) correction when a digit sum exceeds $9$ or produces an internal carry.
- **BCD Subtraction via 9's Complement**: Computes $A + 9\text{'s comp}(B)$ with BCD $+6$ corrections and end-around carry.
- **BCD Subtraction via 10's Complement**: Computes $A + 10\text{'s comp}(B)$ with BCD $+6$ corrections and discard-carry logic.

### 9. Interactive Terminal UI & Keyboard Navigation
- Interactive scrollable menus navigated using **Arrow keys (Up/Down)** or **W / S**, confirmed with **Enter** or **Space**.
- VT100 / ANSI virtual terminal color formatting on Windows Terminal, CMD, PowerShell, and Unix consoles.
- Automatic numeric fallback for legacy terminals.

### 10. 1-Click Built-in Preset Test Matrix
- Instant one-click test suites verifying all required academic test combinations:
  - **Combination 1**: Binary (`101010`) + Octal (`52`) + Decimal (`42`)
  - **Combination 2**: Binary (`11001100`) + Decimal (`100`) + Hexadecimal (`40`)
  - **Combination 3**: Octal (`72`) + Decimal (`35`) + Hexadecimal (`1A`)
  - **Combination 4**: Binary (`11110000`) + Octal (`72`) + Hexadecimal (`3C`)
- Each preset executes the full conversion table, all 4 arithmetic operations, binary complement operations, and BCD addition/subtraction.

---

## 📁 Project Architecture & Directory Layout

The codebase is organized into a clean, modular multi-file architecture separating types, user interface, number conversion, arithmetic/expression parsing, BCD processing, and display rendering:

```text
Number-System-Converter-and-Arithmetic-Calculator/
│
├── main.cpp                  # Entry point & top-level application loop (~70 lines)
│
├── include/                  # Header declarations
│   ├── globals.h             # Global constants (TERMINAL_WIDTH, ANSI color escapes)
│   ├── types.h               # Data structures (InputNumber, Results, BCD, AST tokens)
│   └── prototypes.h          # Forward declarations & default function parameters
│
├── src/                      # Implementation source units
│   ├── ui.cpp                # VT100 initialization, interactive menus, text centering
│   ├── conversion.cpp        # Validation, toDecimal, fromDecimal, processConversion
│   ├── arithmetic.cpp        # Standard ops, 1's/2's complement, Shunting-Yard parser
│   ├── bcd.cpp               # BCD encoding, 9's/10's complements, BCD add & subtract
│   └── display.cpp           # ASCII tables, math step proofs, menus & presets runner
│
├── docs/                     # Academic documentation & specifications
│   ├── SYSTEM_SPECS.md       # Comprehensive requirements, algorithms, pseudocode & flowcharts
│   └── TEST_CASES.md         # Detailed test matrix & verified console outputs
│
├── compile_and_run.bat       # One-click Windows build and launch script
└── README.md                 # Project documentation and quick-start guide
```

---

## 🚀 How to Compile & Run

### Option 1: Double-Click `compile_and_run.bat` (Recommended on Windows)

Simply double-click **`compile_and_run.bat`** in the project root. It will:
1. Detect your MinGW GCC compiler (Dev-C++ default installation or system `PATH`).
2. Compile all source files (`main.cpp` and `src/*.cpp`) with standard `-Iinclude`.
3. Launch `converter.exe` automatically in the console.

---

### Option 2: Command Line (GCC / MinGW / Clang)

Compile using standard C++11 and include the `include/` directory:

```bash
# Using g++ on Windows (PowerShell or CMD) or Linux/macOS
g++ -std=c++11 -Wall -Wextra -Iinclude main.cpp src/ui.cpp src/conversion.cpp src/arithmetic.cpp src/bcd.cpp src/display.cpp -o converter.exe

# Or compile all sources with wildcard
g++ -std=c++11 -Wall -Wextra -Iinclude main.cpp src/*.cpp -o converter.exe

# Run the compiled application
./converter.exe
```

---

### Option 3: Dev-C++ / Code::Blocks IDE

1. Create a new **Console Application (C++)** project.
2. Add `main.cpp` and all `.cpp` files inside the `src/` folder (`ui.cpp`, `conversion.cpp`, `arithmetic.cpp`, `bcd.cpp`, `display.cpp`) to the project.
3. In Project Settings $\to$ Compiler Options, set:
   - Language standard: `-std=c++11`
   - Include search directory: `-Iinclude` (or add the `include` folder path)
4. Press **F9** (or **F11** in Dev-C++) to Compile & Run.

---

## 🗺️ Application Menu Hierarchy

```text
[ MAIN MENU ]
 ├── 1. Interactive Number System Converter (Inputs >= 3)
 │       │
 │       ├── Enter Count N (3 to 20)
 │       ├── For each Input 1..N:
 │       │    ├── Select Base (Binary, Octal, Decimal, Hexadecimal)
 │       │    └── Enter Value (Real-time validated, supports decimals)
 │       │
 │       ├── [ CONVERSION RESULTS TABLE ] (Simultaneous 4-Base Table)
 │       │
 │       └── [ SELECT ARITHMETIC OPERATION ]
 │            ├── Addition (+)           -- Applied across all N inputs
 │            ├── Subtraction (-)        -- Applied across all N inputs
 │            ├── Multiplication (x)     -- Applied across all N inputs
 │            ├── Division (/)           -- Applied across all N inputs
 │            ├── Custom Expression      -- e.g., (a+b)*c-d with operator precedence
 │            │
 │            ├── Binary Complements     --> [ BINARY COMPLEMENT SUBMENU ]
 │            │                                ├── View 1's & 2's Complement Table
 │            │                                ├── Subtract using 1's Complement (pick A, B)
 │            │                                ├── Subtract using 2's Complement (pick A, B)
 │            │                                └── Back to Operation Menu
 │            │
 │            ├── BCD Operations         --> [ BCD OPERATIONS SUBMENU ]
 │            │                                ├── View 9's & 10's Complement Table
 │            │                                ├── BCD Addition (digit-by-digit +6 correction)
 │            │                                ├── BCD Subtract via 9's Complement (pick A, B)
 │            │                                ├── BCD Subtract via 10's Complement (pick A, B)
 │            │                                └── Back to Operation Menu
 │            │
 │            └── Return to Main Menu
 │
 ├── 2. Run Preset Combinations (Test Matrix)
 │       ├── Combination 1: Binary + Octal + Decimal
 │       ├── Combination 2: Binary + Decimal + Hexadecimal
 │       ├── Combination 3: Octal + Decimal + Hexadecimal
 │       ├── Combination 4: Binary + Octal + Hexadecimal
 │       └── Run ALL Combinations (Runs all conversions, arithmetic, complements & BCD)
 │
 ├── 3. View System Specifications & Requirements
 └── 4. Exit Program
```

---

## 📊 Summary of Mathematical Formulations

| Operation | Method / Formulation | Key Rules |
| :--- | :--- | :--- |
| **Positional Expansion** | $V = \sum_{i=0}^{n-1} d_i \cdot b^i + \sum_{j=1}^{m} d_{-j} \cdot b^{-j}$ | Converts any Base $b$ to Decimal (Base 10) |
| **Radix Conversion** | Successive division by $M$ (integer) + successive multiplication by $M$ (fraction) | Converts Decimal to target Base $M$ |
| **Mixed-Base Arithmetic** | $R = \text{eval}(X_1, X_2, \dots, X_N)$ via Decimal intermediates | Displayed in Binary, Octal, Decimal, and Hexadecimal |
| **Infix Expression** | Shunting-Yard Algorithm $\to$ RPN Stack Evaluation | `*` and `/` higher precedence than `+` and `-`; parentheses supported |
| **1's Complement** | Diminished Radix Complement: $\sim X$ or $(2^n - 1) - X$ | Inverts all bits across dynamically aligned bit-width |
| **2's Complement** | Radix Complement: $1\text{'s complement} + 1$ | Flip bits and add $1$ with carry propagation |
| **1's Comp Subtraction** | $A + \text{ones\_comp}(B)$ | End-around carry: carry-out added to LSB; no carry $\to$ negative result |
| **2's Comp Subtraction** | $A + \text{twos\_comp}(B)$ | Discard carry: carry-out discarded; no carry $\to$ negative result |
| **BCD Encoding** | 8421 Binary-Coded Decimal | Each decimal digit $0..9$ encoded into a 4-bit nibble `0000`..`1001` |
| **9's Complement (BCD)** | Diminished Radix: $(9 - d)$ for each decimal digit | Computed on decimal representation, displayed in BCD and all bases |
| **10's Complement (BCD)**| Radix Complement: $9\text{'s complement} + 1$ | Decimal 9's complement $+ 1$ with carry propagation |
| **BCD Addition** | Digit-by-digit sum: $S = d_A + d_B + \text{carry}$ | If $S > 9$ or carry generated, add $+6$ (`0110`) correction factor |
| **BCD 9's Subtraction** | $A + 9\text{'s comp}(B)$ with BCD $+6$ corrections | End-around carry added to least significant digit |
| **BCD 10's Subtraction**| $A + 10\text{'s comp}(B)$ with BCD $+6$ corrections | Discard final carry-out; if no carry, result is negative |

---

## 📑 Academic Documentation & References

- **Comprehensive System Specifications, Algorithms, Pseudocode & Flowcharts**:  
  📄 [docs/SYSTEM_SPECS.md](docs/SYSTEM_SPECS.md)
- **Verified Test Cases & Console Output Matrix**:  
  📄 [docs/TEST_CASES.md](docs/TEST_CASES.md)

---

## 💻 Technical Requirements

- **Operating System**: Windows 7 / 8 / 10 / 11, Linux, macOS
- **Compiler**: Any standard C++11 compliant compiler (`g++ >= 4.9`, `clang++ >= 3.4`, or MSVC)
- **Standard Library Only**: `<iostream>`, `<string>`, `<vector>`, `<cmath>`, `<sstream>`, `<iomanip>`, `<cctype>`, `<algorithm>`, `<chrono>`, `<thread>`
- **Terminal**: Supports ANSI escape codes (Windows Terminal, Windows CMD with VT100 enabled, Bash, Zsh)
