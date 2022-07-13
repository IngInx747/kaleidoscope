# Kaleidoscope: Implementing a Language with LLVM

Tutorial page: **[Link](https://llvm.org/docs/tutorial/)**

The repo contains the codes implementing the frontend and backend of Kaleidoscope.
The frontend uses flex-bison for lexer and LR parser instead of tutorial's hardcoding approach.
The grammar is a little bit different from the tutorial's.
The JIT and user-defined operators are NOT included.

Environment: LLVM-10, clang, flex, bison, on Linux

Build: `> make`

Test: `> ./kcc test-case.kal`

## Target Object

The Kaleidoscope code can be compiled to the object code on the target machine of many popular archs.
The example is provided as `cases/fib.kal` which has 2 functions `fib_1` and `fib_2`.
The driver code `fib.c` has the entry point to launch these 2 functions and display the results.
Wrapping up:

```
> ./kcc fib.kal # an object named fib.kal.o will be generated
> gcc fib.c fib.kal.o -o fib.out # link the object into executable
> ./fib.out # the results of fib_1() and fib_2() are printed out
```

## DWARF Debug Info

The kaleidoscope can insert debug info into the object which can be used by debugger later.
The optimization is disabled when debug mode is enabled.
To generate IR with debug info:

```
> ./kcc t9.kal > t9.ll # generate IR
> clang -x ir t9.ll -o t9.out # clang will create an executable
> gdb t9.out # standard gdb usage
```

| Source panel | Debugger prompt |
| ------------ | --------------- |
| ![img](./illust/gdb-src.png?raw=true)  | ![img](./illust/gdb-cmd.png?raw=true)  |

Note: To tell the entry point to clang, a function `main()` is required in `t9.kal`.
