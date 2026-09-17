# Functions - Math & Buchholz OCF Tool

[![C++](https://img.shields.io/badge/C++-26-blue.svg)](https://en.cppreference.com/w/cpp/26)
[![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)](https://www.microsoft.com/windows)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE.txt)
[![Version](https://img.shields.io/badge/version-v1.5.2-orange.svg)](changes.txt)

High-precision mathematical computations and the &zwnj;**Buchholz Ordinal Collapsing Function (OCF)**&zwnj; implemented in C++. Combines classic number theory functions with advanced ordinal theory analysis, supporting calculations from basic combinatorics to large countable ordinal systems.

## Features

### 1. Ackermann Function — A(m, n)

* Classic computable but &zwnj;**not primitive recursive**&zwnj; function
* Supports large inputs, results output as arbitrary-precision strings

### 2. Fibonacci Sequence — f[i]

* High-precision arbitrary-precision arithmetic
* Computes the *i*-th Fibonacci number efficiently

### 3. Euler's Totient Function — φ(n)

* Counts integers less than *n* that are coprime to *n*
* Efficient sieve-based implementation for fast computation

### 4. Catalan Numbers — C(n) / h(n)

* Classic sequence in combinatorial mathematics
* Arbitrary-precision output for large *n*

### 5. Buchholz OCF — Standard Mode

* &zwnj;**Maximum strength**&zwnj;: Buchholz Ordinal (BO) = ψ₀(Ωω</sub>)
* Supported ordinal constants:
  * &zwnj;**ω (omega)**&zwnj;: Countable limit ordinal (ℵ₀)
  * &zwnj;**Ω (Omega)**&zwnj;: First uncountable cardinal (ℵ₁)
  * &zwnj;**Ω[N]**&zwnj;: *N*-th uncountable cardinal in the Buchholz hierarchy
* Input format: `psi_v(alpha)`
* Examples: `psi_0(omega)`, `psi_0(Omega)`, `psi_0(Ω_2)`

### 6. Buchholz OCF — Cardinal & Reflection Extension

* Extends beyond the basic Buchholz ordinal system
* Supported large cardinal constants:
  * &zwnj;**I_rec**&zwnj;: Smallest recursively inaccessible ordinal (KPI strength)
  * &zwnj;**M_rec**&zwnj;: Smallest recursively Mahlo ordinal (KPM strength)
* Reflection operators:
  * &zwnj;**Pi1(n)**&zwnj;: *n*-th admissible ordinal (KP set theory models)
  * &zwnj;**Pi2(n)**&zwnj;: *n*-th recursively inaccessible ordinal
* &zwnj;**Note**&zwnj;: True large cardinals (inaccessible and above) are excluded

### 7. Buchholz OCF — Value Verification

* &zwnj;**100+ built-in verification test cases**&zwnj;
* Validates basic ordinal values:
  * ψ₀(0) = 1
  * ψ₀(1) = ω
  * ψ₀(Ω) = ε₀
* Validates advanced ordinals: ζ₀, Γ₀, BHO, and more

## Getting Started
### Prerequisites
* &zwnj;**Platform**&zwnj;: Windows (requires `<windows.h>`)
* &zwnj;**Compiler**&zwnj;: MinGW-w64 with C++26 support
* &zwnj;**Encoding**&zwnj;: UTF-8
### Building
#### Quick Build
Double-click `build.bat` — it compiles and runs the program automatically.
#### Manual Build
```bash
D:\mingw64\bin\g++.exe -std=c++26 -g main.cpp Function.cpp -o Function.exe -static-libgcc -static-libstdc++
```
#### Running
```bash
.\Function.exe
```
### Project Structure
```text
├── main.cpp          # Entry point, UTF-8 console setup
├── Function.cpp      # Core function implementations
├── Function.h        # Function class declarations
├── BigNumber.h       # Arbitrary-precision arithmetic library
├── BuchOrdinal.h     # Buchholz OCF implementation
├── build.bat         # Quick build & run script
└── LICENSE.txt       # MIT License
```
### Debug Mode
Select ‌`-1` in the menu to toggle debug mode, which outputs detailed computation logs for tracing ordinal calculations.
### Version
**v1.0.0**
### License
This project is licensed under the ‌MIT License‌ — see the LICENSE.txt file for details.
> Copyright (c) 2026 Makeline-pon
