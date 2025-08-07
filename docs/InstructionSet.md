# x86 Instruction Subset

This compiler outputs an x86 machine code executable and uses only a carefully selected subset of the x86 instruction set. These instructions cover practically everything needed for general-purpose computation and are sufficient for compiling a C-like language. This section also serves as the table of contents — all entries are clickable.

---

## Table of Contents

- [Arithmetic](#Arithmetic)
    - [add](#add)
    - [sub](#sub)
    - [inc](#inc)
    - [dec](#dec)
    - [imul](#imul)
    - [idiv](#idiv)
    - [cdq](#cdq)

- [Logic & Bitwise](#logic--bitwise)
    - [cmp](#cmp)
    - [test](#test)
    - [and](#and)
    - [or](#or)
    - [xor](#xor)

- [Control Flow](#control-flow)
    - [jmp](#jmp)
    - [je](#je)
    - [jne](#jne)
    - [jg](#jg)
    - [jl](#jl)
    - [jge](#jge)
    - [jle](#jle)
    - [call](#call)
    - [ret](#ret)

- [Data Movement](#data-movement)
    - [mov](#mov)
    - [lea](#lea)
    - [push](#push)
    - [pop](#pop)

---

## Arithmetic

### add
Performs integer addition:
`add dest, src` → `dest = dest + src`

### sub
Performs integer subtraction:
`sub dest, src` → `dest = dest - src`

### inc
Increments a value by 1:
`inc reg` → `reg = reg + 1`

### dec
Decrements a value by 1:
`dec reg` → `reg = reg - 1`

### imul
Performs signed integer multiplication. Syntax variants include:
- `imul reg, reg`
- `imul reg, reg, imm`

### idiv
Performs signed division. Divides `edx:eax` by the specified operand. Result:
- Quotient → `eax`
- Remainder → `edx`

### cdq
Sign-extends `eax` into `edx` for use with `idiv`. (Makes a 64-bit dividend.)

---

## Logic & Bitwise

### cmp
Subtracts two values and sets flags, discarding the result:
`cmp a, b`

### test
Performs bitwise AND without storing result, just sets flags:
`test a, b`

### and
Bitwise AND:
`and dest, src` → `dest = dest & src`

### xor
Bitwise XOR:
`xor dest, src` → `dest = dest ^ src`

Commonly used to zero registers:
`xor eax, eax` → `eax = 0`

### or
Bitwise OR: 
`or dest, src` → `dest = dest | src`

---

## Control Flow

### jmp
Unconditional jump to label or address: 
`jmp label`

### je
Jump if equal (ZF = 1):
`je label`

### jne
Jump if not equal (ZF = 0):
`jne label`

### jg
Jump if greater (ZF = 0 and SF = OF):
`jg label`

### jl
Jump if less (SF ≠ OF):
`jl label`

### jge
Jump if greater or equal (SF = OF):
`jge label`

### jle
Jump if less or equal (ZF = 1 or SF ≠ OF):
`jle label`

### call
Calls a function. Pushes return address, then jumps:
`call label`

### ret
Returns from a function. Pops address off the stack:
`ret`

---

## Data Movement

### mov
Copies data between registers, memory, and immediates:
`mov dest, src`

### lea
Loads the address of a memory operand without dereferencing:
- Example: `lea eax, [ebx + ecx*4 + 8]`

### push
Pushes a value onto the stack:
`push reg`

### pop
Pops a value off the stack into a register:
`pop reg`
