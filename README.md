## ![Lisp Mascot](lisp.png?raw=true)

```
  (fun: fib-rec [n Int] Int
    (if (< n 2) n (+ (fib-rec (dec n)) (fib-rec (dec n 2)))))

  (fib-rec 10)

55
```

### intro
Snabl projects aims to implement a practical embedded Lisp interpreter in C++.

### motivation
I like command lines, almost regardless of type of application. And once you have a command line, scripting is right around the corner. So you might as well plan for it by using a solid foundation. Which means that it makes sense to take a gradual approach to avoid paying upfront. This project is intended to simplify implementing that strategy; by providing a flexible, modular framework for implementing interpreted languages in C++.

### design
The [VM](https://github.com/codr7/snabl/blob/main/src/snabl/m.hpp) is register based with sequential allocation and runs 64-bit [bytecode](https://github.com/codr7/snabl/blob/main/src/snabl/op.hpp). The [evaluation loop](https://github.com/codr7/snabl/blob/main/src/snabl/m.hpp) is implemented using computed goto for performance reasons, which means that the set of available operations is fixed at library compile time. [States](https://github.com/codr7/snabl/blob/main/src/snabl/state.hpp) and [frames](https://github.com/codr7/snabl/blob/main/src/snabl/frame.hpp) are [slab](https://github.com/codr7/snabl/blob/main/src/snabl/frame.hpp) allocated, reference counted and passed as raw pointers. [Types](https://github.com/codr7/snabl/tree/main/src/snabl/types) and [values](https://github.com/codr7/snabl/blob/main/src/snabl/val.hpp) are designed to be (cheaply) passed by value. The [reader](https://github.com/codr7/snabl/blob/main/src/snabl/reader.hpp) uses recursive extent and is designed to be easy to customize/extend.

### syntax
Parens are used for calls only, brackets for vectors.

### status
The codebase is approaching `2`kloc. Currently verifying and tweaking the design to improve performance based on initial profiling. Error checking still leaves a lot to wish for.

### setup
Building the project requires a C++17-compiler and CMake, the following shell spell builds and starts the REPL:

```
$ cd snabl
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./snabl
```

### performance
`bench` returns elapsed time in milliseconds for specified number of repetitions. The current performance focus is identifying and fusing bytecode patterns.

First up is basic recursive Fibonacci, Python3 takes `233`ms on the same machine.

```
  (bench 100 (fib-rec 20))

1037
```

Next tail recursive, Python3 takes `105`ms on the same machine.<br/>
Snabl detects and fuses tail calls automagically at compile time.

```
  (fun: fib-tail [n Int a Int b Int] Int
    (if (z? n) a (if (= n 1) b (fib-tail (dec n) b (+ a b)))))

  (bench 10000 (fib-tail 70 0 1))

476
```

### support
If you wish to support Snabl and help me spend more time and energy on evolving the project, please consider a donation in Bitcoin `3Qv3GdBCabkAustonEoEv63mVXMS8htiB5` or Ether `0x5BD559b709800731324e32eC512d786987DAdb0F`.

### coder/mentor for hire
I'm currently available for hire.<br/>
Remote or relocation within Europe.<br/>
Send a message to codr7 at protonmail and I'll get back to you asap.