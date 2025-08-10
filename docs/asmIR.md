# Assembly Intermediate Representation

This document describes the **Assembly-level Intermediate Representation (IR)** used in this compiler.

This IR serves several purposes:
- **Easier debugging:** Provides a clear checkpoint in the compilation pipeline for inspection and troubleshooting.
- **Simplification:** Breaks down the code generation step into a more manageable process, reducing complexity in the backend.
- **Future Proofing:** Allows new opcodes or instructions to be added with minimal impact on the code generation module.

While an IR at this stage is not strictly required for a functioning compiler, it is a common industry practice. This version is intentionally simpler and less feature-rich than those found in production compilers, prioritizing ease of development while still reflecting the architecture of a commercial-scale pipeline.

---

## Table of Contents

- [Format](#format)
- [Encodings](#encodings)
    - [Opcodes](#opcodes)
    - [Registers](#registers)
    - [Memory](#memory)
    - [Immediates](#immediates)

---

## Format

This IR primarily uses a single byte per token. 

For example, instead of the string `mov`, there would be one or more bytes that directly represent that.

This is extremely computer friendly, and drastically simplifies the logic around this IR, while still being relatively human readable.

The format of this IR aims to be as simple as possible, while being expressive enough to describe all possible assembly code. This is done by allowing a large amount of freedom in the typing.

---

## Encodings

### Opcodes

Opcodes are signaled by the prefix of character `'O'` or byte `0x4F` (they are equivalent). One single byte follows that, signaling which opcode is being used.

The translations are as follows:
|  Opcode  |  Byte   |   |  Opcode  |  Byte   |
|----------|---------|---|----------|---------|
| `add`    | `0x00`  |   | `jl`     | `0x10`  |
| `sub`    | `0x01`  |   | `jge`    | `0x11`  |
| `inc`    | `0x02`  |   | `jle`    | `0x12`  |
| `dec`    | `0x03`  |   | `call`   | `0x13`  |
| `imul`   | `0x04`  |   | `ret`    | `0x14`  |
| `idiv`   | `0x05`  |   | `mov`    | `0x15`  |
| `cdq`    | `0x06`  |   | `lea`    | `0x16`  |
| `and`    | `0x07`  |   | `push`   | `0x17`  |
| `or`     | `0x08`  |   | `pop`    | `0x18`  |
| `xor`    | `0x09`  |   | `nop`    | `0x19`  |
| `cmp`    | `0x0A`  |
| `test`   | `0x0B`  |
| `jmp`    | `0x0C`  |
| `je`     | `0x0D`  |
| `jne`    | `0x0E`  |
| `jg`     | `0x0F`  |

### Registers

Registers are signaled by the prefix of character `'R'` or byte `0x52` (they are equivalent). One single byte follows that, signaling which register is being used.

The translations are as follows:
| Register |  Byte   |
|----------|---------|
| `eax`    | `0x00`  |
| `ebx`    | `0x01`  |
| `ecx`    | `0x02`  |
| `edx`    | `0x03`  |
| `esi`    | `0x04`  |
| `edi`    | `0x05`  |
| `esp`    | `0x06`  |
| `ebp`    | `0x07`  |

### Memory

### Immediates

