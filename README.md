## Snabl

```
  (fun: fib-rec [n Int] Int
    (if (< n 2) n (+ (fib-rec (dec n)) (fib-rec (dec n 2)))))

  (fib-rec 10)

55
```

### intro
This projects aims to implement a practical embedded Lisp interpreter in C++.

### motivation
I like command lines, almost regardless of type of application. And once you have a command line, scripting is right around the corner. So you might as well plan for it by using a solid foundation. Which means that it makes sense to take a gradual approach to avoid paying upfront. This project is intended to simplify implementing that strategy; by providing a flexible, modular framework for implementing interpreted languages in C++.

### design
The VM is register based with sequential allocation and runs 64-bit bytecode. The evaluation loop is implemented using computed goto for performance reasons, which means that the set of available operations is fixed at library compile time. States and frames are slab allocated, reference counted and passed as raw pointers. Types and values are designed to be (cheaply) passed by value.

### syntax
Parens are used for calls only, brackets for vectors.

### status
Still verifying and tweaking the design to improve performance based on initial profiling, error checking still leaves a lot to wish for.

### setup
Compiling Snabl requires a C++17-compiler and CMake, the following shell spell builds and starts the REPL:

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

1105
```

Next tail recursive, Python3 takes `105`ms on the same machine.
```
  (fun: fib-tail [n Int a Int b Int] Int
    (if (= n 0) a (if (= n 1) b (fib-tail (dec n) b (+ a b)))))

  (bench 10000 (fib-tail 70 0 1))

625
```

### support
Should you wish to support this effort and allow me to spend more time and energy on evolving the project, please consider a [donation](https://liberapay.com/andreas7/donate).

### coder/mentor for hire
I'm currently available for hire.<br/>
Remote or relocation within Europe.<br/>
Send a message to codr7 at protonmail and I'll get back to you asap.