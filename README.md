# CppEraser

An experimental C++ type erasure generator. Define an interface and generate a wrapper that provides virtual dispatch without modifying the wrapped class.

Try it online: [cpperaser.org](https://cpperaser.org)

## Example

Provide a desired interface:

```cpp
struct Drawable {
    void draw() const;
};
```

The generated wrapper can hold any copy-constructible type with compatible methods, without requiring inheritance:

```cpp
struct Circle {
    void draw() const { /* draw a circle */ }
};

struct Square {
    void draw() const { /* draw a square */ }
};

// Use the generated Drawable wrapper, not the input declaration
void render(const Drawable& object) {
    object.draw();
}

void example() {
    render(Drawable{Circle{}});
    render(Drawable{Square{}});
}
```

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
