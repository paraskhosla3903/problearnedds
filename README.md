# ProbDS: Probabilistic Data Structures in C++

**ProbDS** is a header-only modern C++ library for implementing and visualising probabilistic data structures like:

- Open Addressing Hash Table with Linear Probing
- **Bloom Filter**
  - Simple Bloom Filter
  - Counting Bloom Filter
  - Cuckoo Filter
- **Linear Counter**

These data structures are designed for **space-efficient approximate membership tests** and **cardinality estimation**, ideal for high-performance applications like databases, caching, networking, and analytics.

## Key Features

- **Header-only**: Just include the headers—no build step or linking needed.
- **Built-in Visualisation**: Use `<DataStructure>Visualiser` classes to print live state, structure, and bitmaps directly to the terminal with color-coded output.
- **Customizable Parameters**: Number of hash functions, fingerprint size, counters, etc.
- **Unit-test ready**: Lightweight and modular design.
- **Thread-safe free**: Single-threaded, focused for embedded and analytical use.

---

## Usage Guide

### 1. Clone the repository

```bash
git clone https://github.com/yourusername/pds.git
cd pds
```

### 2. Set the include path while compiling

Create a usage `test.cpp` file using the available definitions and classes under **ProbDS**, and from the root compile using the following flags.

```bash
clang++ -Iinclude --std=c++17 test.cpp
```
