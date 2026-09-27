# CppEraser

An experimental C++ type erasure generator. Define an interface and generate a wrapper that provides virtual dispatch without modifying the wrapped class.

Try it online: [cpperaser.org](https://cpperaser.org)

## Building CLI from source

Requires CMake. Dependencies are included as Git submodules.

```sh
git submodule update --init --recursive
cmake -S . -B build
cmake --build build
```

## CLI usage

Pass an interface declaration as a quoted command-line argument:

```sh
./build/src/cpperaser 'struct Drawable { void draw() const; };'
```
