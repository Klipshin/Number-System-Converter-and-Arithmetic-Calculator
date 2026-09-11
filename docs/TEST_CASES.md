# Activity No. 1: Test Cases and Sample Console Output Matrix

---

## 1. Required Base Combinations Test Matrix

### Test Case Suite 1: Binary + Octal + Decimal

- **Preset Number**: `1`
- **Inputs**:
  1. Input 1: `101010` in Binary (Base 2)
  2. Input 2: `52` in Octal (Base 8)
  3. Input 3: `42` in Decimal (Base 10)

#### Console Output:

```text
  ========================================================================================
                                  CONVERSION RESULTS TABLE
  ========================================================================================
   #   Original Input Base    Binary (Base 2)     Octal (Base 8)  Decimal (10)  Hex (16)
  ----------------------------------------------------------------------------------------
   1   101010         BIN     101010              52              42            2A
   2   52             OCT     101010              52              42            2A
   3   42             DEC     101010              52              42            2A
  ========================================================================================
```

---

### Test Case Suite 2: Binary + Decimal + Hexadecimal

- **Preset Number**: `2`
- **Inputs**:
  1. Input 1: `11001100` in Binary (Base 2)
  2. Input 2: `204` in Decimal (Base 10)
  3. Input 3: `CC` in Hexadecimal (Base 16)

#### Console Output:

```text
  ========================================================================================
                                  CONVERSION RESULTS TABLE
  ========================================================================================
  #   Original Input  Base    Binary (Base 2)     Octal (Base 8)  Decimal (10)  Hex (16)
  ----------------------------------------------------------------------------------------
  1   11001100        BIN     11001100            314             204           CC
  2   204             DEC     11001100            314             204           CC
  3   CC              HEX     11001100            314             204           CC
  ========================================================================================
```

---

### Test Case Suite 3: Octal + Decimal + Hexadecimal

- **Preset Number**: `3`
- **Inputs**:
  1. Input 1: `755` in Octal (Base 8)
  2. Input 2: `493` in Decimal (Base 10)
  3. Input 3: `1ED` in Hexadecimal (Base 16)

#### Console Output:

```text
  ========================================================================================
                                  CONVERSION RESULTS TABLE
  ========================================================================================
  #   Original Input  Base    Binary (Base 2)     Octal (Base 8)  Decimal (10)  Hex (16)
  ----------------------------------------------------------------------------------------
  1   755             OCT     111101101           755             493           1ED
  2   493             DEC     111101101           755             493           1ED
  3   1ED             HEX     111101101           755             493           1ED
  ========================================================================================
```

---

### Test Case Suite 4: Binary + Octal + Hexadecimal

- **Preset Number**: `4`
- **Inputs**:
  1. Input 1: `11110000` in Binary (Base 2)
  2. Input 2: `360` in Octal (Base 8)
  3. Input 3: `F0` in Hexadecimal (Base 16)

#### Console Output:

```text
  ========================================================================================
                                  CONVERSION RESULTS TABLE
  ========================================================================================
  #   Original Input  Base    Binary (Base 2)     Octal (Base 8)  Decimal (10)  Hex (16)
  ----------------------------------------------------------------------------------------
  1   11110000        BIN     11110000            360             240           F0
  2   360             OCT     11110000            360             240           F0
  3   F0              HEX     11110000            360             240           F0
  ========================================================================================
```

---

## 2. Interactive Input Validation & Error Handling Tests

| Test ID    | Input Value | Selected Base          | Expected System Response                                                                                   | Status |
| :--------- | :---------- | :--------------------- | :--------------------------------------------------------------------------------------------------------- | :----- |
| **VAL-01** | `102`       | Binary (Base 2)        | Displays `[!] ERROR: Invalid character '2' for Binary (Base 2). Please re-enter.` and prompts again.       | PASSED |
| **VAL-02** | `89`        | Octal (Base 8)         | Displays `[!] ERROR: Invalid character '8' for Octal (Base 8). Please re-enter.` and prompts again.        | PASSED |
| **VAL-03** | `12A`       | Decimal (Base 10)      | Displays `[!] ERROR: Invalid character 'A' for Decimal (Base 10). Please re-enter.` and prompts again.     | PASSED |
| **VAL-04** | `1G3`       | Hexadecimal (Base 16)  | Displays `[!] ERROR: Invalid character 'G' for Hexadecimal (Base 16). Please re-enter.` and prompts again. | PASSED |
| **VAL-05** | `10.1.1`    | Binary (Base 2)        | Displays `[!] ERROR: Multiple radix points (.) are not allowed. Please re-enter.`                          | PASSED |
| **VAL-06** | `2`         | Number of Inputs ($N$) | Displays `[!] Error: Value must be between 3 and 20.` and re-prompts.                                      | PASSED |

---

## 3. Mathematical Step-by-Step Sample Output

When the user selects **Option 1** to inspect Input #1 (`101010` in Binary):

```text
  ------------------------------------------------------------
  MATHEMATICAL STEP-BY-STEP PROOF FOR INPUT #1
  Input: 101010 in Binary (Base 2)
  ------------------------------------------------------------
  Step 1: Conversion to Decimal (Base 10) using Positional Notation
  Formula: Value = Sum( Digit * Base^Position )

  Expansion:
  = (1 * 2^5) + (0 * 2^4) + (1 * 2^3) + (0 * 2^2) + (1 * 2^1) + (0 * 2^0)
  = 42 (Decimal)

  Step 2: Conversion from Decimal (42) to Target Bases
    -> Binary (Base 2):       101010 (Base 2)
    -> Octal (Base 8):        52 (Base 8)
    -> Decimal (Base 10):     42 (Base 10)
    -> Hexadecimal (Base 16): 2A (Base 16)
  ------------------------------------------------------------
```

---

## 4. Arithmetic Calculator Test Suite

Below are the pre-calculated test cases implemented in the Arithmetic Calculator presets. Together, they cover the required base combinations and all four arithmetic operations.

### Expression 1: Mixed Base Addition ($N_1 + N_2 + N_3$)

- **Base Combination**: Binary + Octal + Decimal
- **Input Expression**: $101010_2 + 52_8 + 42_{10}$
- **Decimal Normalization**: $42_{10} + 42_{10} + 42_{10} = 126_{10}$
- **Multi-Base Final Results**:
  - Binary: `1111110`
  - Octal: `176`
  - Decimal: `126`
  - Hexadecimal: `7E`

### Expression 2: Mixed Base Subtraction ($N_1 - N_2 - N_3$)

- **Base Combination**: Binary + Decimal + Hexadecimal
- **Input Expression**: $11001100_2 - 100_{10} - 40_{16}$
- **Decimal Normalization**: $204_{10} - 100_{10} - 64_{10} = 40_{10}$
- **Multi-Base Final Results**:
  - Binary: `101000`
  - Octal: `50`
  - Decimal: `40`
  - Hexadecimal: `28`

### Expression 3: Mixed Base Multiplication ($N_1 \times N_2 \times N_3$)

- **Base Combination**: Octal + Decimal + Hexadecimal
- **Input Expression**: $7_8 \times 5_{10} \times 3_{16}$
- **Decimal Normalization**: $7_{10} \times 5_{10} \times 3_{10} = 105_{10}$
- **Multi-Base Final Results**:
  - Binary: `1101001`
  - Octal: `151`
  - Decimal: `105`
  - Hexadecimal: `69`

### Expression 4: Mixed Base Division ($N_1 \div N_2 \div N_3$)

- **Base Combination**: Binary + Octal + Hexadecimal
- **Input Expression**: $11110000_2 \div 10_8 \div 3_{16}$
- **Decimal Normalization**: $240_{10} \div 8_{10} \div 3_{10} = 10_{10}$
- **Multi-Base Final Results**:
  - Binary: `1010`
  - Octal: `12`
  - Decimal: `10`
  - Hexadecimal: `A`

### Error Case: Division by Zero

- **Input Expression**: $1010_2 \div 0_{10} \div 2_8$
- **Expected System Response**: Displays `[!] ERROR: Division by zero is not allowed.`
- **Expected Result Fields**: `UNDEFINED` for Binary, Octal, Decimal, and Hexadecimal.
