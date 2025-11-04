# Packaged Executable File Format

This document outlines the format of a Windows Packaged Executable file. While all this information is available online, this is simply my personal relay of that information.
The PE format is quite extensive, so below is a TOC with links to each subsection.

---

## Table of Contents

- [1. DOS Header (`IMAGE_DOS_HEADER`)](#1-dos-header-image_dos_header)
  - [e_magic](#e_magic)
  - [e_cblp](#e_cblp)
  - [e_cp](#e_cp)
  - [e_crlc](#e_crlc)
  - [e_cparhdr](#e_cparhdr)
  - [e_minalloc](#e_minalloc)
  - [e_maxalloc](#e_maxalloc)
  - [e_ss](#e_ss)
  - [e_sp](#e_sp)
  - [e_csum](#e_csum)
  - [e_ip](#e_ip)
  - [e_cs](#e_cs)
  - [e_lfarlc](#e_lfarlc)
  - [e_ovno](#e_ovno)
  - [e_res](#e_res)
  - [e_oemid](#e_oemid)
  - [e_oeminfo](#e_oeminfo)
  - [e_res2](#e_res2)
  - [e_lfanew](#e_lfanew)

- [2. DOS Stub](#2-dos-stub)

- [2.5 Padding](#25-padding)

- [3. PE Signature](#3-pe-signature)

- [4. COFF File Header (`IMAGE_FILE_HEADER`)](#4-coff-file-header-image_file_header)
  - [Machine](#machine)
  - [NumberOfSections](#numberofsections)
  - [TimeDateStamp](#timedatestamp)
  - [PointerToSymbolTable](#pointertosymboltable)
  - [NumberOfSymbols](#numberofsymbols)
  - [SizeOfOptionalHeader](#sizeofoptionalheader)
  - [Characteristics](#characteristics)

- [5. Optional Header (`IMAGE_OPTIONAL_HEADER32`)](#5-optional-header-image_optional_header32)
  - [5.1 Standard Fields](#51-standard-fields)
    - [Magic](#magic)
    - [MajorLinkerVersion](#majorlinkerversion)
    - [MinorLinkerVersion](#minorlinkerversion)
    - [SizeOfCode](#sizeofcode)
    - [SizeOfInitializedData](#sizeofinitializeddata)
    - [SizeOfUninitializedData](#sizeofuninitializeddata)
    - [AddressOfEntryPoint](#addressofentrypoint)
    - [BaseOfCode](#baseofcode)
    - [BaseOfData](#baseofdata)
  - [5.2 Windows-Specific Fields](#52-windows-specific-fields)
    - [ImageBase](#imagebase)
    - [SectionAlignment](#sectionalignment)
    - [FileAlignment](#filealignment)
    - [MajorOperatingSystemVersion](#majoroperatingsystemversion)
    - [MinorOperatingSystemVersion](#minoroperatingsystemversion)
    - [MajorImageVersion](#majorimageversion)
    - [MinorImageVersion](#minorimageversion)
    - [MajorSubsystemVersion](#majorsubsystemversion)
    - [MinorSubsystemVersion](#minorsubsystemversion)
    - [Win32VersionValue](#win32versionvalue)
    - [SizeOfImage](#sizeofimage)
    - [SizeOfHeaders](#sizeofheaders)
    - [CheckSum](#checksum)
    - [Subsystem](#subsystem)
    - [DllCharacteristics](#dllcharacteristics)
    - [SizeOfStackReserve](#sizeofstackreserve)
    - [SizeOfStackCommit](#sizeofstackcommit)
    - [SizeOfHeapReserve](#sizeofheapreserve)
    - [SizeOfHeapCommit](#sizeofheapcommit)
    - [LoaderFlags](#loaderflags)
    - [NumberOfRvaAndSizes](#numberofrvaandsizes)
  - [5.3 Data Directories (`IMAGE_DATA_DIRECTORY[16]`)](#53-data-directories-image_data_directory16)
    - [VirtualAddress](#virtualaddress)
    - [Size](#size)
    - [Directory Entries](#directory-entries)
      - [Export Table](#export-table)
      - [Import Table](#import-table)
      - [Resource Table](#resource-table)
      - [Exception Table](#exception-table)
      - [Certificate Table](#certificate-table)
      - [Base Relocation Table](#base-relocation-table)
      - [Debug Directory](#debug-directory)
      - [Architecture](#architecture)
      - [Global Pointer](#global-pointer)
      - [TLS Table](#tls-table)
      - [Load Configuration Table](#load-configuration-table)
      - [Bound Import Table](#bound-import-table)
      - [Import Address Table (IAT)](#import-address-table-iat)
      - [Delay Import Descriptor](#delay-import-descriptor)
      - [COM Descriptor](#com-descriptor)
      - [Reserved](#reserved)

- [6. Section Table (`IMAGE_SECTION_HEADER[]`)](#6-section-table-image_section_header)
  - [Name](#name)
  - [VirtualSize](#virtualsize)
  - [VirtualAddress](#virtualaddress)
  - [SizeOfRawData](#sizeofrawdata)
  - [PointerToRawData](#pointertorawdata)
  - [PointerToRelocations](#pointertorelocations)
  - [PointerToLinenumbers](#pointertolinenumbers)
  - [NumberOfRelocations](#numberofrelocations)
  - [NumberOfLinenumbers](#numberoflinenumbers)
  - [Characteristics](#characteristics-section)

- [7. Section Contents](#7-section-contents)
  - [.text](#text)
  - [.data](#data)
  - [.rdata](#rdata)
  - [.bss](#bss)
  - [.idata](#idata)
  - [.edata](#edata)
  - [.rsrc](#rsrc)

- [8. Import Table (`IMAGE_IMPORT_DESCRIPTOR[]`)](#8-import-table-image_import_descriptor)
  - [OriginalFirstThunk](#originalfirstthunk)
  - [TimeDateStamp](#timedatestamp-import)
  - [ForwarderChain](#forwarderchain)
  - [Name](#name-import)
  - [FirstThunk](#firstthunk)
  - [Import Name Table (INT)](#import-name-table-int)
  - [Import Address Table (IAT)](#import-address-table-iat)
  - [Hint/Name Entry](#hintname-entry)

- [9. Export Table (`IMAGE_EXPORT_DIRECTORY`)](#9-export-table-image_export_directory)
  - [Characteristics](#characteristics-export)
  - [TimeDateStamp](#timedatestamp-export)
  - [MajorVersion](#majorversion)
  - [MinorVersion](#minorversion)
  - [Name](#name-export)
  - [Base](#base)
  - [NumberOfFunctions](#numberoffunctions)
  - [NumberOfNames](#numberofnames)
  - [AddressOfFunctions](#addressoffunctions)
  - [AddressOfNames](#addressofnames)
  - [AddressOfNameOrdinals](#addressofnameordinals)

- [10. Relocation Table (`IMAGE_BASE_RELOCATION[]`)](#10-relocation-table-image_base_relocation)
  - [VirtualAddress](#virtualaddress-reloc)
  - [SizeOfBlock](#sizeofblock)
  - [TypeOffset[]](#typeoffset)

- [11. Resource Table (`IMAGE_RESOURCE_DIRECTORY`)](#11-resource-table-image_resource_directory)
  - [Characteristics](#characteristics-resource)
  - [TimeDateStamp](#timedatestamp-resource)
  - [MajorVersion](#majorversion-resource)
  - [MinorVersion](#minorversion-resource)
  - [NumberOfNamedEntries](#numberofnamedentries)
  - [NumberOfIdEntries](#numberofidentries)

- [12. Debug Directory (`IMAGE_DEBUG_DIRECTORY`)](#12-debug-directory-image_debug_directory)

- [13. Certificate Table](#13-certificate-table)

- [14. TLS Directory (`IMAGE_TLS_DIRECTORY`)](#14-tls-directory-image_tls_directory)

- [15. Load Configuration Directory](#15-load-configuration-directory)

- [16. Bound Import Directory](#16-bound-import-directory)

- [17. COM Descriptor Directory](#17-com-descriptor-directory)

- [18. Overlay Data](#18-overlay-data)

- [19. Summary of Load Order](#19-summary-of-load-order)

---

## 1. DOS Header (`IMAGE_DOS_HEADER`)

The DOS header is the very first structure in a PE file.
It contains legacy fields from the MS-DOS executable format (MZ), which is hardly relevant to the PE file format. The only notable fields for our purposes are:
- `e_magic` -> Tells Windows to treat the file as a PE, meaning it will directly jump to the following field and skip the rest of the data.
- `e_lfanew` -> Tells Windows where the PE header begins.

---

### e_magic
**Offset:** 0x00  
**Size:** 2 bytes  
**Description:** This field is responsible for marking the file as a DOS executable, which is necessary for Windows to load it as a PE. The required value for this field is `0x5A4D`, corresponding to the ASCII characters `MZ`.

---

### e_cblp
**Offset:** 0x02  
**Size:** 2 bytes  
**Description:** The size of the last page of the file. Pages are 512 bytes in this context, so `e_cblp = (filesize % 512)`. When this field is set to 0, the last page is interpreted to be full (512 bytes). Together with `e_cp`, this determines total filesize.

---

### e_cp
**Offset:** 0x04  
**Size:** 2 bytes  
**Description:** The amount of pages the file occupies. Pages are 512 bytes in this context, and this *includes* the last page even if it's incomplete, so `e_cp = (filesize / 512) + ((filesize % 512 == 0) ? 0 : 1)`. Together with `e_cblp`, this determines total filesize.

---

### e_crlc
**Offset:** 0x06  
**Size:** 2 bytes  
**Description:** The amount of entries contained in the **relocation table**. This says how many 4 byte entries should be read, starting from the offset specified by `e_lfarlc`.

---

### e_cparhdr
**Offset:** 0x08  
**Size:** 2 bytes  
**Description:** The amount of **paragraphs** that make up the header, each paragraph being 16 bytes. While this value is ignored for PE files, it's usually set to 4, as the DOS Header is 64 bytes long.

---

### e_minalloc
**Offset:** 0x0A  
**Size:** 2 bytes  
**Description:** The minimum amount of **paragraphs** the program needs beyond the header, each paragraph being 16 bytes.

---

### e_maxalloc
**Offset:** 0x0C  
**Size:** 2 bytes  
**Description:** The maximum amount of **paragraphs** the program needs beyond the header, each paragraph being 16 bytes.

---

### e_ss
**Offset:** 0x0E  
**Size:** 2 bytes  
**Description:** The address of the initial stack pointer value measured in **paragraphs**, each paragraph being 16 bytes. If the stack does not start on a multiple of 16, `e_sp` is responsible for declaring that offset. `e_ss = (stack_start / 16)`.

---

### e_sp
**Offset:** 0x10  
**Size:** 2 bytes  
**Description:** The specific address of the initial stack pointer value, as an offset from the address specified by `e_ss`. `e_sp = stack_start - (e_ss * 16)`. In practice, there's no reason for this value to exceed 15. While the loader supports larger values, the maximum physical address is limited by real-mode memory (less than 1 MB), so `e_ss` *always* has enough room to hold the entire paragraph count (`(0xFFFF * 16) < 1 MB`).

---

### e_csum
**Offset:** 0x12  
**Size:** 2 bytes  
**Description:** The **additive checksum** of the file. On load, the DOS loader will sum the bytes of the entire file, ignoring overflows (conceptually equivalent to mod `0x10000`), and cross check with this value. 

---

### e_ip
**Offset:** 0x14  
**Size:** 2 bytes  
**Description:** The specific address of the initial instruction pointer value, as an offset from the address specified by `e_cs`. `e_ip = code_start - (e_cs * 16)`. In practice, there's no reason for this value to exceed 15. While the loader supports larger values, the maximum physical address is limited by real-mode memory (less than 1 MB), so `e_cs` *always* has enough room to hold the entire paragraph count (`(0xFFFF * 16) < 1 MB`).

---

### e_cs
**Offset:** 0x16  
**Size:** 2 bytes  
**Description:** The address of the initial instruction pointer value measured in **paragraphs**, each paragraph being 16 bytes. If the code does not start on a multiple of 16, `e_ip` is responsible for declaring that offset. `e_cs = (code_start / 16)`.

---

### e_lfarlc
**Offset:** 0x18  
**Size:** 2 bytes  
**Description:** The offset of the start of the **relocation table** within the file. Together with `e_crlc`, which defines the length of the array, this lets Windows parse the relocation table for handling.

---

### e_ovno
**Offset:** 0x1A  
**Size:** 2 bytes  
**Description:** The **overlay numbers** of this file. An overlay is a logical segment of memory, usually 64 KB in the context of a DOS program. This field is only necessary if the program requires more than one segment. If that is the case, there will be multiple executable files, and this number identifies which overlay this particular file belongs to.

---

### e_res
**Offset:** 0x1C  
**Size:** 8 bytes (4 × 2 bytes)  
**Description:** This field is essentially just padding. You *can* put values here to be read by the program itself, but the loader will completely ignore it. Think of it like a set of optional read-only data fields for the program.

---

### e_oemid
**Offset:** 0x24  
**Size:** 2 bytes  
**Description:** **Original Equipment Manufacturer** ID. While it was initially intended to store an id corresponding to the system that created the executable, these days it's usually left as 0.

---

### e_oeminfo
**Offset:** 0x26  
**Size:** 2 bytes  
**Description:** **Original Equipment Manufacturer** Info. This is used to store specific info about the OEM. Like `e_oemid`, these days it's usually left as 0.

---

### e_res2
**Offset:** 0x28  
**Size:** 20 bytes (10 × 2 bytes)  
**Description:** This field is essentially just padding. You *can* put values here to be read by the program itself, but the loader will completely ignore it. Think of it like a set of optional read-only data fields for the program.

---

### e_lfanew
**Offset:** 0x3C  
**Size:** 4 bytes  
**Description:** Offset to the PE header (start of `"PE\0\0"`) signature. The standard value for this is `0x80`, due to how the following sections are laid out.

---

## 2. DOS Stub

The DOS Stub contains a small program that prints the string `This program cannot be run in DOS mode.\r\r$`. The `$` acts as the terminator for this string.
This is made up of the following instructions:
```
0E 1F          ; push cs
BA 0E 00       ; mov dx, offset msg
B4 09          ; mov ah, 09h (DOS print string)
CD 21          ; int 21h
B8 01 4C       ; mov ax, 4C01h (terminate program)
CD 21          ; int 21h
```
followed by the string itself:
```
54 68 69 73 20 70 72 6F 
67 72 61 6D 20 63 61 6E 
6E 6F 74 20 62 65 20 72 
75 6E 20 69 6E 20 44 4F 
53 20 6D 6F 64 65 2E 0D 
0D 24
```
So the complete data for this section is:
```
0E 1F BA 0E 00 B4 09 CD 
21 B8 01 4C CD 21 54 68 
69 73 20 70 72 6F 67 72 
61 6D 20 63 61 6E 6E 6F 
74 20 62 65 20 72 75 6E 
20 69 6E 20 44 4F 53 20 
6D 6F 64 65 2E 0D 0D 24
```
While this section is never used in PE files, this is the **standard data** for this section.

---

## 2.5 Padding

This is an unofficial section of the PE file format. Its simply 8 extra `0x00` bytes that follow the previous section (the **DOS Stub**) to let the following section (the **PE Signature**) start on a multiple of 16.

As is defined above, the DOS Header and DOS Stub total to `0x78` bytes. With these padding bytes, it totals to `0x80`. While this hardly makes a difference on modern systems, its standard practice in the PE format.

```
00 00 00 00 00 00 00 00
```

---

## 3. PE Signature

The PE Signature marks the beginning of the Windows-specific part of the executable.  
This is the section that is pointed to by the `e_lfanew` field of the **DOS Header**, so its where Windows will jump when it sees `e_magic == 0x5A4D`.

The PE Signature is 4 bytes long and should always contain the ASCII string `"PE\0\0"` (`0x50450000`).

---

## 4. COFF File Header (`IMAGE_FILE_HEADER`)

This header is responsible for defining how the file should be handled by the OS at a very high level. 

At a high level, it describes:
- The **machine architecture** (x86, x64, ARM, etc.)
- The **number of sections** in the file
- **Timestamps and symbol table info** (used mainly for debugging)
- Characteristics that describe what kind of file this is (DLL, EXE, etc.)

---

### Machine
**Offset:** 0x00  
**Size:** 2 bytes  
**Description:** Indicates the target CPU architecture of the file. 

Common values include:
| Architecture | Value |
| ------------ | ----- |
| Intel 386 (x86) | `0x014C` |
| AMD64 (x64) | `0x8664` |
| ARM (32-bit) | `0x01C0` |
| ARM64 | `0xAA64` |

---

### NumberOfSections
**Offset:** 0x02  
**Size:** 2 bytes  
**Description:** How many sections (`.text`, `.data`, etc.) are included in the file. These exist after the **Optional Header**. The loader needs this field to know how much data to read, in whatever sizes are defined in the sections themselves.

---

### TimeDateStamp
**Offset:** 0x04  
**Size:** 4 bytes  
**Description:** A **UNIX Timestamp** (seconds since Jan 1, 1970 UTC). While this field is ignored by the Windows loader, its used by some development tools, so should always be correctly populated.

---

### PointerToSymbolTable
**Offset:** 0x08  
**Size:** 4 bytes  
**Description:** This is for the **COFF Debugging System**. This is the offset of the symbol table within the file, used for debugging.
It is not used by the Windows loader, since PE files have their own system for debugging information. **Normally set to 0**.

---

### NumberOfSymbols
**Offset:** 0x0C  
**Size:** 4 bytes  
**Description:** This is for the **COFF Debugging System**. This is the amount of entries in the symbol table, used for debugging.
It is not used by the Windows loader, since PE files have their own system for debugging information. **Normally set to 0**.

---

### SizeOfOptionalHeader
**Offset:** 0x10  
**Size:** 2 bytes  
**Description:** Specifies the size (in bytes) of the **Optional Header**. The optional header size can vary between PE format types, so this field tells the loader where the section headers begin (since thats where it jumps next).

---

### Characteristics
**Offset:** 0x12  
**Size:** 2 bytes  
**Description:** This field defines what type the file is, and therefore, how Windows will process it. It is stored as a single value, representing a set of flag values (using **binary or** to combine them).

Common flag values include:
| Flag | Value | Description |
| ---- | ----- | ----------- |
| `IMAGE_FILE_EXECUTABLE_IMAGE` | `0x0002` | The file is executable |
| `IMAGE_FILE_32BIT_MACHINE` | `0x0100` | The file is intended for 32-bit architecture |
| `IMAGE_FILE_DLL` | `0x2000` | The file is a DLL |
| `IMAGE_FILE_SYSTEM` | `0x1000` | System file |
| `IMAGE_FILE_RELOCS_STRIPPED` | `0x0001` | Relocation info removed |

For the purposes of this app, the correct value would be `0x0102` (32-bit Windows executables).

---

## 5. Optional Header (`IMAGE_OPTIONAL_HEADER32`)

This header provides the Windows loader with information necessary to map and run the executable in memory.  

It primarily describes:  
- The **entry point** and **base addresses** for code and data  
- **Memory layout** requirements (stack, heap, section alignment)  
- **Subsystem type** (console, GUI, driver, etc.)  
- **Version info** for the OS, image, and subsystem  
- **Data directories** pointing to key tables like imports, exports, resources, and relocations  

Despite the name “optional”, it is **required for all PE files**.

---

### 5.1 Standard Fields

---

#### Magic
**Offset:** 0x00  
**Size:** 2 bytes  
**Description:** Indicates what type of optional header follows, since its not the same in all file types. 

Common values:
| Version | Value |
| ------- | ----- |
| 32-bit (PE32) | `0x010B` |
| 64-bit (PE32+) | `0x020B` |

Since this compiler is targeting x86, the value should always be `0x010B`.

---

#### MajorLinkerVersion
**Offset:** 0x02  
**Size:** 1 byte  
**Description:** This field is purely informational, and is ignored by the Windows loader. **Can be left as 0**.

---

#### MinorLinkerVersion
**Offset:** 0x03  
**Size:** 1 byte  
**Description:** This field is purely informational, and is ignored by the Windows loader. **Can be left as 0**.

---

#### SizeOfCode
**Offset:** 0x04  
**Size:** 4 bytes  
**Description:** This field is informational. It's simply the total size (in bytes) of the **code** in the exectuable, so the sum of all individual code sections. **Can be left as 0**.

---

#### SizeOfInitializedData
**Offset:** 0x08  
**Size:** 4 bytes  
**Description:** This field is informational. It's simply the total size (in bytes) of the **initialized data** in the exectuable, so the sum of all individual initialized data sections. **Can be left as 0**.

---

#### SizeOfUninitializedData
**Offset:** 0x0C  
**Size:** 4 bytes  
**Description:** This field is informational. It's simply the total size (in bytes) of the **uninitialized data** in the exectuable, so the sum of all individual uninitialized data sections. **Can be left as 0**.

---

#### AddressOfEntryPoint
**Offset:** 0x10  
**Size:** 4 bytes  
**Description:** Relative virtual address of the programs entry point. 

For the purposes of this project, this should point to the first instruction of `main()` within the `.text` section. It sometimes points to runtime startup which precede main, but that will not be the case here.

---

#### BaseOfCode
**Offset:** 0x14  
**Size:** 4 bytes  
**Description:** Relative virtual address of the start of the code section (`.text`). Should be aligned to `SectionAlignment` (under `5.2 Windows-Specific Fields`)

---

#### BaseOfData
**Offset:** 0x18  
**Size:** 4 bytes  
**Description:** Relative virtual address of the start of the initialized data sections (`.data`, `.rdata`, etc.). Should be aligned to `SectionAlignment` (under `5.2 Windows-Specific Fields`)

---

### 5.2 Windows-Specific Fields

---

#### ImageBase
**Offset:** 0x1C  
**Size:** 4 bytes  
**Description:** The preferred virtual address for the *start* of the executable when loaded into memory. All **Relative Virtual Addresses (RVAs)** (like `AddressOfEntryPoint`) are offsets from this base.

For 32-bit executables, the standard value is `0x00400000`. If the loader can't load the file at this address (e.g., another DLL is already there), it will load it elsewhere, and the **Base Relocation Table** (if present) will be used to "fix up" any hardcoded addresses.

---

#### SectionAlignment
**Offset:** 0x20  
**Size:** 4 bytes  
**Description:** The alignment (in bytes) of sections when they are loaded into memory. This value must be a power of 2 and greater than or equal to `FileAlignment`.

The common default for 32-bit systems is `0x1000` (4096 bytes), which is the default page size for the x86 architecture. Each section's `VirtualAddress` in the section table must be a multiple of this value.

---

#### FileAlignment
**Offset:** 0x24  
**Size:** 4 bytes  
**Description:** The alignment (in bytes) of section data as it's laid out in the file on disk. This value must be a power of 2, typically `0x200` (512 bytes).

This means the `PointerToRawData` for each section in the section table must be a multiple of this value. The space between the end of one section's raw data and the start of the next is padded with `0x00` bytes to enforce this.

---

#### MajorOperatingSystemVersion
**Offset:** 0x28  
**Size:** 2 bytes  
**Description:** The major version number of the minimum required operating system. For example, for Windows XP, this would be 5. For a simple program, this can often be set to `4` or `5`.

---

#### MinorOperatingSystemVersion
**Offset:** 0x2A  
**Size:** 2 bytes  
**Description:** The minor version number of the minimum required operating system. For Windows XP, this would be 1. For a simple program, this can be set to `0`.

---

#### MajorImageVersion
**Offset:** 0x2C  
**Size:** 2 bytes  
**Description:** The major version number of *this executable file*. This is set by the developer and is purely informational. The loader doesn't care about it. **Can be left as 0**.

---

#### MinorImageVersion
**Offset:** 0x2E  
**Size:** 2 bytes  
**Description:** The minor version number of *this executable file*. Like the major version, this is informational. **Can be left as 0**.

---

#### MajorSubsystemVersion
**Offset:** 0x30  
**Size:** 2 bytes  
**Description:** The major version of the subsystem this program is built for (e.g., the Windows CUI or GUI subsystem). This should generally match the `MajorOperatingSystemVersion`. Setting it to `4` or `5` is generally safe.

---

#### MinorSubsystemVersion
**Offset:** 0x32  
**Size:** 2 bytes  
**Description:** The minor version of the subsystem. **Can be left as 0**.

---

#### Win32VersionValue
**Offset:** 0x34  
**Size:** 4 bytes  
**Description:** This field is reserved and **must be set to 0**.

---

#### SizeOfImage
**Offset:** 0x38  
**Size:** 4 bytes  
**Description:** The total size (in bytes) of the image in virtual memory. This is the distance from `ImageBase` to the end of the last section, rounded up to the nearest multiple of `SectionAlignment`.

This is a **critical field**. A wrong value will cause faulty loading.

---

#### SizeOfHeaders
**Offset:** 0x3C  
**Size:** 4 bytes  
**Description:** The combined size of all headers (DOS Header, DOS Stub, PE Signature, COFF Header, Optional Header, and the full Section Table) rounded up to the nearest multiple of `FileAlignment`.

This value tells the loader where the raw data for the *first section* begins in the file.

---

#### CheckSum
**Offset:** 0x40  
**Size:** 4 bytes  
**Description:** A checksum of the PE file. This is only required for system drivers and some system DLLs. For a user-mode executable, the loader ignores this field. **Can be left as 0**.

---

#### Subsystem
**Offset:** 0x44  
**Size:** 2 bytes  
**Description:** **This is a critical field.** It tells the operating system what kind of program this is, which determines how it's run.

For our purposes, the main values are:
| Flag | Value | Description |
| ---- | ----- | ----------- |
| `IMAGE_SUBSYSTEM_WINDOWS_GUI` | `0x0002` | A graphical (GUI) application. |
| `IMAGE_SUBSYSTEM_WINDOWS_CUI` | `0x0003` | A console (command-line) application. |

For development, **this should be set to `0x0003`**. We may want to create a flag in the future to allow `0x0002` if that becomes a need.

---

#### DllCharacteristics
**Offset:** 0x46  
**Size:** 2 bytes  
**Description:** A set of flags that specify characteristics of a DLL. While this is named for DLLs, it's also used by EXEs to opt-in to certain security features. For our purposes, it **can be left as 0**.

---

#### SizeOfStackReserve
**Offset:** 0x48  
**Size:** 4 bytes  
**Description:** The total amount of virtual memory to *reserve* for the main thread's stack, in bytes. The loader only reserves this space; it doesn't allocate physical memory for it all at once. The typical default is `0x100000` (1 MB).

---

#### SizeOfStackCommit
**Offset:** 0x4C  
**Size:** 4 bytes  
**Description:** The amount of memory (in bytes) to *initially commit* from the reserved stack space. The rest is committed on demand as the stack grows. This is typically set to the system's page size, `0x1000` (4096 bytes).

---

#### SizeOfHeapReserve
**Offset:** 0x50  
**Size:** 4 bytes  
**Description:** The total amount of virtual memory to *reserve* for the process's default heap, in bytes. This is the heap used by functions like `malloc`. The typical default is `0x100000` (1 MB).

---

#### SizeOfHeapCommit
**Offset:** 0x54  
**Size:** 4 bytes  
**Description:** The amount of memory (in bytes) to *initially commit* from the reserved heap space. The typical default is `0x1000` (4096 bytes).

---

#### LoaderFlags
**Offset:** 0x58  
**Size:** 4 bytes  
**Description:** This field is obsolete. **Must be set to 0**.

---

#### NumberOfRvaAndSizes
**Offset:** 0x5C  
**Size:** 4 bytes  
**Description:** The number of data directory entries that follow this field. **Should always be set to `0x10`**.

---

### 5.3 Data Directories (`IMAGE_DATA_DIRECTORY[16]`)

This section is not a typical header. Instead, it's an array of 16 `IMAGE_DATA_DIRECTORY` structures, one right after the other. The `NumberOfRvaAndSizes` field (which we set to `0x10`) tells the loader to read all 16 entries.

Each 8-byte entry in this array is a *pointer* to another data structure elsewhere in the file. It consists of two 4-byte fields:
* **VirtualAddress (4 bytes):** The RVA (Relative Virtual Address) of the data structure.
* **Size (4 bytes):** The total size (in bytes) of that data structure.

If an entry is not used, both its `VirtualAddress` and `Size` **should be set to 0**.

---

#### Directory Entries

---

##### Export Table
**Offset:** 0x00 (relative to DataDirectories start)  
**Size:** 8 bytes  
**Description:** Points to the **Export Directory** (`IMAGE_EXPORT_DIRECTORY`), which lists the functions and variables that *this file* provides to other executables. This is essential for DLLs, but for our purposes, it's not needed. **Can be left as 0**.

---

##### Import Table
**Offset:** 0x08  
**Size:** 8 bytes  
**Description:** **This is a critical entry.** It points to the **Import Directory** (an array of `IMAGE_IMPORT_DESCRIPTOR` structures), which is typically in the `.idata` section. This directory lists all the DLLs (like `kernel32.dll`) and functions (like `ExitProcess`) that the program needs to run.

---

##### Resource Table
**Offset:** 0x10  
**Size:** 8 bytes  
**Description:** Points to the root of the **Resource Directory** (`IMAGE_RESOURCE_DIRECTORY`), which is usually in the `.rsrc` section. This is for things like icons, cursors, string tables, and version information. **Can be left as 0**.

---

##### Exception Table
**Offset:** 0x18  
**Size:** 8 bytes  
**Description:** Points to the table of exception handling information. For 32-bit x86, this is for Structured Exception Handling (SEH). **Can be left as 0**.

---

##### Certificate Table
**Offset:** 0x20  
**Size:** 8 bytes  
**Description:** Points to the **Attribute Certificate Table**. This is for digital signatures (`Authenticode`). Unlike other entries, the `VirtualAddress` is a *file offset*, not an RVA. **Can be left as 0**.

---

##### Base Relocation Table
**Offset:** 0x28  
**Size:** 8 bytes  
**Description:** Points to the **Base Relocation Table** (usually in a `.reloc` section). This table contains a list of all the hardcoded addresses in our code (like `mov eax, 0x00401000`) that need to be "fixed" by the loader if it can't load the program at its preferred `ImageBase` (`0x00400000`).

While optional for a simple EXE, it's good practice. For simplicity, it **can be left as 0**, as long as the code is position independant or will certainly load at the preferred `ImageBase`.

---

##### Debug Directory
**Offset:** 0x30  
**Size:** 8 bytes  
**Description:** Points to an array of **Debug Directories** (`IMAGE_DEBUG_DIRECTORY`). This is used to link the executable to a debug file (like a PDB). It's not needed for the program to run. **Can be left as 0**.

---

##### Architecture
**Offset:** 0x38  
**Size:** 8 bytes  
**Description:** Reserved for architecture-specific data. **Must be 0.**

---

##### Global Pointer
**Offset:** 0x40  
**Size:** 8 bytes  
**Description:** Points to the RVA of the global pointer value, which is used by some CPU architectures (like Itanium) for optimization. For x86, this is not used. **Can be left as 0**.

---

##### TLS Table
**Offset:** 0x48  
**Size:** 8 bytes  
**Description:** Points to the **Thread Local Storage (TLS) Directory** (`IMAGE_TLS_DIRECTORY`). This is for supporting `__declspec(thread)` variables. **Can be left as 0**.

---

##### Load Configuration Table
**Offset:** 0x50  
**Size:** 8 bytes  
**Description:** Points to the **Load Configuration Directory** (`IMAGE_LOAD_CONFIG_DIRECTORY`). This contains various advanced/security-related loader settings. **Can be left as 0**.

---

##### Bound Import Table
**Offset:** 0x58  
**Size:** 8 bytes  
**Description:** Points to a table used for "bound imports," an optimization for older Windows versions that pre-links DLLs. This is now obsolete. **Can be left as 0**.

---

##### Import Address Table (IAT)
**Offset:** 0x60  
**Size:** 8 bytes  
**Description:** **This is a critical entry.** It points to the **Import Address Table** (IAT). The IAT is an array of function pointers that the loader will fill in at runtime with the *actual memory addresses* of the imported functions. Our program's `call` instructions will point to entries in this table. This is often part of the `.idata` section.

---

##### Delay Import Descriptor
**Offset:** 0x68  
**Size:** 8 bytes  
**Description:** Points to a directory of "delay-loaded" imports, which are DLLs that are only loaded when a function from them is first called. **Can be left as 0**.

---

##### COM Descriptor
**Offset:** 0x70  
**Size:** 8 bytes  
**Description:** Points to a header for .NET (COM) information. **Must be 0.**

---

##### Reserved
**Offset:** 0x78  
**Size:** 8 bytes  
**Description:** Reserved for future use. **Must be 0.**
