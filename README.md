## Snabl

```
  (fun: fib-rec [n Int] Int
    (if (< n 2) n (+ (fib-rec (- n 1)) (fib-rec (- n 2)))))
  
  (fib-rec 10)

55
```

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

```
  (bench 100 (fib-rec 20))

2268
```

### support
Should you wish to support this effort and allow me to spend more of my time and energy on evolving GFun, feel free to [help](https://liberapay.com/andreas7/donate) make that economically feasible.

### coder/mentor for hire
I'm currently available for hire.<br/>
Remote or relocation within Europe.<br/>
Send a message to codr7 at protonmail and I'll get back to you asap.