# Activity No. 1 — Number System Converter and Arithmetic Calculator

## System Requirements, Algorithm / Pseudocode, and Flowchart

---

## 1. Project Overview

The **Number System Converter and Arithmetic Calculator** is a console-based software system developed in standard **C++ (C++11)**. It converts multiple numbers across four numeral systems and evaluates mixed-base arithmetic expressions with full operator precedence support.

| Base | Name        | Valid Characters |
| ---- | ----------- | ---------------- |
| 2    | Binary      | `0`, `1`         |
| 8    | Octal       | `0`–`7`          |
| 10   | Decimal     | `0`–`9`          |
| 16   | Hexadecimal | `0`–`9`, `A`–`F` |

Radix-point fractions (e.g. `101.101`, `1A.F`) are supported for conversion in all bases.

---

## 2. System Requirements

### 2.1 Functional Requirements

| #     | Requirement                             | Description                                                                                                                                                         |
| ----- | --------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| FR-01 | **Multi-Input Acceptance**              | Prompts user for count N >= 3 (validated, max 20).                                                                                                                  |
| FR-02 | **Independent Base Selection**          | Each input independently assigns one of: Binary, Octal, Decimal, Hexadecimal.                                                                                       |
| FR-03 | **Strict Character Validation**         | Every character is validated against the chosen base; invalid chars produce a descriptive error and re-prompt without crashing.                                     |
| FR-04 | **Radix Fraction Support**              | Accepts and correctly converts numbers with a single radix point (e.g. `10.11` BIN = `2.75` DEC).                                                                   |
| FR-05 | **4-Base Simultaneous Conversion**      | Each input is converted to Binary, Octal, Decimal, and Hexadecimal in one step.                                                                                     |
| FR-06 | **Formatted Table Output**              | Conversion results displayed in a centered, color-coded ASCII table.                                                                                                |
| FR-07 | **Step-by-Step Math Proof**             | User can inspect positional-notation expansion (N->10) and successive-division/multiplication (10->M) for any input.                                                |
| FR-08 | **Standard Arithmetic Operations**      | Addition, Subtraction, Multiplication, Division applied across all N inputs; result displayed in all 4 bases.                                                       |
| FR-09 | **Custom Expression Evaluator**         | User types any infix expression using variables `a`-`z` (mapped to inputs 1-N) and operators `+ - * /` with parentheses.                                            |
| FR-10 | **Operator Precedence & Associativity** | `*` and `/` bind tighter than `+` and `-`; all operators are left-associative; parentheses override precedence.                                                     |
| FR-11 | **Implicit Multiplication**             | Adjacent terms without an explicit operator (e.g. `a(b+c)`, `(a+b)c`) automatically insert `*`.                                                                     |
| FR-12 | **Division-by-Zero Protection**         | Detects and reports division-by-zero for both simple and custom expression modes; result fields show `UNDEFINED`.                                                   |
| FR-13 | **Try Another Operation (Back)**        | After viewing a result, user returns to the operation selector and tries a different operation without re-entering numbers.                                         |
| FR-14 | **Preset Test Suite**                   | 4 built-in number combinations (BIN+OCT+DEC, BIN+DEC+HEX, OCT+DEC+HEX, BIN+OCT+HEX); each runs all 4 arithmetic operations and complement operations automatically. |
| FR-15 | **Keyboard Navigation**                 | Scrollable menus with Arrow keys or W/S, confirmed with Enter or Space.                                                                                             |
| FR-16 | **1's Complement (Diminished Radix)**   | For any integer input, computes the 1's complement (flip all bits for binary; `(r^n - 1) - X` generalized) displayed in all 4 bases.                                |
| FR-17 | **2's Complement (Radix Complement)**   | Computes the 2's complement (`1's complement + 1`) for any integer input, shown in all 4 bases.                                                                     |
| FR-18 | **Complement Table View**               | Displays a combined 1's & 2's complement table for all entered inputs, with bit-width, padded binary, and all-bases conversion.                                     |
| FR-19 | **Subtraction via 1's Complement**          | Performs `A − B` (Input 1 − Input 2) using the end-around carry method; shows every step and result in all 4 bases.                                                                               |
| FR-20 | **Subtraction via 2's Complement**          | Performs `A − B` (Input 1 − Input 2) using the discard-carry method; shows every step and result in all 4 bases.                                                                                  |
| FR-21 | **BCD Encoding**                            | Any integer input (in any base) is converted to decimal first; each decimal digit is encoded as a 4-bit BCD group (e.g. `42` → `0100 0010`).                                                      |
| FR-22 | **9's & 10's Complement Table (BCD)**       | Displays for each input: decimal value, BCD encoding, 9's complement (`9 − d` per digit) and 10's complement (9's comp + 1) with BCD form and all-bases conversion.                               |
| FR-23 | **BCD Addition**                            | Performs digit-by-digit BCD addition of two operands; applies the +6 correction factor when a digit sum exceeds 9; shows every digit step and result in BCD and all 4 bases.                      |
| FR-24 | **BCD Subtraction via 9's Complement**      | Performs `A − B` using BCD 9's complement (diminished radix): each digit `d` of B → `9 − d`; adds to A with BCD correction; applies end-around carry; shows all steps and result in all 4 bases. |
| FR-25 | **BCD Subtraction via 10's Complement**     | Performs `A − B` using BCD 10's complement (radix complement): 9's comp + 1; adds to A with BCD correction; discards final carry; shows all steps and result in all 4 bases.                      |

### 2.2 Non-Functional Requirements

| #      | Requirement                                                                                                                                                                       |
| ------ | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| NFR-01 | Language: Standard **C++11** — compatible with Dev-C++ MinGW GCC 4.9, Code::Blocks, Visual Studio, GCC.                                                                           |
| NFR-02 | Zero external dependencies — only standard headers: `<iostream>`, `<string>`, `<vector>`, `<cmath>`, `<sstream>`, `<iomanip>`, `<cctype>`, `<algorithm>`, `<chrono>`, `<thread>`. |
| NFR-03 | ANSI color codes used for color output on Windows Terminal and compatible consoles.                                                                                               |
| NFR-04 | Robust input handling — no crashes or infinite loops on any user input.                                                                                                           |
| NFR-05 | Centered, formatted UI at configurable terminal width (default 92 columns).                                                                                                       |

---

## 3. Algorithms and Pseudocode

### 3.1 Algorithm 1: Input Validation

```
Algorithm: Validate_Input(input_str, base)
Input:  input_str (string), base in {2, 8, 10, 16}
Output: isValid (bool), errorMsg (string)

1.  IF input_str is empty:
        errorMsg = "Input cannot be empty."
        Return FALSE

2.  dotCount <- 0

3.  FOR EACH character c IN input_str:
        IF c == '.':
            dotCount <- dotCount + 1
            IF dotCount > 1:
                errorMsg = "Multiple radix points not allowed."
                Return FALSE
            CONTINUE
        upper_c   <- ToUpper(c)
        digit_val <- Position of upper_c in "0123456789ABCDEF"
        IF digit_val == NOT_FOUND OR digit_val >= base:
            errorMsg = "Invalid character '" + c + "' for " + BaseName(base)
            Return FALSE

4.  IF input_str == ".":
        errorMsg = "Cannot be just a decimal point."
        Return FALSE

5.  Return TRUE
```

---

### 3.2 Algorithm 2: Base-N to Decimal Conversion (N -> 10)

```
Algorithm: To_Decimal(input_str, base)
Input:  input_str (string), base (integer)
Output: decimal_value (double)

1.  Split input_str at '.' -> integer_part, fractional_part
2.  IF integer_part is empty -> set integer_part = "0"
3.  decimal_value <- 0.0
4.  int_len <- length(integer_part)

    // Integer part: positional expansion
5.  FOR i FROM 0 TO int_len - 1:
        digit         <- Char_To_Digit(integer_part[i])
        power         <- int_len - 1 - i
        decimal_value <- decimal_value + digit * (base ^ power)

    // Fractional part: positional expansion
6.  FOR j FROM 0 TO length(fractional_part) - 1:
        digit         <- Char_To_Digit(fractional_part[j])
        power         <- -(j + 1)
        decimal_value <- decimal_value + digit * (base ^ power)

7.  Return decimal_value
```

---

### 3.3 Algorithm 3: Decimal to Base-M Conversion (10 -> M)

```
Algorithm: From_Decimal(decimal_value, target_base, max_frac = 6)
Input:  decimal_value (double), target_base (int), max_frac (int)
Output: result_str (string)

1.  IF decimal_value == 0 -> Return "0"
2.  is_negative <- (decimal_value < 0)
3.  abs_val   <- absolute value of decimal_value
4.  int_part  <- FLOOR(abs_val)
5.  frac_part <- abs_val - int_part
6.  int_str   <- ""

    // Integer part: successive division
7.  IF int_part == 0:
        int_str = "0"
    ELSE:
        temp <- int_part
        WHILE temp > 0:
            rem     <- temp MOD target_base
            int_str <- DIGITS[rem] + int_str   // prepend digit
            temp    <- FLOOR(temp / target_base)

    // Fractional part: successive multiplication
8.  frac_str <- ""
9.  IF frac_part > 0:
        temp_frac <- frac_part
        count <- 0
        WHILE temp_frac > 0 AND count < max_frac:
            temp_frac <- temp_frac * target_base
            digit     <- FLOOR(temp_frac)
            frac_str  <- frac_str + DIGITS[digit]
            temp_frac <- temp_frac - digit
            count     <- count + 1

10. IF is_negative -> int_str = "-" + int_str
11. IF frac_str != "" -> Return int_str + "." + frac_str
12. Return int_str
```

---

### 3.4 Algorithm 4: Standard Arithmetic Operation

```
Algorithm: Process_Arithmetic(numbers[], operation)
Input:  numbers[] (list of InputNumber), operation in {ADD, SUB, MUL, DIV}
Output: ArithmeticResult (expression string, decimal result, BIN/OCT/DEC/HEX)

1.  Build original expression string:
        FOR EACH number[i]:
            expression <- expression + number[i].originalStr + " [" + BaseCode(i) + "]"
            IF not last -> append " <op_symbol> "

2.  CASE operation:
        ADD: result <- 0
             FOR EACH n: result <- result + n.decimalValue
        SUB: result <- numbers[0].decimalValue
             FOR i FROM 1 TO N-1: result <- result - numbers[i].decimalValue
        MUL: result <- 1
             FOR EACH n: result <- result * n.decimalValue
        DIV: result <- numbers[0].decimalValue
             FOR i FROM 1 TO N-1:
                 IF numbers[i].decimalValue == 0:
                     isValid  <- FALSE
                     errorMsg <- "Division by zero is not allowed."
                     STOP
                 result <- result / numbers[i].decimalValue

3.  IF isValid:
        binStr <- From_Decimal(result, 2)
        octStr <- From_Decimal(result, 8)
        decStr <- Format(result)
        hexStr <- From_Decimal(result, 16)
    ELSE:
        binStr = octStr = decStr = hexStr = "UNDEFINED"

4.  Build decimal expression:  decExpr = "X1 op X2 op ... = result"
5.  Return ArithmeticResult
```

---

### 3.6 Algorithm 6: 1's & 2's Complement Computation

```
Algorithm: Compute_Complements(num)
Input:  num (InputNumber with integer binary representation)
Output: ComplementResult (bitWidth, paddedBin, onesComp_*, twosComp_*)

1. IF num has a radix-point fraction:
       Note = "Fractional numbers — complement not applicable."
       Return (applicable = FALSE)

2. rawBin      <- num.binStr  (integer binary, no '.' )
3. reqBits     <- length(rawBin)
4. bitWidth    <- smallest multiple of 8  that is >= reqBits  (min 8)
5. paddedBin   <- LeftPad(rawBin, bitWidth, '0')

6. onesComp_bin <- FlipAllBits(paddedBin)
       // Each '0' -> '1', each '1' -> '0'
7. onesDecVal   <- To_Decimal(onesComp_bin, 2)
8. Convert onesDecVal -> OCT, DEC, HEX

9. twosComp_bin <- AddOne(onesComp_bin)  // carry wraps within bitWidth
10. twosDecVal  <- To_Decimal(twosComp_bin, 2)
11. Convert twosDecVal -> OCT, DEC, HEX

12. Return ComplementResult
```

---

### 3.7 Algorithm 7: Subtraction by Complement Method

```
Algorithm: Complement_Subtract(A, B, useTwos)
Input:  A, B (InputNumber — integers only), useTwos (bool)
Output: CompSubResult (step data, final result in BIN/OCT/DEC/HEX)

1. IF A or B have fractional parts:
       errorMsg = "Complement subtraction requires integer inputs."
       Return (isValid = FALSE)

2. rawA <- A.binStr,  rawB <- B.binStr
3. reqBits  <- max(length(rawA), length(rawB))
4. bitWidth <- smallest multiple of 8 >= reqBits  (min 8)
5. A_bin <- LeftPad(rawA, bitWidth, '0')
6. B_bin <- LeftPad(rawB, bitWidth, '0')

   // Step 1: compute complement of B
7. IF useTwos:
       comp_bin <- TwosComplement(B_bin)   // flip + add 1
   ELSE:
       comp_bin <- OnesComplement(B_bin)   // flip all bits

   // Step 2: binary addition
8. (sum_bin, carryOut) <- BinaryAdd(A_bin, comp_bin)

   // Step 3: carry adjustment
9. IF useTwos:
       adjusted_bin <- sum_bin         // discard carryOut
       isNegative   <- (carryOut == FALSE)
   ELSE:
       IF carryOut:
           adjusted_bin <- BinaryAdd(sum_bin, "1")  // end-around carry
           isNegative   <- FALSE
       ELSE:
           adjusted_bin <- OnesComplement(sum_bin)  // get magnitude
           isNegative   <- TRUE

10. mag <- To_Decimal(adjusted_bin, 2)
11. decimalResult <- isNegative ? -mag : mag
12. Convert decimalResult -> BIN, OCT, DEC, HEX
13. Return CompSubResult
```

---

### 3.8 Algorithm 8: BCD Addition

```
Algorithm: BCD_Add(A, B)
Input:  A, B (InputNumber — non-negative integers only)
Output: BCDAddResult (digit steps, result in BCD and BIN/OCT/DEC/HEX)

1. A_dec <- decimal string of A   (via toDecimal then formatDecimalValue)
2. B_dec <- decimal string of B
3. Pad A_dec and B_dec to equal length with leading zeros
4. nDigits <- length(padded string)

5. carry <- 0,  resultDigits <- ""
6. FOR i FROM nDigits-1 DOWN TO 0  (LSDigit to MSDigit):
       digitA   <- A_dec[i] - '0'
       digitB   <- B_dec[i] - '0'
       rawSum   <- digitA + digitB + carry
       rawBin   <- 4-bit binary of rawSum

       IF rawSum > 9:
           corrected   <- rawSum + 6        // BCD correction factor
           carry       <- corrected / 10    // decimal carry to next digit
           bcdDigitOut <- corrected % 10
       ELSE:
           corrected   <- rawSum
           carry       <- 0
           bcdDigitOut <- rawSum

       Append bcdDigitOut to front of resultDigits
       Record BCDDigitStep (digitA, digitB, carryIn, rawSum, rawBin,
                            needCorrect, corrected, carryOut, bcdDigitOut)

7. IF carry > 0:
       Prepend carry digit to resultDigits  (overflow carry)

8. result_dec <- trim leading zeros from resultDigits
9. result_bcd <- decToBCD(result_dec)
10. Convert result_dec -> BIN, OCT, HEX
11. Return BCDAddResult
```

---

### 3.9 Algorithm 9: BCD Subtraction via 9's and 10's Complement

```
Algorithm: BCD_Subtract(A, B, useTens)
Input:  A, B (InputNumber — non-negative integers only),
        useTens (bool: false = 9's complement, true = 10's complement)
Output: BCDSubResult (digit steps, carry adjustment, result in all bases)

1. A_dec, B_dec <- decimal strings of A and B (padded to equal length)

   // Step 1: Compute complement of B
2. FOR EACH digit d in B_dec:
       compDigit[i] <- 9 - d       // 9's complement of each digit
   IF useTens:
       Add 1 with carry through compDigits (from right)
                                   // produces 10's complement

   // Step 2: Add A + comp(B) digit-by-digit with BCD correction
3. carry <- 0,  resultDigits <- ""
4. FOR i FROM nDigits-1 DOWN TO 0:
       rawSum   <- A_dec[i] + compDigit[i] + carry
       IF rawSum > 9:
           corrected   <- rawSum + 6
           carry       <- corrected / 10
           bcdDigitOut <- corrected % 10
       ELSE:
           carry       <- 0
           bcdDigitOut <- rawSum
       Append bcdDigitOut to front of resultDigits

   hasCarry <- (carry != 0)

   // Step 3: Carry adjustment
5. IF NOT useTens (9's complement):
       IF hasCarry:
           End-Around Carry: add 1 to resultDigits (with BCD correction)
           isNegative <- FALSE
       ELSE:
           Take 9's complement of resultDigits to get magnitude
           isNegative <- TRUE
   ELSE (10's complement):
       IF hasCarry:
           Discard carry
           isNegative <- FALSE
       ELSE:
           Take 10's complement of resultDigits to get magnitude
           isNegative <- TRUE

6. result_dec  <- trim resultDigits
7. result_bcd  <- decToBCD(result_dec)
8. decimalResult <- isNegative ? -value(result_dec) : +value(result_dec)
9. Convert decimalResult -> BIN, OCT, HEX
10. Return BCDSubResult
```

---

### 3.5 Algorithm 5: Custom Expression Evaluator (Shunting-Yard)

Parses and evaluates any infix expression such as `(a+b)*c-d` where variables
`a`–`z` map to inputs 1–N in order.

#### Step A — Lexer with Implicit Multiplication Pass

```
Algorithm: Tokenize_Expression(expr_str, num_vars)
Input:  expr_str (string), num_vars (int)
Output: tokens[] (Token list), errorMsg (string)

Token kinds: TOK_VAR, TOK_OP, TOK_LPAREN, TOK_RPAREN, TOK_END

1.  raw_tokens <- []
2.  FOR EACH character c IN expr_str:
        SKIP whitespace
        IF c == '(' -> push TOK_LPAREN
        IF c == ')' -> push TOK_RPAREN
        IF c in {'+', '-', '*', '/'} -> push TOK_OP(c)
        IF c is a letter:
            idx <- lowercase(c) - 'a'
            IF idx >= num_vars:
                errorMsg = "Variable '" + c + "' out of range."
                Return []
            push TOK_VAR(idx)
        ELSE:
            errorMsg = "Invalid character '" + c + "'"
            Return []
3.  push TOK_END

    // Implicit multiplication pass
    // Insert '*' between adjacent tokens that imply multiplication:
4.  expanded <- []
5.  FOR j FROM 0 TO length(raw_tokens) - 1:
        expanded.push(raw_tokens[j])
        IF j + 1 < length(raw_tokens):
            cur  <- raw_tokens[j].kind
            next <- raw_tokens[j+1].kind
            needMul =  (cur == VAR    AND next == LPAREN)   // a(b+c)
                    OR (cur == RPAREN AND next == VAR)       // (a+b)c
                    OR (cur == RPAREN AND next == LPAREN)    // (a+b)(c+d)
                    OR (cur == VAR    AND next == VAR)       // ab
            IF needMul:
                expanded.push(TOK_OP('*'))
6.  Return expanded
```

#### Step B — Shunting-Yard: Infix to Postfix (RPN)

```
Algorithm: Infix_To_Postfix(tokens[])
Input:  tokens[] (from lexer)
Output: postfix[] (RPN token list), errorMsg (string)

Precedence:  '*' = '/' = 2 (high),  '+' = '-' = 1 (low)
Associativity: all left-associative

1.  output_queue <- [],  op_stack <- []
2.  FOR EACH token t IN tokens[]:
        IF t.kind == TOK_END -> BREAK
        IF t is VAR:
            output_queue.enqueue(t)
        IF t is OP:
            WHILE op_stack not empty
                  AND top(op_stack) is OP
                  AND precedence(top) >= precedence(t):
                output_queue.enqueue(op_stack.pop())
            op_stack.push(t)
        IF t is LPAREN:
            op_stack.push(t)
        IF t is RPAREN:
            WHILE op_stack not empty AND top(op_stack) != LPAREN:
                output_queue.enqueue(op_stack.pop())
            IF op_stack is empty:
                errorMsg = "Mismatched parentheses: extra ')' detected."
                Return []
            op_stack.pop()   // discard the matching LPAREN

3.  WHILE op_stack not empty:
        IF top(op_stack) is LPAREN:
            errorMsg = "Mismatched parentheses: unclosed '(' detected."
            Return []
        output_queue.enqueue(op_stack.pop())

4.  Return output_queue
```

#### Step C — RPN Stack Evaluator

```
Algorithm: Evaluate_Postfix(postfix[], numbers[])
Input:  postfix[] (RPN token list), numbers[] (InputNumber list)
Output: result (double), errorMsg (string)

1.  value_stack <- []
2.  FOR EACH token t IN postfix[]:
        IF t is VAR:
            value_stack.push(numbers[t.varIdx].decimalValue)
        IF t is OP:
            IF size(value_stack) < 2:
                errorMsg = "Not enough operands for operator '" + t.op + "'"
                Return error
            b <- value_stack.pop()
            a <- value_stack.pop()
            CASE t.op:
                '+': value_stack.push(a + b)
                '-': value_stack.push(a - b)
                '*': value_stack.push(a * b)
                '/': IF b == 0:
                         errorMsg = "Division by zero: divisor evaluates to 0."
                         Return error
                     value_stack.push(a / b)

3.  IF size(value_stack) != 1:
        errorMsg = "Malformed expression."
        Return error
4.  Return value_stack.top()
```

---

## 4. System Flowchart

```mermaid
flowchart TD
    A([Start Program]) --> B[Display Tetris Animation\nASCII Intro Banner]
    B --> C[Main Menu]
    C --> D{User Choice}

    D -- "Option 1\nInteractive Converter" --> E["Prompt for count N\nminimum 3, max 20"]
    E --> F{N >= 3?}
    F -- No --> E
    F -- Yes --> G[Loop i = 1 to N]

    G --> H["Select Base for Input i\nBIN / OCT / DEC / HEX"]
    H --> I["Enter value string for Input i"]
    I --> J{Valid characters\nfor chosen base?}
    J -- Invalid --> K[Show specific error message\nre-prompt]
    K --> I
    J -- Valid --> L["Convert Input i\nN to 10 then 10 to 2 8 16"]
    L --> M{More inputs\ni < N?}
    M -- Yes --> G
    M -- No --> N["Show Conversion Results Table\nShow Variable Map\na = Input 1, b = Input 2 ..."]

    N --> OL["OPERATION LOOP\nUser keeps same numbers\nuntil Return chosen"]
    OL --> OP["Main Operation Menu\n0: Addition  1: Subtraction\n2: Multiplication  3: Division\n4: Custom Expression\n5: Complement Operations\n6: Return to Main Menu"]
    OP -- "Option 6" --> C

    OP -- "0-3\nStandard ops" --> PA[Normalize all inputs\nto Decimal]
    PA --> PB{Any divisor\n== 0?}
    PB -- Yes --> PC["Show ERROR: Division by zero\nResult = UNDEFINED"]
    PB -- No --> PD[Perform arithmetic\non decimal values]
    PD --> PE[Convert result to\nBIN OCT DEC HEX]
    PE --> PF[Show Arithmetic Result]

    OP -- "Option 4\nCustom Expression" --> CE1["Show Variable Map\nUser types expression\ne.g. a times open-b+c-close minus d"]
    CE1 --> CE2["Lexer: tokenize +\ninsert implicit * tokens"]
    CE2 --> CE3{Lex error?}
    CE3 -- Yes --> CE4[Show error\nre-prompt]
    CE4 --> CE1
    CE3 -- No --> CE5["Shunting-Yard\nInfix to Postfix RPN"]
    CE5 --> CE6{Parenthesis\nerror?}
    CE6 -- Yes --> CE4
    CE6 -- No --> CE7[Evaluate RPN\nstack machine]
    CE7 --> CE8{Runtime error?\ndiv by zero etc}
    CE8 -- Yes --> PC
    CE8 -- No --> PE

    OP -- "Option 5\nComplement Operations" --> SUB["Complement Submenu\n0: View 1s & 2s Comp Table\n1: Subtract using 1s Comp\n2: Subtract using 2s Comp\n3: View 9s & 10s Comp Table BCD\n4: BCD Addition\n5: BCD Sub via 9s Comp\n6: BCD Sub via 10s Comp\n7: Back"]
    SUB -- "Option 7 Back" --> OL

    SUB -- "Option 0\n1s & 2s Comp Table" --> CT1["For each input:\nCheck integer-only"]
    CT1 --> CT2["Pad binary to bitWidth\nCompute 1s comp flip bits\nCompute 2s comp add 1"]
    CT2 --> CT3["Convert complements\nto OCT DEC HEX"]
    CT3 --> CT4[Show 1s & 2s Complement Table]
    CT4 --> CPF[Show Complement Result]

    SUB -- "Option 1 or 2\n1s or 2s Comp Sub" --> CS0["Pick A Minuend\nfrom N inputs"]
    CS0 --> CS0B["Pick B Subtrahend\nfrom N inputs"]
    CS0B --> CS2{"Both are\nintegers?"}
    CS2 -- No --> CSE[Show error\nNot applicable]
    CS2 -- Yes --> CS3["Pad A and B to same bitWidth"]
    CS3 --> CS4["Compute complement of B"]
    CS4 --> CS5["Add A plus complement of B\nRecord carryOut"]
    CS5 --> CS6{"1s method?"}
    CS6 -- Yes --> CS7{carryOut?}
    CS7 -- Yes --> CS8["End-around carry: add carry back"]
    CS7 -- No --> CS9["Take 1s comp = magnitude\nMark negative"]
    CS6 -- No --> CS10{carryOut?}
    CS10 -- Yes --> CS11["Discard carry: positive result"]
    CS10 -- No --> CS12["Take 2s comp = magnitude\nMark negative"]
    CS8 --> CPF
    CS9 --> CPF
    CS11 --> CPF
    CS12 --> CPF
    CSE --> CPF

    SUB -- "Option 3\n9s & 10s Comp Table BCD" --> BT1["For each input:\nConvert to decimal string"]
    BT1 --> BT2["9s comp: each digit d -> 9-d\n10s comp: 9s comp + 1 with carry"]
    BT2 --> BT3["Convert complements to BCD\nOCT DEC HEX BIN"]
    BT3 --> CPF

    SUB -- "Option 4\nBCD Addition" --> BA0["Pick A and B\nfrom N inputs"]
    BA0 --> BA1["Convert to decimal strings\nPad to equal digit count"]
    BA1 --> BA2["For each digit pair LSDigit to MSDigit:\nrawSum = digitA + digitB + carry"]
    BA2 --> BA3{rawSum > 9?}
    BA3 -- Yes --> BA4["Apply correction: +6\ncarry = corrected / 10\nbcdDigit = corrected % 10"]
    BA3 -- No --> BA5["No correction\nbcdDigit = rawSum  carry = 0"]
    BA4 --> BA6[Next digit]
    BA5 --> BA6
    BA6 --> BA7{More digits?}
    BA7 -- Yes --> BA2
    BA7 -- No --> BA8["Prepend final carry if any\nBuild result BCD string"]
    BA8 --> CPF

    SUB -- "Option 5 or 6\nBCD Sub 9s or 10s" --> BS0["Pick A and B"]
    BS0 --> BS1["Compute 9s complement of B digits\nIF 10s: add 1 with carry"]
    BS1 --> BS2["Add A + comp B digit by digit\nwith BCD correction"]
    BS2 --> BS3{hasCarry?}
    BS3 -- "9s comp & carry" --> BS4["End-Around Carry\nadd 1 to LSDigit\nPositive result"]
    BS3 -- "9s comp & no carry" --> BS5["Take 9s comp of raw sum\nNegative result"]
    BS3 -- "10s comp & carry" --> BS6["Discard carry\nPositive result"]
    BS3 -- "10s comp & no carry" --> BS7["Take 10s comp of raw sum\nNegative result"]
    BS4 --> CPF
    BS5 --> CPF
    BS6 --> CPF
    BS7 --> CPF

    CPF --> CPOST[Post-Complement Options]
    CPOST --> CPS{User choice}
    CPS -- "View Math Steps" --> MS2["Show positional expansion\nand successive division proof"]
    MS2 --> CPOST
    CPS -- "View Result Again" --> CPF
    CPS -- "Try Another Complement Op" --> SUB
    CPS -- "Back to Operation Menu" --> OL

    PC --> PF
    PF --> POST[Post-Result Options Menu]
    POST --> PS{User choice}
    PS -- "View Math Steps\nfor Input i" --> MS["Show positional expansion\nand successive division proof"]
    MS --> POST
    PS -- "View Result Again" --> PF
    PS -- "Try Another Operation\nkeep same numbers" --> OL
    PS -- "Return to Main Menu" --> C

    D -- "Option 2\nPreset Combinations" --> PR["Select Combination\n1: BIN+OCT+DEC\n2: BIN+DEC+HEX\n3: OCT+DEC+HEX\n4: BIN+OCT+HEX\nor Run All"]
    PR --> PRR["Run ALL 4 arithmetic ops\nComplement Table\n1s and 2s Complement Sub\nfor selected combo"]
    PRR["Run ALL 4 arithmetic ops\nComplement Table\n1s and 2s Complement Sub\nBCD Comp Table\nBCD Add and BCD Sub"] --> C

    D -- "Option 3\nSystem Specs" --> SS[Display system specifications]
    SS --> C

    D -- "Option 4 Exit" --> Z([End Program])
```

---

## 5. Implementation Details

### 5.1 Key Data Structures

```cpp
struct InputNumber {
    string originalStr;                    // value as entered by user
    int    base;                           // 2, 8, 10, or 16
    double decimalValue;                   // intermediate decimal
    string binStr, octStr, decStr, hexStr; // converted forms
    bool   isValid;
};

struct ArithmeticResult {
    string operationName;                  // e.g. "Addition"
    string operationSymbol;               // e.g. "+"
    string expression;                    // e.g. "101010 [BIN] + 52 [OCT]"
    string decimalExpr;                   // e.g. "42 + 42 = 84"
    double decimalValue;
    string binStr, octStr, decStr, hexStr;
    bool   isValid;
    string errorMsg;
};

struct CustomExprResult {
    string originalExpr;    // as typed:          (a+b)*c
    string substitutedExpr; // values with labels: (101010[BIN]+52[OCT])*42[DEC]
    string decimalExpr;     // decimal form:       (42+42)*42 = 3528
    double decimalValue;
    string binStr, octStr, decStr, hexStr;
    bool   isValid;
    string errorMsg;
};

struct BCDDigitStep {
    int    digitA, digitB, carryIn, rawSum; // per-digit operand data
    string rawBin;                          // 4-bit binary of rawSum
    bool   needCorrect;                     // true if rawSum > 9
    int    corrected, carryOut, bcdDigitOut;
    string corrBin;                         // 4-bit binary of output digit
};

struct BCDSubDigitStep {
    int digitA, compDigit, carryIn, rawSum;
    bool needCorrect;
    int  corrected, carryOut, bcdDigitOut;
};

struct BCDAddResult {
    bool   isValid;  string errorMsg;
    string A_dec, B_dec, A_bcd, B_bcd;
    vector<BCDDigitStep> steps;
    int    finalCarry;
    string result_dec, result_bcd;
    string result_bin, result_oct, result_hex;
};

struct BCDSubResult {
    bool   isValid;  string errorMsg;
    bool   useTens;                     // false = 9's, true = 10's
    string A_dec, B_dec, A_bcd, B_bcd, comp_bcd;
    vector<BCDSubDigitStep> steps;
    bool   hasCarry, isNegative;
    string adjusted_bcd;
    double decimalResult;
    string result_dec, result_bcd;
    string result_bin, result_oct, result_hex;
};

struct BCDComplementResult {
    bool   applicable;  string note;
    string original_dec, original_bcd;
    // 9's complement
    string ninesComp_dec, ninesComp_bcd;
    string ninesComp_bin, ninesComp_oct, ninesComp_hex;
    // 10's complement
    string tensComp_dec, tensComp_bcd;
    string tensComp_bin, tensComp_oct, tensComp_hex;
};
```

### 5.2 Arithmetic Formulas

| Operation                  | Formula                                                                              |
| -------------------------- | ------------------------------------------------------------------------------------ |
| Addition                   | R = X1 + X2 + ... + XN                                                               |
| Subtraction                | R = X1 - X2 - ... - XN                                                               |
| Multiplication             | R = X1 x X2 x ... x XN                                                               |
| Division                   | R = X1 / X2 / ... / XN                                                               |
| 1's Complement             | `~X` (flip all bits), generalized as `(r^n - 1) - X`                                |
| 2's Complement             | `~X + 1` (1's complement plus 1)                                                     |
| Sub (1's comp)             | `A + ones_comp(B)`; add carry-out back (end-around)                                  |
| Sub (2's comp)             | `A + twos_comp(B)`; discard carry-out                                                |
| BCD Encoding               | Each decimal digit `d` → 4-bit group: `0`–`9` → `0000`–`1001`                       |
| 9's Complement (BCD)       | Each digit `d` → `9 − d`; i.e. diminished radix complement in decimal               |
| 10's Complement (BCD)      | 9's complement + 1 (with carry propagation); i.e. radix complement in decimal        |
| BCD Addition               | Digit-by-digit: `rawSum = dA + dB + carry`; if `rawSum > 9` add correction `+6`     |
| BCD Sub (9's comp)         | `A + nines_comp(B)`; apply end-around carry to LSDigit                               |
| BCD Sub (10's comp)        | `A + tens_comp(B)`; discard final carry                                              |

### 5.3 Operator Precedence Table

| Operator       | Symbol | Precedence | Associativity |
| -------------- | ------ | ---------- | ------------- |
| Multiplication | `*`    | 2 — High   | Left          |
| Division       | `/`    | 2 — High   | Left          |
| Addition       | `+`    | 1 — Low    | Left          |
| Subtraction    | `-`    | 1 — Low    | Left          |
| Parentheses    | `( )`  | Highest    | Overrides all |

### 5.4 Variable Mapping (Custom Expression Mode)

| Variable | Maps to            |
| -------- | ------------------ |
| `a`      | Input 1            |
| `b`      | Input 2            |
| `c`      | Input 3            |
| `d`      | Input 4            |
| ...      | ... up to Input 20 |

**Implicit multiplication** — automatically inserted between adjacent terms:

| You type     | System interprets as |
| ------------ | -------------------- |
| `a(b+c)`     | `a * (b+c)`          |
| `(a+b)c`     | `(a+b) * c`          |
| `(a+b)(c+d)` | `(a+b) * (c+d)`      |
| `ab`         | `a * b`              |

### 5.5 Error Handling Summary

| Error Condition                 | Detection Point         | Message Shown                                                              |
| ------------------------------- | ----------------------- | -------------------------------------------------------------------------- |
| Invalid char in number input    | Lexer (validateInput)   | "Invalid character 'X' for Base N"                                         |
| Multiple radix points           | Lexer (validateInput)   | "Multiple radix points not allowed"                                        |
| Division by zero (simple mode)  | processArithmetic       | "Division by zero is not allowed."                                         |
| Division by zero (custom mode)  | evalPostfix             | "Division by zero: divisor evaluates to 0."                                |
| Variable out of range           | tokenizeExpr            | "Variable 'X' out of range. Only N inputs defined."                        |
| Invalid character in expression | tokenizeExpr            | "Invalid character 'X' in expression."                                     |
| Unclosed parenthesis            | infixToPostfix          | "Mismatched parentheses: unclosed '(' detected."                           |
| Extra closing parenthesis       | infixToPostfix          | "Mismatched parentheses: extra ')' detected."                              |
| Malformed expression            | evalPostfix             | "Malformed expression: too many values left unevaluated."                  |
| Empty expression                | processCustomExpression | "Expression cannot be empty."                                              |
| Fractional input for BCD ops    | bcdAdd / bcdSubtract    | "BCD operation requires integer inputs (no radix fractions)."              |
| Negative input for BCD ops      | bcdAdd / bcdSubtract    | "BCD operation is defined for non-negative integers."                      |
| Fractional for BCD complement   | computeBCDComplements   | "Fractional numbers — BCD complement not applicable."                      |
