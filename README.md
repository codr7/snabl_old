## Snabl

```
  (fun: fib-rec [n Int] Int
    (if (< n 2) n (+ (fib-rec (dec n)) (fib-rec (dec n 2)))))

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

1230
```

### support
Should you wish to support this effort and allow me to spend more time and energy on evolving the project, please consider a [donation](https://liberapay.com/andreas7/donate).

### coder/mentor for hire
I'm currently available for hire.<br/>
Remote or relocation within Europe.<br/>
Send a message to codr7 at protonmail and I'll get back to you asap.