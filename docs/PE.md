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