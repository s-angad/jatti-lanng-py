# 🚀 Jatti Language

**A Modern Programming Language Inspired by Punjabi**

```
     ██  █████  ████████ ████████ ██   ██ 
     ██ ██   ██    ██       ██    ██   ██
     ██ ███████    ██       ██    ██   ██
██   ██ ██   ██    ██       ██    ██  
 █████  ██   ██    ██       ██    ██   ██
```

**Version:** 0.4.0  
**Status:** Production Ready ✅  
**License:** MIT  
**Author:** Mr. Angad Singh

---

## 📋 Table of Contents

1. [About Jatti](#about-jatti)
2. [Features](#features)
3. [Quick Start](#quick-start)
4. [Syntax Reference](#syntax-reference)
5. [Data Types](#data-types)
6. [Keywords](#keywords)
7. [Operations](#operations)
8. [Control Flow](#control-flow)
9. [Functions](#functions)
10. [Collections](#collections)
11. [Built-in Functions](#built-in-functions)
12. [String Methods](#string-methods)
13. [Error Handling](#error-handling)
14. [Examples](#examples)
15. [Installation](#installation)
16. [VS Code Extension](#vs-code-extension)
17. [Documentation](#documentation)

---

## 🎯 About Jatti

Created by **Mr. Angad Singh** to make programming accessible and fun!

### Why Jatti?

✨ **Easy to Learn** - Clear, readable syntax  
✨ **Fun Syntax** - Punjabi keywords make coding enjoyable  
✨ **Powerful** - Full functional and procedural support  
✨ **Fast** - Compiled bytecode execution via C VM  

---

## ✨ Features

### Core Features
- ✅ **Interpreted Language** - Run code directly via C VM
- ✅ **Dynamic Typing** - Types determined at runtime
- ✅ **Functions** - First-class functions with recursion
- ✅ **Collections** - Lists and dictionaries
- ✅ **String Support** - Full string operations and methods
- ✅ **Exception Handling** - `chal_koshish_karle` / `pakad` blocks
- ✅ **File I/O** - Read and write files
- ✅ **Loops & Conditionals** - Full control flow

### Advanced Features
- ✅ **Recursion** - Full support for recursive functions
- ✅ **List Operations** - Append, indexing, iteration
- ✅ **String Methods** - `vada_likha() , vand_karo()`, etc.
- ✅ **Built-in Functions** - 20+ utility functions
- ✅ **Math Operations** - Complete arithmetic and comparison
- ✅ **Debugging** - Clear error messages with cultural roasts

### IDE Support
- ✅ **VS Code Extension** - Full editor integration
- ✅ **Syntax Highlighting** - Color-coded syntax
- ✅ **Run Button** - One-click execution
- ✅ **Output Panel** - Dedicated output pane
- ✅ **Error Messages** - Clear error reporting with roasts

---

## 🚀 Quick Start

### Install Jatti

```powershell
# From repository root, use:
jatti run yourfile.jatti
```

### Your First Program

**File: `hello.jatti`**

```jatti
sun_we
    chilla_we "Hello Jatti!"
ja_we
```

**Run it:**

```powershell
jatti run hello.jatti
```

**Output:**

```
Hello Jatti!
```

---

## 📝 Syntax Reference

### Program Structure

Every Jatti program must have this structure:

```jatti
sun_we
    fuddu_chiz Your code here
ja_we
```

**Translation:**
- `sun_we` = "Listen"  (Start main block)
- `ja_we` = "Go" (End main block)
- `fuddu_chiz` = Comment

### Comments

```jatti
fuddu_chiz This is a comment
fuddu_chiz Use fuddu_chiz to add notes
```

---

## 🔤 Data Types

### Strings
```jatti
chal_oye greeting ban "Hello"
chal_oye name ban "World"
chilla_we greeting
```

### Numbers (Integer & Float)
```jatti
chal_oye age ban 25
chal_oye height ban 5.9
chal_oye pi ban 3.14159
```

### Boolean
```jatti
chal_oye isTrue ban sach
chal_oye isFalse ban jhoot
```

### Lists
```jatti
chal_oye fruits ban ["apple", "banana", "mango"]
chal_oye numbers ban [1, 2, 3, 4, 5]
chal_oye mixed ban [1, "text", 3.14, sach]
```

### Dictionaries
```jatti
chal_oye person ban {
    "name": "Singh",
    "age": 25,
    "city": "Punjab"
}
```

### Khaali (None)
```jatti
chal_oye empty ban khaali
```

---

## 🔑 Keywords

| Keyword | Meaning | Usage |
|---------|---------|-------|
| `sun_we` | Listen | Start main block |
| `ja_we` | Go | End main block |
| `chal_oye` | Let's go | Variable assignment |
| `ban` | Is/Become | Assignment operator |
| `chilla_we` | Shout | Print output |
| `fuddu_chiz` | Nonsense/Comment | Add comment |
| `je` | If | Conditional |
| `nahin_taan_je` | if else if | if Else block |
| `nahin_taan` | Otherwise | Else block |
| `har_ek` | Every one | For loop |
| `jadon_tak` | While | While loop |
| `roko_oye_roko` | Stop/Halt | Exit loop |
| `chalo_oye_chalo` | Go/Continue | Skip iteration |
| `kaam` | Work/Function | Define function |
| `wapas_kar` | Return | Return value |
| `chal_koshish_karle` | Attempt/Try | Try block |
| `pakad` | Catch | Catch block |
| `vadha_hai` | Greater than | > |
| `nikka_hai` | Less than | < |
| `barabar` | Equal | == |
| `barabar_nahi_hai` | Not equal | != |
| `vadha_ya_barabar` | Greater or equal | >= |
| `nikka_ya_barabar` | Less or equal | <= |
| `sach` | Truth | Boolean true |
| `jhoot` | Lie | Boolean false |
| `khaali` | Empty | Null/None value |
| `ate` | And | AND operator |
| `ya_te` | Or | OR operator |
| `nahi` | Not | NOT operator |

---

## ➕ Operations

### Arithmetic

```jatti
sun_we
    chal_oye a ban 10
    chal_oye b ban 3
    
    chilla_we a + b          fuddu_chiz 13
    chilla_we a - b          fuddu_chiz 7
    chilla_we a * b          fuddu_chiz 30
    chilla_we a / b          fuddu_chiz 3.33
    chilla_we a % b          fuddu_chiz 1
ja_we
```

### Comparison

```jatti
sun_we
    chal_oye x ban 5
    
    chilla_we x vadha_hai 3           fuddu_chiz sach
    chilla_we x nikka_hai 10          fuddu_chiz sach
    chilla_we x barabar 5             fuddu_chiz sach
ja_we
```

### Logical

```jatti
sun_we
    chal_oye a ban sach
    chal_oye b ban jhoot
    
    je a ate b                   fuddu_chiz AND operator
        chilla_we "Both sach"
ja_we
```

---

## 🔀 Control Flow

### If-Else

```jatti
sun_we
    chal_oye age ban 18
    
    je age vadha_hai 18
        chilla_we "Adult"
    nahin_taan
        chilla_we "Minor"
ja_we
```

### For Loop

```jatti
sun_we
    har_ek i range_banao(1, 5)
        chilla_we i
ja_we
```

**Output:**
```
1
2
3
4
```

### While Loop

```jatti
sun_we
    chal_oye count ban 0
    
    jadon_tak count nikka_hai 5
        chilla_we count
        chal_oye count ban count + 1
ja_we
```

### roko_oye_roko & chalo_oye_chalo

```jatti
sun_we
    har_ek i range_banao(1, 10)
        je i barabar 5
            roko_oye_roko
        nahin_taan
            chilla_we i
ja_we
```

---

## 🔧 Functions

### Define Function

```jatti
sun_we
    kaam greet(name)
        chilla_we "Hello, " + name
        wapas_kar "Done"
    
    chal_oye result ban greet("Singh")
ja_we
```

### Return Value

```jatti
sun_we
    kaam add(x, y)
        wapas_kar x + y
    
    chal_oye result ban add(5, 3)
    chilla_we result
ja_we
```

### Parameters

```jatti
sun_we
    kaam product(a, b, c)
        wapas_kar a * b * c
    
    chilla_we product(2, 3, 4)
ja_we
```

---

## 📦 Collections

### Lists

```jatti
sun_we
    chal_oye fruits ban ["apple", "banana", "mango"]
    
    fuddu_chiz Access elements
    chilla_we fruits[0]         fuddu_chiz apple
    chilla_we fruits[1]         fuddu_chiz banana
    
    fuddu_chiz List length
    
    fuddu_chiz Loop through list
    har_ek fruit fruits
        chilla_we fruit
ja_we
```

### Dictionaries

```jatti
sun_we
    chal_oye person ban {"name": "Singh", "age": 25}
    
    fuddu_chiz Access values
    chilla_we person["name"]    fuddu_chiz Singh
    chilla_we person["age"]     fuddu_chiz 25
ja_we
```

---

## 🎁 Built-in Functions

| Function | Purpose | Example |
|----------|---------|---------|
| `range_banao(n)` | Create range | `range_banao(5)` → 0,1,2,3,4 |
| `kinna_lamba()` | Length | `kinna_lamba([1,2,3])` → 3 |
| `kism()` | Type | `kism(5)` → number |
| `likh()` | Write file | `likh("file.txt", "text")` |
| `padh()` | Read file | `padh("file.txt")` |
| `ganao()` | Sum | `ganao([1,2,3])` → 6 |
| `sab_ton_vaddha()` | Max | `sab_ton_vaddha([1,5,3])` → 5 |
| `sab_ton_chhota()` | Min | `sab_ton_chhota([1,5,3])` → 1 |
| `sorted()` | Sort | `sorted([3,1,2])` → [1,2,3] |
| `reversed()` | Reverse | `reversed([1,2,3])` → [3,2,1] |

---

## 🔤 String Methods

Jatti provides powerful string manipulation functions:

| Method | Purpose | Example |
|--------|---------|---------|
| `vada_likha(str)` | Uppercase | `vada_likha("hello")` → "HELLO" |
| `chhota_likha(str)` | Lowercase | `chhota_likha("HELLO")` → "hello" |
| `saf_karo(str)` | Trim spaces | `saf_karo("  text  ")` → "text" |
| `vand_karo(str, delim)` | Split string | `vand_karo("a,b,c", ",")` → ["a", "b", "c"] |
| `badal_de(str, old, new)` | Replace | `badal_de("hello", "l", "L")` → "heLLo" |
| `shuru_hunda(str, prefix)` | Check start | `shuru_hunda("hello", "he")` → sach |
| `khatam_hunda(str, suffix)` | Check end | `khatam_hunda("hello", "lo")` → sach |
| `dhundh_ja(str, substr)` | Find index | `dhundh_ja("hello", "ll")` → 2 |

### String Methods Examples

```jatti
sun_we
    chal_oye text ban "Jatti Programming"
    
    chilla_we vada_likha(text)          fuddu_chiz JATTI PROGRAMMING
    chilla_we chhota_likha(text)        fuddu_chiz jatti programming
    
    chal_oye words ban vand_karo(text, " ")
    chilla_we words                     fuddu_chiz ["Jatti", "Programming"]
    
    je shuru_hunda(text, "Jatti")
        chilla_we "Shuru hunda Jatti naal"
    
    chal_oye position ban dhundh_ja(text, "a")
    chilla_we position                 fuddu_chiz 1
    
    chal_oye replaced ban badal_de(text, "Jatti", "JATTI")
    chilla_we replaced                 fuddu_chiz JATTI Programming
    
    je khatam_hunda(text, "ing")
        chilla_we "Khatam hunda ing naal"
ja_we
```

---

## ❌ Error Handling

### Try-Catch Blocks

```jatti
sun_we
    chal_koshish_karle
        chal_oye result ban 10 / 0
    pakad err
        chilla_we "Error caught: " + err
ja_we
```

### Error Output With Roasts

Jatti shows a random Punjabi roast first, then the error:

```text
🔥 Roast: Syntax nu respect de.
❌ JATTI ERROR
🔴 Error: Program must start with sun_we.
📍 Line 1
```

### Available Roasts

- "Galti ho gayi !! koi gall nahi."
- "Dhyaan de, Jatti style rakhi !!"
- "Phir ohi mistake !!"
- "Tu compiler nu test kar reha !!"
- "Compiler thak gaya. Tu vi !!"
- "Tu coding chadd de !!"
- "Tere to nhi hona oye !!"
- "Eh ki likh ta tu?"
- "Dimag use kar le thoda."
- "Jatti Lang mazak nahi hai."
- "Syntax nu respect de."

---

## 💡 Examples

### Example 1: Calculator

```jatti
sun_we
    kaam calculate(a, op, b)
        je op barabar "+"
            wapas_kar a + b
        nahin_taan_je op barabar "-"
            wapas_kar a - b
        nahin_taan_je op barabar "*"
            wapas_kar a * b
        nahin_taan
            wapas_kar a / b
    
    chal_oye result1 ban calculate(10, "+", 5)
    chilla_we result1
ja_we
```

### Example 2: String Processing

```jatti
sun_we
    chal_oye sentence ban "Jatti is amazing"
    
    chal_oye upper_text ban vada_likha(sentence)
    chilla_we upper_text
    
    chal_oye words ban split(sentence, " ")
    chal_oye word_count ban kinna_lamba(words)
    chilla_we word_count
ja_we
```

### Example 3: File Operations

```jatti
sun_we
    likh("output.txt", "Hello from Jatti!")
    
    chal_oye content ban padh("output.txt")
    chilla_we content
ja_we
```

---

## 🎨 Installation

### Windows

```powershell
# Install Extension from vs code
Jatii extension

# Navigate to directory
cd jatti-lang

# Run a file
jatti run yourfile.jatti
```

---

## 🎨 VS Code Extension

### Features
- ✅ Syntax highlighting
- ✅ Run button (▶️)
- ✅ Keyboard shortcuts (Ctrl+Alt+R)
- ✅ Dedicated output panel
- ✅ Error reporting
- ✅ Marketplace icon

### Usage
1. Create `hello.jatti`
2. Click ▶️ button or press Ctrl+Alt+R
3. See output in **Jatti** output panel

---

## 📚 Documentation

See the full documentation set below. These links point to the main project docs from the extension folder:

- **[Language Specification]
# 📖 Jatti Language - Complete Specification

**Version:** 0.4.0  
**Last Updated:** January 2026  
**Status:** Production Ready

---

## Table of Contents

1. [Overview](#overview)
2. [Syntax Basics](#syntax-basics)
3. [Data Types](#data-types)
4. [Operators](#operators)
5. [Control Flow](#control-flow)
6. [Functions](#functions)
7. [Built-in Functions](#built-in-functions)
8. [String Methods](#string-methods)
9. [Collections](#collections)
10. [Error Handling](#error-handling)
11. [Advanced Features](#advanced-features)
12. [Standard Library](#standard-library)
13. [CLI Tools](#cli-tools)

---

## Overview

**Jatti** is a dynamically-typed programming language designed to be accessible to Punjabi speakers. It combines familiar programming concepts with Punjabi-influenced syntax.

### Key Features
- 🇮🇳 Punjabi-inspired keywords (intuitive and memorable)
- ⚡ Dynamic typing with automatic type coercion
- 🔄 First-class functions
- 📚 Rich collection types (dicts)
- 🛡️ Exception handling with chal_koshish_karle/catch
- 🎯 Simple, readable syntax

### Design Philosophy
- **Accessibility:** Non-programmers should understand the syntax
- **Expressiveness:** Powerful enough for real programs
- **Pragmatism:** Keep the core language small and readable

---

## Syntax Basics

### Comments
```jatti
# This is a comment
# Comments start with # and go to end of line
```

### Programs
Every Jatti program has this structure:
```jatti
sun_we
    # Your code here
ja_we
```

- `sun_we` = Program start
- `ja_we` = Program end
- Code inside must be indented with 4 spaces

### Example Program
```jatti
sun_we
    chilla_we "Hello, World!"
    chal_oye x ban 5
    chilla_we x
ja_we
```

---

## Data Types

### Primitives

#### Numbers (Integer & Float)
```jatti
chal_oye age ban 25          # Integer
chal_oye price ban 19.99     # Float
chal_oye negative ban -10    # Negative
```

#### Strings
```jatti
chal_oye name ban "Singh"
chal_oye greeting ban 'Hello'
chal_oye multiline ban "Line 1
Line 2
Line 3"
```

**Escape Sequences:**
```jatti
"\n"      # Newline
"\t"      # Tab
"\\"      # Backslash
"\""      # Double quote
"\'"      # Single quote
```

#### Booleans
```jatti
chal_oye is_active ban sach     # sach
chal_oye is_deleted ban khaali   # jhoot
```

### Collections

#### Lists (Arrays)
```jatti
chal_oye numbers ban [1, 2, 3, 4, 5]
chal_oye mixed ban [1, "two", 3.0, sach]
chal_oye empty ban []

# Access by index
chilla_we numbers[0]      # Output: 1
chilla_we numbers[-1]     # Output: 5 (last element)

```

#### Dictionaries
```jatti
chal_oye person ban {
    naam: "Singh",
    age: 25,
    city: "Punjab"
}

# Access by key
chilla_we person["naam"]         # Output: Singh
chilla_we person.get("country")  # Output: None (if key doesn't exist)
```

## Operators

### Arithmetic
```jatti
chal_oye sum ban 10 + 5        # 15
chal_oye diff ban 10 - 5       # 5
chal_oye product ban 10 * 5    # 50
chal_oye quotient ban 10 / 5   # 2
chal_oye fuddu_chiz remainder ban 10 % 3  # 1
chal_oye power ban 2 ** 3      # 8
```

### Comparison
```jatti
chal_oye a ban 5
chal_oye b ban 3

je a vadha_hai b              # a > b (greater than)
je a nikka_hai b              # a < b (less than)
je a barabar b                # a == b (equal)
je a barabar_nahi_hai b       # a != b (not equal)
je a vadha_ya_barabar b       # a >= b (greater than or equal)
je a nikka_ya_barabar b       # a <= b (less than or equal)
```

### Logical
```jatti
je (x vadha_hai 5) ya_te (y nikka_hai 3)        # OR
je (x vadha_hai 5) ate (y nikka_hai 3)        # AND
je nahi (x barabar 0)                     # NOT
```

### Assignment
```jatti
chal_oye x ban 10
chal_oye x ban x + 5          # x = 15
```

---

## Control Flow

### If/Elif/Else
```jatti
chal_oye score ban 85

je score vadha_ya_barabar 90
    chilla_we "A Grade"
nahin_taan_je score vadha_ya_barabar 80
    chilla_we "B Grade"
nahin_taan_je score vadha_ya_barabar 70
    chilla_we "C Grade"
nahin_taan
    chilla_we "F Grade"
```

**Keywords:**
- `je` = if
- `nahin_taan_je` = elif
- `nahin_taan` = else

### While Loop
```jatti
chal_oye count ban 0
jadon_tak count nikka_hai 5
    chilla_we count
    chal_oye count ban count + 1
```

**Keywords:**
- `jadon_tak` = while
- `roko_oye_roko` = exit loop
- `chalo_oye_chalo` = skip iteration

### For Loop
```jatti
har_ek number [1, 2, 3, 4, 5]
    chilla_we number

har_ek i range(10)
    chilla_we i

har_ek key, value person
    chilla_we key + value
```

**Keywords:**
- `har_ek` = for
- Works with lists, dicts, ranges, and any iterable

### roko_oye_roko & chalo_oye_chalo
```jatti
har_ek i range(10)
    je i barabar 5
        roko_oye_roko

har_ek i range(10)
    je i % 2 barabar 0
        chalo_oye_chalo
    chilla_we i
```

---

## Functions

### Definition
```jatti
kaam greet(name)
    chilla_we "Hello, " + name
ja_we

greet("Singh")
```

### With Return
```jatti
kaam add(a, b)
    chal_oye result ban a + b
    wapas_kar result
ja_we

chal_oye sum ban add(5, 3)
chilla_we sum      # Output: 8
```

### Default Parameters
```jatti
kaam greet(name, greeting="Hello")
    chilla_we greeting + ", " + name
ja_we

greet("Singh")           # Output: Hello, Singh
greet("Singh", "Hi")     # Output: Hi, Singh
```

### Multiple Return Values
```jatti
kaam get_coordinates()
    wapas_kar 10, 20
ja_we

chal_oye x, y ban get_coordinates()
```

### Recursion
```jatti
kaam factorial(n)
    je n barabar 1
        wapas_kar 1
    nahin_taan
        wapas_kar n * factorial(n - 1)
ja_we

chilla_we factorial(5)     # Output: 120
```

---

## Built-in Functions

### String Functions
```jatti
kinna_lamba("hello")              # 5 (length)
```

### List Functions
```jatti
chal_oye numbers ban [3, 1, 4, 1, 5]

kinna_lamba(numbers)              # 5 (length)
sort_hoja_oye(numbers)            # Sorts list
ulta_hoja_oye(numbers)            # Reverses list
jod_oye(numbers)                  # 14 (sum)
average_kad(numbers)              # 2.8 (average)
sabton_vaddha(numbers)            # 5 (max)
sabton_nikka(numbers)             # 1 (min)
```

### Dictionary Functions
```jatti
chal_oye person ban {naam: "Singh", age: 25}

kinna_lamba(person)               # 2 (number of keys)
person.get_keys()                 # ["naam", "age"]
person.get_values()               # ["Singh", 25]
```

### Type Conversion
```jatti
chal_oye num_str ban "42"
chal_oye num ban int(num_str)     # 42

chal_oye float_str ban "3.14"
chal_oye pi ban float(float_str)  # 3.14

chal_oye str_num ban str(42)      # "42"
```

### Range
```jatti
range(5)           # 0, 1, 2, 3, 4
range(1, 5)        # 1, 2, 3, 4
range(1, 10, 2)    # 1, 3, 5, 7, 9
```

---

## String Methods

```jatti
chal_oye text ban "Hello World"

text.upper_case_oye()             # "HELLO WORLD"
text.lower_case_oye()             # "hello world"
text.tut_ja_oye(" ")              # ["Hello", "World"]
text.badal_ja_oye("World", "Jatti") # "Hello Jatti"
text.haiga_hai("World")           # sach (sach)
text.shuru_hunda_hai("Hello")     # sach (sach)
text.khatam_hunda_hai("World")    # sach (sach)
text.trim_hoja_oye()              # "Hello World"
```

---

## Collections

### List Operations
```jatti
chal_oye nums ban [1, 2, 3]

# Add element
nums.append(4)                    # [1, 2, 3, 4]

# Insert at position
nums.insert(1, 99)                # [1, 99, 2, 3, 4]

# Check if exists
nums.contains(99)                 # sach

# Find index
nums.index_of(2)                  # 1 (0-based)

# fuddu_chizove element
nums.fuddu_chizove(99)                   # [1, 2, 3, 4]

# Get and fuddu_chizove last
chal_oye last ban nums.pop()      # 4, nums = [1, 2, 3]
```

### List Comprehension
```jatti
# Simple
chal_oye squares ban [x * x har_ek x [1, 2, 3, 4, 5]]
# Result: [1, 4, 9, 16, 25]

# With condition
chal_oye evens ban [x har_ek x [1, 2, 3, 4, 5] je x % 2 barabar 0]
# Result: [2, 4]

# With expression
chal_oye doubled ban [x * 2 har_ek x range(1, 6)]
# Result: [2, 4, 6, 8, 10]
```

### Dictionary Comprehension
```jatti
chal_oye mapping ban {x: x*2 har_ek x range(1, 5)}
# Result: {1: 2, 2: 4, 3: 6, 4: 8}
```

---

## Error Handling

### chal_koshish_karle/Catch
```jatti
chal_koshish_karle
    chal_oye result ban 10 / 0
pakad error
    chilla_we "Error: " + error
ja_we
```

### Raise Exception
```jatti
kaam validate_age(age)
    je age nikka_hai 0
        throw "Age cannot be negative"
    nahin_taan
        chilla_we "Age is valid"
ja_we
```

### Error Types
```jatti
chal_koshish_karle
    chal_oye value ban numbers[100]  # IndexError
pakad error
    chilla_we error
ja_we

chal_koshish_karle
    chal_oye x ban undefined_var     # NameError
pakad error
    chilla_we error
ja_we

chal_koshish_karle
    chal_oye result ban "text" / 5   # TypeError
pakad error
    chilla_we error
ja_we
```

---
---

## CLI Tools

### jatti run
Execute a Jatti program:
```bash
jatti run program.jatti
```

---

## Keywords Reference

| Jatti | English | Purpose |
|-------|---------|---------|
| `sun_we` | program start | Mark program beginning |
| `ja_we` | program end | Mark program ending |
| `chal_oye` | let/var | Variable assignment |
| `ban` | equals/assign | Assignment operator |
| `chilla_we` | print | Output to console |
| `kaam` | function | Define function |
| `wapas_kar` | return | Return from function |
| `je` | if | Conditional statement |
| `nahin_taan_je` | elif | Else if |
| `nahin_taan` | else | Else clause |
| `jadon_tak` | while | While loop |
| `har_ek` | for | For loop |
| `roko_oye_roko` | roko_oye_roko | Exit loop |
| `chalo_oye_chalo` | chalo_oye_chalo | Skip iteration |
| `chal_koshish_karle` | chal_koshish_karle | Start exception block |
| `pakad` | catch | Catch exception |
| `throw` | raise/throw | Raise exception |
| `global` | global | Access global variable |
| `sach` | sach | Boolean sach |
| `khaali` | jhoot/empty | Boolean jhoot |

---

## Operators Reference

| Operator | Name | Example |
|----------|------|---------|
| `+` | Addition | `5 + 3` → 8 |
| `-` | Subtraction | `5 - 3` → 2 |
| `*` | Multiplication | `5 * 3` → 15 |
| `/` | Division | `10 / 2` → 5 |
| `%` | Modulo | `10 % 3` → 1 |
| `**` | Power | `2 ** 3` → 8 |
| `=` | Assignment | `x = 5` |
| `==` | Equal | `5 == 5` → sach |
| `!=` | Not equal | `5 != 3` → sach |
| `<` | Less than | `3 < 5` → sach |
| `>` | Greater than | `5 > 3` → sach |
| `<=` | Less or equal | `5 <= 5` → sach |
| `>=` | Greater or equal | `5 >= 3` → sach |
| `and` / `ate` | Logical AND | `sach ate sach` → sach |
| `or` / `ya_te` | Logical OR | `sach ya_te khaali` → sach |
| `not` / `nahi` | Logical NOT | `nahi sach` → khaali |

---

## Error Messages

When something goes wrong, you'll see helpful error messages:

```
SyntaxError: Unknown keyword 'chhala' on line 5
Did you mean 'chal_oye'?

NameError: Variable 'username' is not defined on line 12
Available variables: name, age, city

TypeError: Cannot / string by number
Line 8: chal_oye result ban "hello" / 5
                                    ^ error here

IndexError: List index 10 out of range (list size: 5)
Line 15: chilla_we numbers[10]
                      ^ error here

ZeroDivisionError: Cannot / by zero
Line 7: chal_oye result ban 10 / 0
                                ^ error here
```

---

## Version History

### v0.4.0 (Current)
- ✅ Complete CLI tooling (run, build, format)
- ✅ Enhanced error handling and debugging
- ✅ All Phase 3 advanced features
- ✅ Windows/Linux/Mac compatibility

### v0.3.0
- ✅ Advanced operators (%, **)
- ✅ Dictionary/Range support
- ✅ List/Dict comprehensions
- ✅ Global keyword

### v0.2.0
- ✅ Standard library functions
- ✅ String methods
- ✅ List operations
- ✅ Error handling (chal_koshish_karle/catch)

### v0.1.0
- ✅ Core syntax and keywords
- ✅ Variables and functions
- ✅ Control flow (if/while/for)
- ✅ Basic operators

---

## Summary

Jatti is a complete, production-ready programming language with:

✅ Simple, readable Punjabi-inspired syntax  
✅ Dynamic typing with type coercion  
✅ Comprehensive built-in functions  
✅ Exception handling and debugging  
✅ Professional CLI tooling  
✅ Detailed error messages  

**Ready to build real programs!** 🚀

- **[Beginner Tutorial]- Start here!
# 🎓 Jatti Language - Beginner Tutorial

Welcome to Jatti! This tutorial will teach you the basics of the language step by step.

---

## Part 1: Hello World

Every Jatti program starts with `sun_we` and ends with `ja_we`.

Create a file called `hello.jatti`:

```jatti
sun_we
    chilla_we "Hello, World!"
ja_we
```

Run it:
```bash
jatti run hello.jatti
```

**Output:**
```
Hello, World!
```

**Explanation:**
- `sun_we` = "Listen up" (program start)
- `chilla_we` = "shout/print" (output)
- `ja_we` = "that's all" (program end)
- Code inside must be indented (4 spaces)

---

## Part 2: Variables

Variables store values. Use `chal_oye` to create them:

```jatti
sun_we
    chal_oye name ban "Singh"
    chal_oye age ban 25
    chal_oye height ban 5.9
    
    chilla_we name
    chilla_we age
    chilla_we height
ja_we
```

**Output:**
```
Singh
25
5.9
```

**Explanation:**
- `chal_oye` = "Let's have" (variable declaration)
- `ban` = "is" or "equals" (assignment)
- `chilla_we` = prints the value

### Changing Variables

```jatti
sun_we
    chal_oye x ban 10
    chilla_we x         # Output: 10
    
    chal_oye x ban 20
    chilla_we x         # Output: 20
ja_we
```

### Comments

```jatti
sun_we
    # This is a comment
    chal_oye name ban "Singh"  # Comments can go here too
    chilla_we name
ja_we
```

---

## Part 3: Basic Math

```jatti
sun_we
    chal_oye a ban 10
    chal_oye b ban 5
    
    chilla_we a + b     # Output: 15 (addition)
    chilla_we a - b     # Output: 5 (subtraction)
    chilla_we a * b     # Output: 50 (multiplication)
    chilla_we a / b     # Output: 2 (division)
ja_we
```

**Output:**
```
15
5
50
2
```

### More Math

```jatti
sun_we
    chilla_we 10 % 3    # Output: 1 (fuddu_chiz remainder)
    chilla_we 2 ** 3    # Output: 8 (power)
ja_we
```

---

## Part 4: If Statements (Conditionals)

Make decisions based on conditions:

```jatti
sun_we
    chal_oye age ban 16
    
    je age vadha_hai 18
        chilla_we "You can vote!"
    nahin_taan
        chilla_we "Too young to vote"
ja_we
```

**Output:**
```
Too young to vote
```

**Explanation:**
- `je` = "if"
- `vadha_hai` = "is greater than" (>)
- `nahin_taan` = "else"

### Comparison Operators

```jatti
sun_we
    chal_oye score ban 85
    
    # > greater than
    je score vadha_hai 90
        chilla_we "Great!"
    
    # < less than
    je score nikka_hai 50
        chilla_we "Need to improve"
    
    # == equal
    je score barabar 85
        chilla_we "Perfect score!"
ja_we
```

### Multiple Conditions

```jatti
sun_we
    chal_oye age ban 25
    chal_oye has_license ban sach
    
    # AND condition
    je (age vadha_ya_barabar 18) ate (has_license barabar sach)
        chilla_we "You can drive!"
ja_we
```

**Explanation:**
- `ate` = "and"
- `ya_te` = "or"
- `nahi` = "not"

---

## Part 5: Loops - While

Repeat code until a condition is jhoot:

```jatti
sun_we
    chal_oye count ban 1
    
    jadon_tak count nikka_ya_barabar 5
        chilla_we count
        chal_oye count ban count + 1
ja_we
```

**Output:**
```
1
2
3
4
5
```

**Explanation:**
- `jadon_tak` = "while"
- `nikka_ya_barabar` = "<=" (less than or equal)

### roko_oye_roko & chalo_oye_chalo

```jatti
sun_we
    chal_oye i ban 0
    
    jadon_tak i nikka_hai 10
        je i barabar 5
            roko_oye_roko          # Exit the loop
        chilla_we i
        chal_oye i ban i + 1
ja_we
```

**Output:**
```
0
1
2
3
4
```

---

## Part 6: Loops - For

Iterate over collections:

```jatti
sun_we
    chal_oye fruits ban ["Apple", "Banana", "Cherry"]
    
    har_ek fruit fruits
        chilla_we fruit
ja_we
```

**Output:**
```
Apple
Banana
Cherry
```

**Explanation:**
- `har_ek` = "for each"
- Works with lists, dictionaries, ranges

### Using range()

```jatti
sun_we
    har_ek i range(1, 6)
        chilla_we i
ja_we
```

**Output:**
```
1
2
3
4
5
```

---

## Part 7: Lists

Store multiple values:

```jatti
sun_we
    chal_oye numbers ban [10, 20, 30, 40, 50]
    
    # Access by index
    chilla_we numbers[0]   # Output: 10
    chilla_we numbers[2]   # Output: 30
    chilla_we numbers[-1]  # Output: 50 (last element)
ja_we
```

**Index:**
```
     [10, 20, 30, 40, 50]
      0   1   2   3   4    (forward)
     -5  -4  -3  -2  -1    (backward)
```

### Modifying Lists

```jatti
sun_we
    chal_oye nums ban [1, 2, 3]
    
    nums.append(4)         # Add 4 → [1, 2, 3, 4]
    nums.insert(1, 99)     # Insert 99 at index 1 → [1, 99, 2, 3, 4]
    nums.fuddu_chizove(99)        # fuddu_chizove 99 → [1, 2, 3, 4]
    
    chilla_we nums
ja_we
```

### List Operations

```jatti
sun_we
    chal_oye items ban ["a", "b", "c"]
    
    chilla_we kinna_lamba(items)      # Output: 3 (length)
    chilla_we items.contains("b")     # Output: sach (sach)
ja_we
```

---

## Part 8: Strings

Text values:

```jatti
sun_we
    chal_oye message ban "Hello, Jatti!"
    
    chilla_we message
    chilla_we kinna_lamba(message)    # Output: 14 (length)
ja_we
```

### String Methods

```jatti
sun_we
    chal_oye text ban "Hello World"
    
    chilla_we text.vada_likha()        # HELLO WORLD
    chilla_we text.lower_case_oye()        # hello world
    chilla_we text.tut_ja_oye(" ")         # ["Hello", "World"]
    chilla_we text.badal_ja_oye("World", "Jatti")  # Hello Jatti
ja_we
```

### String Concatenation

```jatti
sun_we
    chal_oye first_name ban "Singh"
    chal_oye last_name ban "Kumar"
    
    chilla_we first_name + " " + last_name  # Output: Singh Kumar
ja_we
```

---

## Part 9: Functions

Reusable blocks of code:

```jatti
kaam greet(name)
    chilla_we "Hello, " + name
ja_we

sun_we
    greet("Singh")
    greet("Priya")
    greet("Raj")
ja_we
```

**Output:**
```
Hello, Singh
Hello, Priya
Hello, Raj
```

**Explanation:**
- `kaam` = "function"
- Functions must be defined before `sun_we`
- Parameters are in parentheses

### Functions with Return

```jatti
kaam add(a, b)
    chal_oye result ban a + b
    wapas_kar result
ja_we

sun_we
    chal_oye sum ban add(5, 3)
    chilla_we sum       # Output: 8
ja_we
```

**Explanation:**
- `wapas_kar` = "return"

### Default Parameters

```jatti
kaam greet(name, greeting="Hi")
    chilla_we greeting + ", " + name
ja_we

sun_we
    greet("Singh")              # Output: Hi, Singh
    greet("Singh", "Hello")     # Output: Hello, Singh
ja_we
```

---

## Part 10: Error Handling

Handle problems gracefully:

```jatti
sun_we
    chal_koshish_karle
        chal_oye numbers ban [1, 2, 3]
        chilla_we numbers[10]   # Error! Index out of range
    pakad error
        chilla_we "Error: " + error
    ja_we
ja_we
```

**Output:**
```
Error: list index out of range
```

**Explanation:**
- `chal_koshish_karle` = start protected code block
- `pakad` = "catch" exception
- `ja_we` = end error handling block

### Common Errors

```jatti
sun_we
    # Division by zero
    chal_koshish_karle
        chal_oye x ban 10 / 0
    pakad error
        chilla_we "Cannot / by zero"
    ja_we
    
    # Undefined variable
    chal_koshish_karle
        chilla_we undefined_var
    pakad error
        chilla_we "Variable not found"
    ja_we
ja_we
```

---

## Part 11: Dictionaries

Store data with labels:

```jatti
sun_we
    chal_oye person ban {
        naam: "Singh",
        age: 25,
        city: "Punjab"
    }
    
    chilla_we person["naam"]      # Output: Singh
    chilla_we person["age"]       # Output: 25
ja_we
```

### Dictionary Operations

```jatti
sun_we
    chal_oye student ban {
        naam: "Priya",
        grade: "A"
    }
    
    chilla_we kinna_lamba(student)    # Output: 2
    chilla_we student.get_keys()      # Output: ["naam", "grade"]
    chilla_we student.get_values()    # Output: ["Priya", "A"]
ja_we
```

---

## Part 12: Complete Example Program

Here's a simple program using everything you learned:

```jatti
kaam calculate_grade(score)
    je score vadha_ya_barabar 90
        wapas_kar "A"
    nahin_taan_je score vadha_ya_barabar 80
        wapas_kar "B"
    nahin_taan_je score vadha_ya_barabar 70
        wapas_kar "C"
    nahin_taan
        wapas_kar "F"
ja_we

sun_we
    chal_oye students ban {
        singh: 85,
        priya: 92,
        raj: 78,
        neha: 95
    }
    
    har_ek name, score students
        chal_oye grade ban calculate_grade(score)
        chilla_we name + ": " + score + " (" + grade + ")"
ja_we
```

**Output:**
```
singh: 85 (B)
priya: 92 (A)
raj: 78 (C)
neha: 95 (A)
```

---

## Part 13: What's Next?

You now know the basics! chalo_oye_chalo with:

1. **Intermediate Guide** - Learn about comprehensions, and advanced features
2. **Advanced Topics** - Explore performance tips, best practices, and debugging
3. **Example Programs** - See real-world applications
4. **API Reference** - Complete documentation of all functions and methods

---

## Cheat Sheet

```jatti
# Variables
chal_oye x ban 10

# Output
chilla_we x

# Math
+ - * / % **

# Comparison
vadha_hai (>) nikka_hai (<) barabar (==) 
barabar_nahi_hai (!=) vadha_ya_barabar (>=) nikka_ya_barabar (<=)

# Logical
ate (AND) ya_te (OR) nahi (NOT)

# Control Flow
je (if) nahin_taan_je (elif) nahin_taan (else)
jadon_tak (while) har_ek (for) roko_oye_roko chalo_oye_chalo

# Functions
kaam name(params) ... wapas_kar value ja_we

# Error Handling
chal_koshish_karle ... pakad error ... ja_we

# Collections
[list] {dict}
```

---

## Tips for Learning

1. **Type out the code** - Don't just copy/paste
2. **Experiment** - Change numbers and strings, see what happens
3. **roko_oye_roko things** - Errors are how you learn
4. **Start small** - Combine simple things into complex programs
5. **Have fun** - Programming should be enjoyable!

---

**Happy Coding! 🎉**

Questions? See the Troubleshooting Guide.

- **[Local Setup]
# Local Setup (Windows)

This repo contains:
- The Jatti C compiler + bytecode VM (`c/`)
- A VS Code extension (publishable scaffold in `jatti-vscode-extension-c/`)

## Requirements

- Windows 10/11
- Git (optional, but recommended)
- Visual Studio 2022 Build Tools (MSVC)

## 1) Get the code

```powershell
git clone https://github.com/s-angad/jattigit
cd Punjabi-Language-Jatti-
```

If you already have the folder, just `cd` into it.

## 2) Create and activate a virtual environment

No setup is required for the C VM.

## 3) Run a Jatti file (CLI)

Create `hello.jatti`:

```jatti
sun_we
    chilla_we "Hello Jatti!"
ja_we
```

Run:

```powershell
   jatti run hello.jatti
```

- **[Language Basics] - Variables, operators, and loops
# Language Basics (Variables, Operators, Control Flow)

This is a quick reference for everyday Jatti syntax.

## Program structure

Every program starts with `sun_we` and ends with `ja_we`.

```jatti
sun_we
    chilla_we "hello"
ja_we
```

## Comments

```jatti
fuddu_chiz this is a comment
```

## Variables

Assignment uses `chal_oye <name> ban <value>`.

```jatti
sun_we
    chal_oye x ban 5
    chal_oye name ban "Jatti"
    chal_oye ok ban sach
    chal_oye none ban khaali
    chilla_we x
    chilla_we name
ja_we
```

## Numbers and strings

```jatti
sun_we
    chal_oye a ban 10
    chal_oye b ban 3
    chilla_we a + b
    chilla_we a * b
    chilla_we a / b
ja_we
```

## Comparison operators (keywords)

Use these in conditions and prints:

- `vadha_hai`  → `>`
- `nikka_hai` → `<`
- `barabar` → `==`
- `barabar_nahi_hai` → `!=`
- `vadha_ya_barabar` → `>=`
- `nikka_ya_barabar` → `<=`

Example:

```jatti
sun_we
    chal_oye x ban 5
    je x vadha_hai 3
        chilla_we "big"
    nahin_taan
        chilla_we "small"
ja_we
```

## If / else-if / else

```jatti
sun_we
    chal_oye n ban 10

    je n vadha_hai 10
        chilla_we "gt 10"
    nahin_taan_je n barabar 10
        chilla_we "eq 10"
    nahin_taan
        chilla_we "lt 10"
ja_we
```

## Loops

### While (`jadon_tak`)

```jatti
sun_we
    chal_oye i ban 1
    chal_oye total ban 0

    jadon_tak i nikka_ya_barabar 5
        chal_oye total ban total + i
        chal_oye i ban i + 1

    chilla_we total
ja_we
```

### For-each (`har_ek`)

```jatti
sun_we
    chal_oye total ban 0
    har_ek i range_banao(1, 6)
        chal_oye total ban total + i
    chilla_we total
ja_we
```

## Try / catch (`chal_koshish_karle` / `pakad`)

```jatti
sun_we
    chal_koshish_karle
        chal_oye x ban 10 / 0
    pakad err
        chilla_we err
ja_we
```

## Next reads

- Full spec: `LANGUAGE_SPECIFICATION.md`
- Beginner tutorial: `BEGINNER_TUTORIAL.md`

- **[String Methods Reference] - Punjabi string method guide
# 🔤 Jatti String Methods - Punjabi Reference

All string methods in Jatti now use Punjabi names!

## String Methods Summary

| Punjabi Name | English Translation | Purpose | Syntax |
|--------------|-------------------|---------|--------|
| `vada_likha` | Big Letters | Convert string to UPPERCASE | `vada_likha(str)` |
| `chhota_likha` | Small Letters | Convert string to lowercase | `chhota_likha(str)` |
| `saf_karo` | Clean/Trim | Remove leading/trailing whitespace | `saf_karo(str)` |
| `vand_karo` | Split/Divide | Split string by delimiter into list | `vand_karo(str, delimiter)` |
| `badal_de` | Replace/Change | Replace all occurrences of substring | `badal_de(str, old, new)` |
| `shuru_hunda` | Starts With | Check if string starts with prefix | `shuru_hunda(str, prefix)` |
| `khatam_hunda` | Ends With | Check if string ends with suffix | `khatam_hunda(str, suffix)` |
| `dhundh_ja` | Find/Search | Find index of substring (returns -1 if not found) | `dhundh_ja(str, substring)` |

---

## 📝 Usage Examples

### Example 1: Text Transformation

```jatti
sun_we
    chal_oye naam ban "Singh"
    
    fuddu_chiz Uppercase karo
    chilla_we vada_likha(naam)     fuddu_chiz SINGH
    
    fuddu_chiz Lowercase karo
    chilla_we chhota_likha(naam)   fuddu_chiz singh
ja_we
```

### Example 2: String Splitting

```jatti
sun_we
    chal_oye sentence ban "Jatti,is,awesome"
    
    fuddu_chiz Split by comma
    chal_oye words ban vand_karo(sentence, ",")
    chilla_we words                fuddu_chiz ["Jatti", "is", "awesome"]
ja_we
```

### Example 3: String Replacement

```jatti
sun_we
    chal_oye text ban "hello world"
    
    fuddu_chiz Replace "world" with "Jatti"
    chal_oye result ban badal_de(text, "world", "Jatti")
    chilla_we result               fuddu_chiz hello Jatti
ja_we
```

### Example 4: String Checking

```jatti
sun_we
    chal_oye filename ban "document.txt"
    
    fuddu_chiz Check if it's a text file
    je khatam_hunda(filename, ".txt")
        chilla_we "Yes, it's a text file!"
    
    fuddu_chiz Check if it starts with "document"
    je shuru_hunda(filename, "document")
        chilla_we "Yes, starts with document!"
ja_we
```

### Example 5: Finding Substrings

```jatti
sun_we
    chal_oye text ban "Jatti Language"
    
    fuddu_chiz Find position of "Lang"
    chal_oye pos ban dhundh_ja(text, "Lang")
    chilla_we pos                  fuddu_chiz 7
    
    fuddu_chiz Find non-existent substring
    chal_oye pos2 ban dhundh_ja(text, "xyz")
    chilla_we pos2                 fuddu_chiz -1
ja_we
```

### Example 6: Trimming Whitespace

```jatti
sun_we
    chal_oye text ban "   Jatti Language   "
    
    fuddu_chiz Remove leading and trailing spaces
    chal_oye cleaned ban saf_karo(text)
    chilla_we cleaned              fuddu_chiz "Jatti Language"
ja_we
```

### Example 7: Complex Text Processing

```jatti
sun_we
    chal_oye data ban "NAME:Angad,AGE:25,CITY:Punjab"
    
    fuddu_chiz Split by comma first
    chal_oye pairs ban vand_karo(data, ",")
    
    fuddu_chiz Process each pair
    har_ek pair pairs
        chilla_we vada_likha(pair)  fuddu_chiz Convert to uppercase
ja_we
```

---

## Why Punjabi Names?

The string methods use Punjabi names to:
- Make the language more accessible to Punjabi speakers
- Maintain cultural consistency
- Make code more readable for Punjabi audiences
- Honor Punjabi linguistic traditions

---

## API Reference

### `vada_likha(str)` - Uppercase
- **Input:** String
- **Output:** Uppercase version of string
- **Example:** `vada_likha("hello")` → `"HELLO"`

### `chhota_likha(str)` - Lowercase
- **Input:** String
- **Output:** Lowercase version of string
- **Example:** `chhota_likha("HELLO")` → `"hello"`

### `saf_karo(str)` - Trim
- **Input:** String
- **Output:** String with leading/trailing whitespace removed
- **Example:** `saf_karo("  text  ")` → `"text"`

### `vand_karo(str, delimiter)` - Split
- **Input:** String, delimiter string
- **Output:** List of strings split by delimiter
- **Example:** `vand_karo("a,b,c", ",")` → `["a", "b", "c"]`
- **Note:** Delimiter cannot be empty

### `badal_de(str, old, new)` - Replace
- **Input:** String, old substring, new substring
- **Output:** String with all occurrences of old replaced with new
- **Example:** `badal_de("hello", "l", "L")` → `"heLLo"`

### `shuru_hunda(str, prefix)` - Starts With
- **Input:** String, prefix to check
- **Output:** Boolean (sach/jhoot)
- **Example:** `shuru_hunda("hello", "he")` → `sach`

### `khatam_hunda(str, suffix)` - Ends With
- **Input:** String, suffix to check
- **Output:** Boolean (sach/jhoot)
- **Example:** `khatam_hunda("hello", "lo")` → `sach`

### `dhundh_ja(str, substring)` - Find
- **Input:** String, substring to find
- **Output:** Number (index of substring, or -1 if not found)
- **Example:** `dhundh_ja("hello", "ll")` → `2`

---

**Happy coding with Jatti! 🚀**

---
---

## 📊 Language Stats

- **Version:** 0.4.0
- **Lines of Code:** 3500+
- **Keywords:** 25+
- **Built-in Functions:** 18+ (including 8 Punjabi string methods)
- **Examples:** 60+
- **Documentation:** 1700+ lines

---

## 🤝 Contributing

Contributions welcome! Please:

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

---

## 📄 License

MIT License 

---

## 👤 Author

**Mr. Singh / Mr. Angad Singh**

- Created: January 2025
- Language: Jatti
- Inspiration: Punjabi culture, Python simplicity

---

## 🙏 Acknowledgments

Thanks to:
- Punjabi language speakers
- Python community for inspiration
- All contributors and users

---

## 📞 Support

- 📧 Email: jatti-lang@example.com
- 🐛 Issues: GitHub Issues
- 💬 Discussions: GitHub Discussions

---

## 📜 Version History

### v0.4.0 (Current - Production Ready) ✅
- ✅ Full recursive function support
- ✅ String methods API with Punjabi names (vada_likha, chhota_likha, vand_karo, badal_de, saf_karo, shuru_hunda, khatam_hunda, dhundh_ja)
- ✅ Safe string handling
- ✅ All comparison operators
- ✅ Punjabi roast error messages with exception types
- ✅ VS Code output panel integration
- ✅ Try-catch exception handling
- ✅ Comprehensive test coverage

### v0.3.0
- Basic language features
- String operations
- Collections (lists, dicts)
- Control flow (if/else, loops)

---

**Happy Coding! 🎉 - Jatti Lang Team**
