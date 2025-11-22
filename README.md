# sdl_test

## Description
A small project whose purpose was to learn the following :
- CMake build on linux
- Error handling with `std::unexpected`
- How moving works in cpp and `r_value`
- The SDL framework

Additionaly I wanted to refresh my cpp and seeing how my understanding improved after programming in rust for a while. \
Conclusion, definitely useful in understanding how moving data works (although the destructor being called in cpp feels strange). It also helps to understand why `std::unique_ptr<Foo>` is needed for polymorphism and how smart pointers work in general.

## Build & Run
<ins>How to build :</ins>
```sh
cmake -B build
cmake --build build
```
This is untested on Windows or Mac but should work similarly in principle.

<ins>How to run :</ins> \
`./build/sdl_test `
