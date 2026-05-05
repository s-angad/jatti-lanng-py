# 🎯 Jatti Language - Intermediate Guide

Welcome to intermediate Jatti! This guide covers advanced techniques and patterns.

---

## Part 1: Working with Collections

### List Iteration Patterns

```jatti
sun_we
    chal_oye nums ban [10, 20, 30, 40, 50]
    
    fuddu_chiz Basic iteration
    har_ek num nums
        chilla_we num
    
    fuddu_chiz Filter pattern using conditionals
    har_ek num nums
        je num vadha_hai 25
            chilla_we num
ja_we
```

### Dictionary Iteration

```jatti
sun_we
    chal_oye person ban {
        "naam": "Singh",
        "age": 25,
        "city": "Punjab"
    }
    
    har_ek key person
        chilla_we key + ": " + person[key]
ja_we
```

---

## Part 2: Functions - Advanced Patterns

### Recursive Functions

```jatti
kaam factorial(n)
    je n nikka_ya_barabar 1
        wapas_kar 1
    nahin_taan
        wapas_kar n * factorial(n - 1)
ja_we

sun_we
    chilla_we factorial(5)    fuddu_chiz Output: 120
ja_we
```

### Function as Data

Functions are first-class values:

```jatti
kaam apply_operation(a, b, operation)
    wapas_kar operation(a, b)
ja_we

kaam add_numbers(x, y)
    wapas_kar x + y
ja_we

sun_we
    chal_oye result ban apply_operation(5, 3, add_numbers)
    chilla_we result    fuddu_chiz Output: 8
ja_we
```

---

## Part 3: String Manipulation

### Building Strings

```jatti
sun_we
    chal_oye words ban ["Jatti", "is", "awesome"]
    
    fuddu_chiz Join words with space
    chal_oye message ban words[0] + " " + words[1] + " " + words[2]
    chilla_we message
ja_we
```

### Advanced String Operations

```jatti
sun_we
    chal_oye text ban "  Hello World  "
    
    fuddu_chiz Trim and transform
    chal_oye cleaned ban saf_karo(text)
    chal_oye upper ban vada_likha(cleaned)
    chilla_we upper
    
    fuddu_chiz Split and search
    chal_oye words ban vand_karo(cleaned, " ")
    je dhundh_ja(cleaned, "World") barabar_nahi_hai -1
        chilla_we "Found 'World' in text"
ja_we
```

---

## Part 4: Error Handling Patterns

### Multiple Error Catching

```jatti
sun_we
    chal_koshish_karle
        chal_oye x ban 10
        chal_oye y ban 0
        chal_oye result ban x / y    fuddu_chiz May cause error
    pakad error
        chilla_we "Error occurred: " + error
    ja_we
    
    chilla_we "Program continues after error"
ja_we
```

### Protected Operations

```jatti
kaam safe_divide(a, b)
    chal_koshish_karle
        wapas_kar a / b
    pakad err
        chilla_we "Division error: " + err
        wapas_kar 0
    ja_we
ja_we

sun_we
    chilla_we safe_divide(10, 2)
    chilla_we safe_divide(10, 0)
ja_we
```

---

## Part 5: Mathematical Operations

### Power Operator

```jatti
sun_we
    chilla_we 2 ** 8      fuddu_chiz 256
    chilla_we 10 ** 2     fuddu_chiz 100
    chilla_we 3 ** 3      fuddu_chiz 27
ja_we
```

### Complex Calculations

```jatti
kaam fibonacci(n)
    je n nikka_ya_barabar 1
        wapas_kar n
    nahin_taan
        wapas_kar fibonacci(n - 1) + fibonacci(n - 2)
ja_we

sun_we
    har_ek i range_banao(1, 11)
        chilla_we fibonacci(i)
ja_we
```

---

## Part 6: Sorting and Filtering

### Sorting Lists

```jatti
sun_we
    chal_oye numbers ban [5, 2, 8, 1, 9, 3]
    chal_oye sorted_nums ban chal_sort_hoja(numbers)
    chilla_we sorted_nums    fuddu_chiz [1, 2, 3, 5, 8, 9]
    
    chal_oye words ban ["zebra", "apple", "mango", "banana"]
    chal_oye sorted_words ban chal_sort_hoja(words)
    chilla_we sorted_words
ja_we
```

### Reversing Lists

```jatti
sun_we
    chal_oye items ban [1, 2, 3, 4, 5]
    chal_oye reversed ban chal_reverse_hoja(items)
    chilla_we reversed    fuddu_chiz [5, 4, 3, 2, 1]
ja_we
```

---

## Part 7: List Aggregation

### Sum and Statistics

```jatti
sun_we
    chal_oye scores ban [85, 90, 78, 92, 88]
    
    chal_oye total ban ganao(scores)
    chilla_we "Total: " + kism(total)
    
    chal_oye highest ban sab_ton_vaddha(scores)
    chal_oye lowest ban sab_ton_chhota(scores)
    
    chilla_we "Highest: " + kism(highest)
    chilla_we "Lowest: " + kism(lowest)
ja_we
```

---

## Part 8: File Operations

### Reading and Processing Files

```jatti
sun_we
    chal_oye content ban padh("data.txt")
    je content barabar_nahi_hai khaali
        chal_oye lines ban vand_karo(content, "\n")
        chilla_we "Read " + kism(kinna_lamba(lines)) + " lines"
    nahin_taan
        chilla_we "File not found"
    ja_we
ja_we
```

### Writing Results

```jatti
sun_we
    chal_oye report ban "Jatti Program Report\n"
    chal_oye report ban report + "Date: 2026\n"
    chal_oye report ban report + "Status: Success\n"
    
    chal_oye success ban likh("report.txt", report)
    je success
        chilla_we "Report written successfully"
    nahin_taan
        chilla_we "Failed to write file"
    ja_we
ja_we
```

---

## Part 9: Type Checking and Validation

### Type Inspection

```jatti
kaam process_value(val)
    chal_oye val_type ban kism(val)
    
    je val_type barabar "number"
        chilla_we "Processing number: " + kism(val)
    nahin_taan_je val_type barabar "string"
        chilla_we "Processing string: " + val
    nahin_taan_je val_type barabar "list"
        chilla_we "Processing list with " + kism(kinna_lamba(val)) + " items"
    nahin_taan
        chilla_we "Unknown type: " + val_type
    ja_we
ja_we

sun_we
    process_value(42)
    process_value("hello")
    process_value([1, 2, 3])
ja_we
```

---

## Part 10: Control Flow Mastery

### Break and Continue

```jatti
sun_we
    har_ek i range_banao(0, 20)
        je i % 2 barabar 0
            chalo_oye_chalo     fuddu_chiz Skip even numbers
        
        je i vadha_hai 15
            roko_oye_roko       fuddu_chiz Exit when > 15
        
        chilla_we i
ja_we
```

### Nested Loops

```jatti
sun_we
    har_ek i range_banao(1, 4)
        har_ek j range_banao(1, 4)
            chilla_we i + " * " + j + " = " + kism(i * j)
ja_we
```

---

## Summary

You've now learned:
- ✅ Advanced collection patterns
- ✅ Recursive functions
- ✅ Function composition
- ✅ Error handling strategies
- ✅ File I/O operations
- ✅ Type checking and validation

---

**Next:** Read [Advanced Topics](./ADVANCED_TOPICS.md) for best practices and optimization techniques!

---

**Happy Coding! 🎉**
