# Assembly Intermediate Representation

This document describes the **Assembly-level Intermediate Representation (IR)** used in this compiler.

The IR serves several purposes:
- **Easier debugging:** Acts as a clear checkpoint in the compilation pipeline for inspection and troubleshooting.  
- **Simplification:** Breaks down the code generation step into a more manageable process, reducing backend complexity.  
- **Future-proofing:** Allows new opcodes or instructions to be added with minimal impact on the code generation module.  

While an IR at this stage is not strictly required for a functioning compiler, it is common industry practice.  
This version is intentionally simpler and less feature-rich than those found in production compilers, prioritizing ease of development while still reflecting the architecture of a commercial-scale pipeline.

---

## Table of Contents
- [Format](#format)  
- [Encodings](#encodings)  
    - [Opcodes](#opcodes)  
    - [Registers](#registers)  
    - [Memory](#memory)  
    - [Immediates](#immediates)  
- [Example Translation](#example-translation)  

---

## Format

This IR prioritizes compactness and computer-friendliness. It uses plain bytes to represent most elements, with readable ASCII characters where possible to improve human readability.

For example, instead of storing the string `mov`, the IR uses one or more bytes that directly represent the instruction.

This approach simplifies parsing logic while remaining relatively human-readable.  
The format aims to be as simple as possible while still being expressive enough to represent any assembly code, allowing flexibility in typing.

---

## Encodings

### Opcodes

Opcodes are signaled by the prefix character `'O'` (ASCII `0x4F`). One byte follows, indicating which opcode is being used.

| Opcode  | Byte   |   | Opcode  | Byte   |
|---------|--------|---|---------|--------|
| `add`   | `0x00` |   | `jl`    | `0x10` |
| `sub`   | `0x01` |   | `jge`   | `0x11` |
| `inc`   | `0x02` |   | `jle`   | `0x12` |
| `dec`   | `0x03` |   | `call`  | `0x13` |
| `imul`  | `0x04` |   | `ret`   | `0x14` |
| `idiv`  | `0x05` |   | `mov`   | `0x15` |
| `cdq`   | `0x06` |   | `lea`   | `0x16` |
| `and`   | `0x07` |   | `push`  | `0x17` |
| `or`    | `0x08` |   | `pop`   | `0x18` |
| `xor`   | `0x09` |   | `nop`   | `0x19` |
| `cmp`   | `0x0A` |   |         |        |
| `test`  | `0x0B` |   |         |        |
| `jmp`   | `0x0C` |   |         |        |
| `je`    | `0x0D` |   |         |        |
| `jne`   | `0x0E` |   |         |        |
| `jg`    | `0x0F` |   |         |        |

---

### Registers

Registers are signaled by the prefix character `'R'` (ASCII `0x52`). One byte follows, indicating which register is being referenced.

| Register | Byte   |
|----------|--------|
| `eax`    | `0x00` |
| `ecx`    | `0x01` |
| `edx`    | `0x02` |
| `ebx`    | `0x03` |
| `esp`    | `0x04` |
| `ebp`    | `0x05` |
| `esi`    | `0x06` |
| `edi`    | `0x07` |

---

### Memory

A memory operand is any value accessed via an address, typically shown in x86 assembly with square brackets.

Memory references are signaled by the prefix character `'M'` (ASCII `0x4D`). This is followed by:
1. A single register byte (as defined in [Registers](#registers), **without** the `'R'` prefix).  
2. An immediate value encoding (as defined in [Immediates](#immediates), **without** the `'I'` prefix).

**Structure:**
| Byte 0 | Byte 1  | Byte 2          | Byte 3+         |
|--------|---------|-----------------|-----------------|
| `'M'`  | Register| Immediate Length| Immediate Value |

**Example:**  
The reference `[ecx + 64]` would be encoded as:

```
4D 01 31 40
```

- `4D` → `'M'`  
- `01` → register `ecx`  
- `31` → `'1'` (1-byte offset)  
- `40` → value `64` (hex)  

---

### Immediates

An immediate is any constant value used in an instruction.

Immediates are signaled by the prefix `'I'` (ASCII `0x49`). This is followed by:
1. A single byte indicating the size (ASCII `'1'` for 1 byte, `'4'` for 4 bytes).  
2. The immediate value in **big-endian** hex.

| Size (bytes) | Char  | Hex   |
|--------------|-------|-------|
| 1            | `'1'` | `0x31`|
| 4            | `'4'` | `0x34`|

**Note:** Currently, only 1- and 4-byte immediates are supported. Two-byte immediates are valid in x86 but not yet implemented for simplicity.

**Example:**  
The value `0x12345678` is encoded as:

```
49 34 12 34 56 78
```

- `49` → `'I'`  
- `34` → `'4'` (4-byte immediate)  
- `12 34 56 78` → big-endian value  

---

## Example Translation

Let’s translate the following assembly snippet into this IR format:

```
mov eax, 5          ; eax = 5
add eax, 10         ; eax = eax + 10
mov ecx, [eax]      ; ecx = *eax
```

| Token  | Prefix        | Value               | Bytes           |
|--------|---------------|---------------------|-----------------|
| `mov`  | `'O'` = `0x4F`| `0x15`              | `4F 15`         |
| `eax`  | `'R'` = `0x52`| `0x00`              | `52 00`         |
| `5`    | `'I'` = `0x49`| `0x31` + `0x05`     | `49 31 05`      |
| `add`  | `'O'` = `0x4F`| `0x00`              | `4F 00`         |
| `eax`  | `'R'` = `0x52`| `0x00`              | `52 00`         |
| `10`   | `'I'` = `0x49`| `0x31` + `0x0A`     | `49 31 0A`      |
| `mov`  | `'O'` = `0x4F`| `0x15`              | `4F 15`         |
| `ecx`  | `'R'` = `0x52`| `0x01`              | `52 01`         |
| `[eax]`| `'M'` = `0x4D`| (see note)          | `4D 00 31 00`   |

**Note:** For memory references, the register and offset appear without the `'R'` or `'I'` prefixes. Here:  
- `0x00` → register `eax`  
- `0x31` → offset length = 1 byte  
- `0x00` → offset value = 0  

**Final encoded bytes:**

```
4F 15   52 00   49 31 05        ; mov eax, 5
4F 00   52 00   49 31 0A        ; add eax, 10
4F 15   52 01   4D 00 31 00     ; mov ecx, [eax]
```
