# Glossary

The following is a glossary of terms used throughout these docs. I had to learn most of these terms myself during the development of this project, so this is for both my sake and anyone else's.

---

- [dword](#dword)
- [ib](#ib)
- [id](#id)
- [modr/m](#modrm)
    - [mod](#mod)
    - [reg](#reg)
    - [r/m](#rm)
- [qword](#qword)
- [reg32](#reg32)
- [rel8](#rel8)
- [rel32](#rel32)
- [r/m32](#rm32)
- [SIB](#sib)
    - [scale](#scale)
    - [index](#index)
    - [base](#base)
- [word](#word)
- [+rd](#rd)
- [/digit](#digit)
- [/r](#r)

---

## dword

**double word — a 4 byte value**

`0x12345678` is a *dword*

---

## ib

**Immediate byte**

This refers to any byte constant that is part of an instruction.
For example, in `add eax, 0x05`, `0x05` is the *ib*.

---

## id

**Immediate dword** (see [dword](#dword))

This refers to any dword constant that is part of an instruction.
For example, in `mov eax, 0x12345678`, `0x12345678` is the *id*.

These are encoded in **little endian** in the operation bytes.
For example, `mov eax, 0x12345678` would be encoded as `B8 78 56 34 12`, with the last 4 bytes being the little endian version of the id `0x12345678`.

---

## ModR/M

**Mode, register, and register/memory**

This is a byte that follows the base opcode for many instructions. It encodes how to interpret the operands involved. 

This byte is treated as 8 individual bits, which are encoded as follows:

| bit7 | bit6 | bit5 | bit4 | bit3 | bit2 | bit1 | bit0 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| mod | mod | reg | reg | reg | r/m | r/m | r/m |

### mod

This defines the mode that the instruction should use. As follows:

| mod | Meaning | Example |
| --- | --- | --- |
| 00 | Memory, no displacement | `mov eax, [ebx]` |
| 01 | Memory, 8-bit displacement follows | `mov eax, [ebx+0x12]` |
| 10 | Memory, 32-bit displacement follows | `mov eax, [ebx+0x12345678]` |
| 11 | Register only | `mov eax, ebx` |

**Special case:** When `mod = 00` and `r/m = 101`, then a 32-bit *absolute address* follows. For example, `mov eax, [0x12345678]`

### reg

This specifies a register operand. Its exact meaning depends on the opcode used (the byte preceding the modr/m byte). It can refer to either the source or destination operand.

| reg | Register |
| --- | --- |
| 000 | eax |
| 001 | ecx |
| 010 | edx |
| 011 | ebx |
| 100 | esp |
| 101 | ebp |
| 110 | esi |
| 111 | edi |

**Note:** For certain opcodes — known as *grouped opcodes* — this field does **not** specify a register. Instead, it acts as an **opcode extension**, selecting the specific operation to be performed within the opcode group.

For example, the opcode byte `0x80` can represent `add`, `or`, `and`, etc. depending on the value of `reg`.

### r/m

This determines either:
- **When `mod = 11`**: A register operand
- **When `mod < 11`**: A memory addressing mode

| r/m | `mod = 11` | `mod < 11` |
| --- | --- | --- |
| 000 | eax | [eax] |
| 001 | ecx | [ecx] |
| 010 | edx | [edx] |
| 011 | ebx | [ebx] |
| 100 | esp | *SIB byte used* \* |
| 101 | ebp | [ebp] \*\* |
| 110 | esi | [esi] |
| 111 | edi | [edi] |

\* When `r/m = 100` and `mod < 11`, an additional SIB byte (see [SIB](#sib)) follows to encode more complex addressing modes. For example, the operand `[eax + ecx*4 + 8]` would be encoded using the SIB byte.

\*\* When `r/m = 101` and `mod = 00`, the operand becomes a **32-bit absolute address**. This is the only case that behaves this way, which is why it's not shown directly in the table.

---

## qword

**Quad word — an 8 byte value**

`0x123456789ABCDEF0` is a *qword*

---

## reg32

**32 bit register**

`eax`, `ebx`, `ecx`, `edx`, `esi`, `edi`, `esp`, and `ebp` all fall under "reg32".

---

## rel8

**8 bit relative address**

This is used in jumps and calls. It means "this value represents a **signed address offset** from the current address".

For example:
`jmp +0x05` assembles to `EB 05`. `05` is the *rel8*.

---

## rel32

**32 bit relative address**

This is used in jumps and calls. It means "this value represents a **signed address offset** from the current address".

For example:
`jmp +0x12345678` assembles to `E9 78 56 34 12`. `78 56 34 12` is the *rel32*.

---

## r/m32

**Register or memory (32-bit)**

This essentially means either a register or memory address is allowed in this position.

For example:
`push r/m32` means "push a register value or memory value, depending what follows"

---

## SIB

**Scale-Index-Base**

The SIB byte is used to encode more complex memory addressing modes. This follows the ModR/M byte (see [ModR/M](#modrm)) when `mod < 11` and `r/m = 100`.

It allows for memory access in the form:

```
[base + index*scale + displacement]
```

This byte is treated as 8 individual bits, which are encoded as follows:

| bit7 | bit6 | bit5 | bit4 | bit3 | bit2 | bit1 | bit0 |
| --- | --- | --- | --- | --- | --- | --- | --- |
| scale | scale | index | index | index | base | base | base |

### scale

This specifies the multiplication factor applied to index.

| scale | Multiplier |
| --- | --- |
| 00 | 1 |
| 01 | 2 |
| 10 | 4 |
| 11 | 8 |

### index

This selects the **index register** which is multiplied by scale

| reg | Register |
| --- | --- |
| 000 | eax |
| 001 | ecx |
| 010 | edx |
| 011 | ebx |
| 100 | \* |
| 101 | ebp |
| 110 | esi |
| 111 | edi |

\* If `index = 100`, then **no index is used**. This is a special case.

### base

This specifies the **base register**.

| reg | Register |
| --- | --- |
| 000 | eax |
| 001 | ecx |
| 010 | edx |
| 011 | ebx |
| 100 | esp |
| 101 | \* |
| 110 | esi |
| 111 | edi |

\* If `base = 101` and `mod = 00` (from the ModR/M byte), **no base register is used**. For example, `[index*scale + displacement]`.

---

## word

**A 2 byte value**

`0x1234` is a *word*

---

## +rd

**The base opcode changes depending which register is used**

The rd is a 3 bit code that corresponds to a register. +rd just means we add that code to the opcode, rather than specifying the register elsewhere.

The register codes are as follows:
| Register | rd (binary) | rd (decimal) |
| --- | --- | --- |
| EAX | 000 | 0 |
| ECX | 001 | 1 |
| EDX | 010 | 2 |
| EBX | 011 | 3 |
| ESP | 100 | 4 |
| EBP | 101 | 5 |
| ESI | 110 | 6 |
| EDI | 111 | 7 |

For example, `push` uses +rd. since `push` has base opcode `0x50`, we add the register code as follows:
`push eax` → `0x50 + 0` → `0x50`
`push ecx` → `0x50 + 1` → `0x51`
...
`push edi` → `0x50 + 7` → `0x57`

---

## /digit

**Specifies an opcode extension**

This is used in opcode documentation to specify which opcode is being used within an **opcode group**. These are written as `/n`, where `n` is a digit from 0-7 inclusive.

This digit specifies the value of the `reg` field in the ModR/M byte (see [ModR/M](#modrm)).

For example, `F7 /3` means: *use opcode `F7` with `reg = 011` in the ModR/M byte*

This is necessary because grouped instructions like `F7 /3` and `F7 /4` share the same base opcode (`F7`), but represent **different operations**, selected by the `reg` field

---

## /r

**Specifies that an instruction requires a ModR/M byte** (see [ModR/M](#modrm))

This is common for instructions that operate between two operands, like `mov`, `add`, `sub`, etc.

For example, `89 /r` means: *use opcode `89`, followed by a ModR/M byte to specify operands*
