# x86 Instruction Subset

This compiler outputs an x86 machine code executable and uses only a carefully selected subset of the x86 instruction set. These instructions cover practically everything needed for general-purpose computation and are sufficient for the scope of this project.

---

## Table of Contents

- Arithmetic
    - [add](#add)
    - [sub](#sub)
    - [inc](#inc)
    - [dec](#dec)
    - [imul](#imul)
    - [idiv](#idiv)
    - [cdq](#cdq)
    
- Bitwise
    - [and](#and)
    - [or](#or)
    - [xor](#xor)

- Logic
    - [cmp](#cmp)
    - [test](#test)

- Control Flow
    - [jmp](#jmp)
    - [je](#je)
    - [jne](#jne)
    - [jg](#jg)
    - [jl](#jl)
    - [jge](#jge)
    - [jle](#jle)
    - [call](#call)
    - [ret](#ret)

- Data Movement
    - [mov](#mov)
    - [lea](#lea)
    - [push](#push)
    - [pop](#pop)

- Other
    - [nop](#nop)

---

## add
Performs integer addition:
`add dest, src` → `dest = dest + src`

### Encodings:
| Opcode | Form |
| --- | --- |
| `01 /r` | `add r/m32, r32` |
| `03 /r` | `add r32, r/m32` |
| `81 /0` | `add r/m32, imm32` |
| `83 /0` | `add r/m32, imm8` |
| `05` | `add eax, imm32` |

---

## sub  
Performs integer subtraction:  
`sub dest, src` → `dest = dest - src`  

### Encodings:  
| Opcode | Form |  
| --- | --- |  
| `29 /r` | `sub r/m32, r32` |  
| `2B /r` | `sub r32, r/m32` |  
| `81 /5` | `sub r/m32, imm32` |  
| `83 /5` | `sub r/m32, imm8` |  
| `2D` | `sub eax, imm32` | 

---

## inc  
Increments the operand by 1:  
`inc dest` → `dest = dest + 1`  

### Encodings:  
| Opcode | Form |  
| --- | --- |  
| `40 +rd` | `inc r32` |  
| `FF /0`  | `inc r/m32` |  

---

## dec  
Decrements the operand by 1:  
`dec dest` → `dest = dest - 1`  

### Encodings:  
| Opcode | Form |  
| --- | --- |  
| `48 +rd` | `dec r32` |  
| `FF /1`  | `dec r/m32` | 

---

## imul  
Performs signed multiplication:  
`imul dest, src` → `dest = dest * src`  

### Encodings:  
| Opcode | Form |  
| --- | --- |  
| `0F AF /r` | `imul r32, r/m32` |  
| `69 /r`   | `imul r32, r/m32, imm32` |  
| `6B /r`   | `imul r32, r/m32, imm8` |  
| `F7 /5`   | `imul r/m32` | 

---

## idiv
Performs signed division. Divides `edx:eax` by the specified operand. Result:
- Quotient → `eax`
- Remainder → `edx`

### Encodings:  
| Opcode | Form |  
| --- | --- |  
| `F7 /7` | `idiv r/m32` |  

---

## cdq  
Converts double to quadword:  
Sign-extends `eax` into `edx:eax`  

### Encoding:  
| Opcode | Form |  
| --- | --- |  
| `99` | `cdq` |

---

## and  
Performs bitwise AND:  
`and dest, src`  

### Encodings:  
| Opcode | Form |  
| --- | --- |  
| `21 /r` | `and r/m32, r32` |  
| `23 /r` | `and r32, r/m32` |  
| `81 /4` | `and r/m32, imm32` |  
| `83 /4` | `and r/m32, imm8` |  
| `25`    | `and eax, imm32` |  

---

## or  
Performs bitwise OR:  
`or dest, src`  

### Encodings:  
| Opcode | Form |  
| --- | --- |  
| `09 /r` | `or r/m32, r32` |  
| `0B /r` | `or r32, r/m32` |  
| `81 /1` | `or r/m32, imm32` |  
| `83 /1` | `or r/m32, imm8` |  
| `0D`    | `or eax, imm32` | 

---

## xor  
Performs bitwise exclusive OR:  
`xor dest, src`  

### Encodings:  
| Opcode | Form |  
| --- | --- |  
| `31 /r` | `xor r/m32, r32` |  
| `33 /r` | `xor r32, r/m32` |  
| `81 /6` | `xor r/m32, imm32` |  
| `83 /6` | `xor r/m32, imm8` |  
| `35`    | `xor eax, imm32` |  

---

## cmp  
Compares two operands by subtracting `src` from `dest` (without storing result):  
`cmp dest, src`

### Encodings:  
| Opcode | Form |  
| --- | --- |  
| `39 /r` | `cmp r/m32, r32` |  
| `3B /r` | `cmp r32, r/m32` |  
| `81 /7` | `cmp r/m32, imm32` |  
| `83 /7` | `cmp r/m32, imm8` |  
| `3D`    | `cmp eax, imm32` |

### Flags
| Flag | Behaviour |
| --- | --- |
| CF | Set if `src > dest`, when interpreted as unsigned ints |
| ZF | Set if `src == dest` |
| SF | Set if the result of the subtraction is negative |
| OF | Set if signed overflow occurs |

**NOTE:** There are two more flags affected, but neither of them are relevant for this instruction subset

---

## test  
Performs bitwise AND on two operands and sets flags without storing result:  
`test dest, src`

### Encodings:  
| Opcode | Form |  
| --- | --- |  
| `85 /r` | `test r/m32, r32` |  
| `A9`    | `test eax, imm32` |  
| `F7 /0` | `test r/m32, imm32` |

### Flags
| Flag | Behaviour |
| --- | --- |
| CF | Cleared |
| ZF | Set if the result of the AND is zero |
| SF | Set if the result of the AND is negative |
| OF | Cleared |

**NOTE:** There are two more flags affected, but neither of them are relevant for this instruction subset

---

## jmp  
Unconditional jump:  
`jmp offset`  

### Encodings:  
| Opcode | Form |  
| --- | --- |  
| `EB`   | `jmp rel8` (short jump) |  
| `E9`   | `jmp rel32` (near jump) |  

---

## je  
Jump if equal (`ZF == 1`):  
`je offset`  

### Encodings:  
| Opcode | Form |  
| --- | --- |  
| `74`   | `je rel8` (short jump) |  
| `0F 84`| `je rel32` (near jump) |  

---

## jne  
Jump if not equal (`ZF == 0`):  
`jne offset`  

### Encodings:  
| Opcode | Form |  
| --- | --- |  
| `75`   | `jne rel8` (short jump) |  
| `0F 85`| `jne rel32` (near jump) |  

---

## jg  
Jump if greater (`ZF == 0` and `SF == OF`):  
`jg offset`  

### Encodings:  
| Opcode  | Form |  
| ---     | ---  |  
| `7F`    | `jg rel8` (short jump) |  
| `0F 8F` | `jg rel32` (near jump) |  

---

## jl  
Jump if less (`SF != OF`):  
`jl offset`  

### Encodings:  
| Opcode  | Form |  
| ---     | ---  |  
| `7C`    | `jl rel8` (short jump) |  
| `0F 8C` | `jl rel32` (near jump) |  

---

## jge  
Jump if greater or equal (`SF == OF`):  
`jge offset`  

### Encodings:  
| Opcode  | Form |  
| ---     | ---  |  
| `7D`    | `jge rel8` (short jump) |  
| `0F 8D` | `jge rel32` (near jump) |  

---

## jle  
Jump if less or equal (`ZF == 1` or `SF != OF`):  
`jle offset`  

### Encodings:  
| Opcode  | Form |  
| ---     | ---  |  
| `7E`    | `jle rel8` (short jump) |  
| `0F 8E` | `jle rel32` (near jump) |  

---

## call  
Calls a procedure (pushes return address and jumps):  
`call offset`  

### Encodings:  
| Opcode | Form |  
| --- | --- |  
| `E8`   | `call rel32` (near call) |  

---

## ret  
Returns from procedure:  
`ret`  

### Encodings:  
| Opcode | Form |  
| --- | --- |  
| `C3`   | `ret` (near return) |  
| `C2`   | `ret imm16` (near return with stack adjustment) |  

---

## mov  
Moves data from source to destination:  
`mov dest, src`  

### Encodings:  
| Opcode | Form |  
| --- | --- |  
| `89 /r`  | `mov r/m32, r32` |  
| `8B /r`  | `mov r32, r/m32` |  
| `B8 +rd` | `mov r32, imm32` |  
| `C7 /0`  | `mov r/m32, imm32` |  

---

## lea  
Loads effective address into register:  
`lea r32, m`  

### Encodings:  
| Opcode | Form |  
| --- | --- |  
| `8D /r` | `lea r32, m` | 

---

## push  
Pushes a register or memory value onto the stack:  
`push src`  

### Encodings:  
| Opcode | Form |  
| --- | --- |  
| `50 +rd` | `push r32` (register only) |  
| `FF /6`  | `push r/m32` (register or memory) |  
| `6A`     | `push imm8` |  
| `68`     | `push imm32` |  

---

## pop  
Pops a value from the stack into register or memory:  
`pop dest`  

### Encodings:  
| Opcode | Form |  
| --- | --- |  
| `58 +rd` | `pop r32` (register only) |  
| `8F /0`  | `pop r/m32` (register or memory) |  

---

## nop  
No operation (does nothing):  

### Encoding:  
| Opcode | Form |  
| --- | --- |  
| `90` | `nop` | 
