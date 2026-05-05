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
