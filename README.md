## ![Lisp Mascot](lisp.png?raw=true)

```
  (fun: fib-rec [n Int] Int
    (if (< n 2) n (+ (fib-rec (dec n)) (fib-rec (dec n 2)))))
    
  (fib-rec 10)

55
```

### intro
Snabl aims to implement a practical embedded Lisp interpreter in postmodern C++.

### motivation
I like command lines, almost regardless of type of application. And once you have a command line, some flavor of scripting is right around the corner. So you might as well plan for it by using a solid foundation. Which means that it makes sense to take a gradual approach and avoid paying upfront. This project is intended to simplify implementing that strategy; by providing a flexible, modular framework for implementing interpreted languages in C++.

### design
The [VM](https://github.com/codr7/snabl/blob/main/src/snabl/m.hpp) is register based with sequential allocation and runs 64-bit [bytecode](https://github.com/codr7/snabl/blob/main/src/snabl/op.hpp). The [evaluation loop](https://github.com/codr7/snabl/blob/main/src/snabl/m.hpp) is implemented using computed goto for performance reasons, which means that the set of available operations is fixed. [States](https://github.com/codr7/snabl/blob/main/src/snabl/state.hpp) and [frames](https://github.com/codr7/snabl/blob/main/src/snabl/frame.hpp) are [slab](https://github.com/codr7/snabl/blob/main/src/snabl/frame.hpp) allocated, reference counted and passed as raw pointers. [Types](https://github.com/codr7/snabl/tree/main/src/snabl/types) and [values](https://github.com/codr7/snabl/blob/main/src/snabl/val.hpp) are designed to be (cheaply) passed by value. The [reader](https://github.com/codr7/snabl/blob/main/src/snabl/reader.hpp) is implemented using recursive descent and designed to be easy to customize/extend.

### language
The tip of the iceberg is a custom Lisp that wants to be as pragmatic as Common Lisp while dropping cruft & ceremony.

- Parens are for calls, vectors use brackets
- Everything is a method
- `let*` is defult
- Globals look special by definition

### status
The codebase is approaching `3`kloc. Currently verifying and tweaking the design to improve performance based on initial profiling. Error checking still leaves a lot to wish for.

### setup
Building the project requires a C++17-compiler and CMake, the following shell spell builds and starts the [REPL](https://github.com/codr7/snabl/blob/main/src/snabl/repl.cpp):

```
$ snabl
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./snabl
```

### bindings
`let` may be used to bind values to identifiers within a [scope](https://github.com/codr7/snabl/blob/main/src/snabl/scope.hpp).

```
  (let [foo 35 bar (+ foo 7)] bar)

42
```

### tests
`test` may be used to check the result of a piece of code,

```
(test 1 2)

Test 2 = 1...FAIL
F
  (test 42 42)

Test 42 = 42...OK
T
```

Snabl comes with a modest but growing [regression test suite](https://github.com/codr7/snabl/blob/main/test/all.sl).

```
$ cd snabl/build
$ ./snabl ../test/all.sl
Test 42 = 42...OK
...
```

### debugging
`dump` may be used to dump any value to `STDOUT`.

```
  (dump 42)

42
_
```

`debug` may be used to turn tracing on/off.

```
 (+ 1 2)

3
  (debug)

7 STOP
T
  (+ 1 2)

8 STATE_BEG 1 9
9 LOAD_INT1 1 1
10 LOAD_INT1 2 2
11 CALLI1 0 (Fun +)
12 STOP
3
```

### performance
`bench` returns elapsed time in milliseconds for specified number of repetitions. The current performance focus is identifying and fusing bytecode [patterns](https://github.com/codr7/snabl/tree/main/src/snabl/fuses).

First up is basic recursive Fibonacci, Python3 takes `233`ms on the same machine.

```
  (bench 100 (fib-rec 20))

1037
```

Next tail recursive, Python3 takes `105`ms on the same machine.<br/>
Snabl detects and fuses tail calls [automagically](https://github.com/codr7/snabl/blob/main/src/snabl/fuses/tail_call.cpp) at compile time.

```
  (fun: fib-tail [n Int a Int b Int] Int
    (if (z? n) a (if (one? n) b (fib-tail (dec n) b (+ a b)))))

  (bench 10000 (fib-tail 70 0 1))
  
397
```

### support
If you wish to support Snabl and help me spend more time and energy on evolving the project, please consider a donation in Bitcoin `3Qv3GdBCabkAustonEoEv63mVXMS8htiB5` or Ether `0x5BD559b709800731324e32eC512d786987DAdb0F`.

### coder/mentor for hire
I'm currently available for hire.<br/>
Remote or relocation within Europe.<br/>
Send a message to codr7 at protonmail and I'll get back to you asap.