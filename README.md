# Moxie

An ANSI C99 Preprocessor Macros Header Library for CppUMock

## Getting Started

1. Build [CppUMock](https://github.com/cpputest/cpputest) with *Memory Leak Detection* disabled.
    1. Clone CppUTest: `git clone git@github.com:cpputest/cpputest.git`
    2. Enter Build Directory: `cd cpputest/build`
    3. Generate `configure`: `autoreconf .. -i`
    4. Configure: `../configure --disable-memory-leak-detection`
    5. Build: `make`
    6. Libraries: `ls cpputest/build/lib`
        - `libCppUTest.a`
        - `libCppUTestExt.a`
2. Include `moxie.h`.
3. Link `CppUTestExt` with `-lm` (*Math*) and `-lstdc++` (*C++ Standard Library*).
4. Annotate mockable functions with `#define M_EXPORT_MOCK __attribute__((weak))`.
5. **(Linux)** Compile the *Test* executable with `ld --wrap`s.
6. **(macOS)** Compile the *Test* executable with *DYLD Interposing*.