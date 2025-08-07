# Glossary

The following is a glossary of terms using throughout these docs. I had to learn most of these terms myself during the development of this project, so this is for both my sake and anyone elses.

---

- [dword](#dword)
- [ib](#ib)
- [id](#id)
- [modr/m](#modrm)
- [qword](#qword)
- [reg32](#reg32)
- [rel8](#rel8)
- [rel32](#rel32)
- [r/m32](#rm32)
- [word](#word)
- [+rd](#rd)
- [/digit](#digit)
- [/r](#r)

---

## dword

**A 4 byte value**

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

## modr/m

**Mode: register/memory**

This is a byte that follows the base opcode for many instructions. It encodes how to interpret the operands involved.

---

## qword

**An 8 byte value**

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
`jmp +0x12345678` assembles to `EB 78 56 34 12`. `78 56 34 12` is the *rel32*.

---

## r/m32

**Register or memory (32-bit)**

This essentially means either a register or memory address is allowed in this position.

For example:
`push r/m32` means "push a register value or memory value, depending what follows"

---

## word

**A 2 byte value**

`0x1234` is a *word*

---

## +rd

**The base opcode changes depending which register is used**

The rd is a 3 bit code that cooresponds to a register. +rd just means we add that code to the opcode, rather than specifying the register elsewhere.

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

not quite sure lol

---

## /r

not quite sure lol