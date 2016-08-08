# Custom Grep
-------------------

## Usage
` gcc kmp.c`

`./a.out {pattern} {filename}`

This is a small implementation of KMP Pattern matching in C. System arguments are specified in the above syntax.

## Comparison

Input file length - 105000 letters.

| Obs. Case | Native Grep   |      Custom Grep      |
|:-------------:|:-------------:|:---------------------:|
| 15 Char left/right | 0.006 sec   |  0.007 sec |
| 20 Char left/right | 0.009 sec   |  0.007 sec |
| 30 Char left/right | 0.018 sec   |  0.008 sec |
| 40 Char left/right | 0.027 sec   |  0.008 sec |

## Features
  - Matches pattern and prints the line number and 20 characters before and after.
  - Uses a sliding window to keep track of letters before pattern matching.
  - Reads the file in chunks, so as to manage memory consumption, handles large files really well.
  - DFA like implementation of kmp enables to match pattern across chunks (if broken into halves).

## To Do

 - Implement user input last and next character display near patterns.
 - Make the pattern matching faster, substitute a method instead of sliding window to keep track of back letters.
 - More options for searching.