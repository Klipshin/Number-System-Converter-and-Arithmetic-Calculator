# Activity No. 1: Number System Converter and Arithmetic Calculator

## System Requirements & Specifications (C++ Implementation)

---

## 1. Project Overview

The **Number System Converter and Arithmetic Calculator** is a console-based software system developed in standard **C++ (C++11/C++17)**. It performs high-precision conversions across four standard numeral systems:

1. **Binary (Base 2)**
2. **Octal (Base 8)**
3. **Decimal (Base 10)**
4. **Hexadecimal (Base 16)**

This document details the complete system requirements, algorithms, pseudocode, and workflow flowchart for the complete **Multi-Input Converter and Arithmetic Calculator**.

---

## 2. System Requirements

### 2.1 Functional Requirements

1. **Multi-Input Acceptance**:
   - The program prompts the user to enter the number of inputs $N$ (where $N \ge 3$ is strictly enforced).
   - Validates that $N$ is a positive integer $\ge 3$.
2. **Independent Base Assignment**:
   - For each input $1 \dots N$, the user independently selects its base:
     - `[1]` Binary (Base 2)
     - `[2]` Octal (Base 8)
     - `[3]` Decimal (Base 10)
     - `[4]` Hexadecimal (Base 16)
3. **Strict Input Validation**:
   - The system validates every input character according to the chosen base:
     - Binary: `0`, `1`, and optional radix point `.`
     - Octal: `0` through `7`, and optional radix point `.`
     - Decimal: `0` through `9`, and optional radix point `.`
     - Hexadecimal: `0` through `9`, `A` through `F` (case-insensitive), and optional radix point `.`
   - If invalid characters are entered (e.g. `'2'` in binary, `'8'` in octal, `'G'` in hex), the program displays a specific error message and prompts the user to re-enter without crashing.
4. **Multi-Base Simultaneous Conversion**:
   - Every input is converted simultaneously into:
     - Binary (Base 2)
     - Octal (Base 8)
     - Decimal (Base 10)
     - Hexadecimal (Base 16)
5. **Formatted Tabular Output**:
   - Displays a clean, aligned ASCII table presenting original inputs and their 4 converted base representations.
6. **Mathematical Step-by-Step Breakdown**:
   - Allows the student to select any of the entered numbers to view the positional notation expansion ($N \to 10$) and decimal to target conversions ($10 \to M$).
7. **Preset Verification Suite**:
   - Includes 1-click built-in test suites for all combinations specified in Activity 1 (`BIN+OCT+DEC`, `BIN+DEC+HEX`, `OCT+DEC+HEX`, `BIN+OCT+HEX`).
8. **Mixed-Base Arithmetic Calculator**:
   - Lets the user select one operation after entering the input numbers:
     - Addition
     - Subtraction
     - Multiplication
     - Division
   - Converts all values to Decimal (Base 10) before performing the selected operation.
   - Displays the original mixed-base expression and the final answer in Binary, Octal, Decimal, and Hexadecimal.
   - Prevents division by zero and displays a clear error message.

### 2.2 Non-Functional Requirements

- **Language & Standard**: Standard C++11 (works in Dev-C++, Code::Blocks, Visual Studio, GCC).
- **Zero External Dependencies**: Uses only standard C++ library headers (`<iostream>`, `<string>`, `<vector>`, `<iomanip>`, `<cmath>`, `<cctype>`, `<sstream>`).
- **User Experience (UX)**: Clean text menus, formatted ASCII tables, clear error feedback, robust input stream handling.

---

## 3. Algorithms & Pseudocode

### 3.1 Algorithm 1: Input Validation

```text
Algorithm: Validate_Input(input_str, base)
Input: input_str (string), base (integer: 2, 8, 10, 16)
Output: boolean isValid, string errorMsg

1. If input_str is empty:
     errorMsg = "Input cannot be empty."
     Return False

2. Initialize dotCount = 0
3. For each character 'c' in input_str:
     If 'c' == '.':
       dotCount = dotCount + 1
       If dotCount > 1:
         errorMsg = "Multiple radix points (.) are not allowed."
         Return False
       Continue

     upper_c = ToUpper(c)
     digit_value = IndexOf(upper_c in "0123456789ABCDEF")

     If digit_value == NOT_FOUND or digit_value >= base:
       errorMsg = "Invalid character '" + c + "' for Base " + base
       Return False

4. If input_str == ".":
     errorMsg = "Input cannot be just a decimal point."
     Return False

5. Return True
```

### 3.2 Algorithm 2: Base-N to Decimal Intermediate Conversion ($N \to 10$)

```text
Algorithm: Convert_To_Decimal(input_str, base)
Input: input_str (string), base (integer)
Output: decimal_value (double)

1. Split input_str into integer_part and fractional_part by '.'
2. If integer_part is empty, set integer_part = "0"
3. decimal_value = 0.0
4. int_length = length(integer_part)

// Integer Part Positional Expansion
5. For i from 0 to (int_length - 1):
     digit = Char_To_Digit(integer_part[i])
     power = int_length - 1 - i
     decimal_value = decimal_value + (digit * (base ^ power))

// Fractional Part Positional Expansion
6. For j from 0 to (length(fractional_part) - 1):
     digit = Char_To_Digit(fractional_part[j])
     power = -(j + 1)
     decimal_value = decimal_value + (digit * (base ^ power))

7. Return decimal_value
```

### 3.3 Algorithm 3: Decimal to Target Base-M Conversion ($10 \to M$)

```text
Algorithm: Convert_From_Decimal(decimal_value, target_base, max_frac_precision = 6)
Input: decimal_value (double), target_base (integer), max_frac_precision (integer)
Output: target_str (string)

1. If decimal_value == 0:
     Return "0"

2. int_part = FLOOR(decimal_value)
3. frac_part = decimal_value - int_part
4. int_result = ""

// Integer Conversion via Successive Division
5. If int_part == 0:
     int_result = "0"
   Else:
     temp = int_part
     While temp > 0:
       rem = temp MOD target_base
       int_result = Digit_To_Char(rem) + int_result // Prepend
       temp = FLOOR(temp / target_base)

// Fractional Conversion via Successive Multiplication
6. frac_result = ""
7. If frac_part > 0:
     temp_frac = frac_part
     count = 0
     While temp_frac > 0 and count < max_frac_precision:
       temp_frac = temp_frac * target_base
       digit = FLOOR(temp_frac)
       frac_result = frac_result + Digit_To_Char(digit)
       temp_frac = temp_frac - digit
       count = count + 1
     Return int_result + "." + frac_result

8. Return int_result
```

### 3.4 Algorithm 4: Mixed-Base Arithmetic Operation

```text
Algorithm: Process_Arithmetic(numbers, operation)
Input: numbers (list of valid InputNumber records), operation
Output: arithmetic result in Binary, Octal, Decimal, Hexadecimal

1. Build expression using each original input value and its selected base.
2. Use each number's Decimal value as the common representation.
3. If operation is Addition:
     result = 0
     For each number:
       result = result + number.decimalValue
4. Else if operation is Subtraction:
     result = first number's decimalValue
     For each remaining number:
       result = result - number.decimalValue
5. Else if operation is Multiplication:
     result = 1
     For each number:
       result = result * number.decimalValue
6. Else if operation is Division:
     result = first number's decimalValue
     For each remaining number:
       If number.decimalValue == 0:
         Display "Division by zero is not allowed."
         Stop operation
       result = result / number.decimalValue
7. Convert final Decimal result to Binary, Octal, Decimal, and Hexadecimal.
8. Display the operation name, original expression, common representation, and final results.
```

---

## 4. System Flowchart

```mermaid
flowchart TD
    A([Start Program]) --> B[Display Header & Main Menu]
    B --> C{Select Option}

    C -- Option 1 -- --> D[Prompt for Number of Inputs N >= 3]
    D --> E{Is N >= 3?}
    E -- No -- --> D
    E -- Yes -- --> F[Loop i = 1 to N]

    F --> G[Select Base for Input #i: 2, 8, 10, 16]
    G --> H[Enter Value String for Input #i]
    H --> I{Validate Characters for Selected Base}
    I -- Invalid -- --> J[Display Specific Error Message & Re-prompt]
    J --> H

    I -- Valid -- --> K[Convert to Decimal Intermediate N -> 10]
    K --> L[Convert Decimal to Binary, Octal, Decimal, Hexadecimal]
    L --> M{More Inputs i < N?}
    M -- Yes -- --> F
    M -- No -- --> N[Print Formatted Conversion Results Table]

    N --> O[Select Arithmetic Operation]
    O --> P[Normalize All Inputs to Decimal]
    P --> T{Division by Zero?}
    T -- Yes -- --> U[Display Error Message]
    T -- No -- --> V[Perform Selected Arithmetic Operation]
    V --> W[Convert Final Result to Binary, Octal, Decimal, Hexadecimal]
    U --> X{Inspect Math Steps or Return?}
    W --> X
    X -- Inspect -- --> Y[Print Positional Expansion Proof for Selected Number]
    Y --> X
    X -- Return -- --> B

    C -- Option 2 -- --> Q[Run Preset Test Combinations Suite]
    Q --> N

    C -- Option 3 -- --> R[Display System Specifications]
    R --> B

    C -- Option 4 -- --> S([Exit Program])
```

---

## 5. Arithmetic Calculator Implementation

All inputs in the system are stored in a unified `InputNumber` structure containing a normalized `decimalValue`. The arithmetic engine performs:

- **Addition**: $R_{10} = \sum X_k$
- **Subtraction**: $R_{10} = X_1 - X_2 - \dots - X_n$
- **Multiplication**: $R_{10} = \prod X_k$
- **Division**: $R_{10} = X_1 \div X_2 \div \dots \div X_n$ (with check for division by zero).

The resulting decimal value is converted to all 4 bases using the existing `fromDecimal()` function. Negative subtraction results are supported with a leading minus sign.
