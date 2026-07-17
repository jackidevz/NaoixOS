# NaoixOS — Bare-Metal x86 Hobby Kernel

![License](https://img.shields.io/badge/license-GPL--2.0-blue)




![Version](https://img.shields.io/badge/version-v0.6.8--rc1-orange)




![Tested](https://img.shields.io/badge/tested-QEMU%20only-red)




![Platform](https://img.shields.io/badge/arch-x86%20(i686)-lightgrey)


![Status](https://img.shields.io/badge/status-experimental-yellow)

**Version:** `v0.6.8-rc1`

**Embedded Shell:** NaoixSH `v0.2.3-patchV1`

**Developer:** Jack (Khalid Oumouh)

**Read Arabic version ![here](README-ar.md)**

---

## ⚠️ Important Warnings (Read Before Anything Else)

> **This project is fully experimental and is a hobby project.**

- **This kernel has only ever been tested on virtual/emulated CPUs (QEMU)**, across a range of models. **It has never been tested on any real hardware.**
- **I take no responsibility whatsoever for any failure, damage, or malfunction of your real hardware** if you attempt to boot NaoixOS on it (via USB, floppy, or any real boot method). Trying it on real hardware is **entirely at your own risk**.
- **I only work on this project in my free time** (I'm a high school student preparing for my baccalaureate), so **do not expect stable or regular updates**.
- **This release is incomplete** — it is a release candidate (`-rc1`).
- **There is no memory management yet** — no paging, no real heap allocator, no memory protection.
- **The generated ISO is not guaranteed to work**, since I have never actually tested it myself. Use it at your own risk.
- **THE MOST IMPORTANT THING: THERE IS NO WAY TO SHUT IT DOWN**

> **Read it again if you did not understand**
If you've read and understood the risks, feel free to continue 🙂

---

## What is NaoixOS?

NaoixOS is a minimal x86 kernel, built from scratch as a learning/hobby project to understand how operating systems work at a very low level (bare-metal). It's developed and cross-compiled entirely from an Android phone via Termux, using Clang/LLD targeting `i686-elf`.

## Current Features

- **Dual boot path:**
  - Multiboot2 via GRUB (`make iso` / `make run-grub`)
  - Custom legacy bootloader (stage1 + stage2) for environments like the Limbo ARM emulator (`make img`)
- **GDT (Global Descriptor Table)** — basic segmentation setup.
- **IDT (Interrupt Descriptor Table) + ISRs** — interrupt and exception handling.
- **PIC (Programmable Interrupt Controller)** — remapping of hardware interrupts.
- **VGA text-mode driver** — 80x25 mode, with color support.
- **PS/2 keyboard driver** — auto-detects Set 1 / Set 2 scancodes.
- **Serial port driver** — for debugging over COM1.
- **Built-in interactive shell called NaoixSH**, with commands such as:
  - `help`, `about`, `clear`, `ver`, `colors`
  - Hidden easter eggs (like `hidden-easter-egg` and a developer mode `jackmasterdev`)
- **A minimal string library (`kstring`)** — basic string functions for the freestanding environment.

## What's Missing

- No memory management (no paging, no real malloc/heap).
- No filesystem support.
- No multitasking / scheduler.
- No real storage device drivers.
- A confirmed `CMOV` instruction failure on virtual 486/Pentium CPUs, left intentionally unfixed for now (the kernel targets newer CPUs).
- No shutdown/reboot command or button (no ACPI support)

## Build & Run

Requires `clang` + `ld.lld` (or `gcc` as a fallback), `qemu-system-i386` for testing, and `grub-mkrescue` to build an ISO.

```bash
# Build the Multiboot2 kernel ELF
make

# Run directly in QEMU (no ISO)
make run

# Build a bootable ISO via GRUB
make iso

# Run the ISO in QEMU
make run-grub

# Build the legacy floppy image (for Limbo ARM emulator / SeaBIOS)
make img
make run-img
```

**Reminder:** the `make iso` target produces an ISO that **has not actually been tested on any device, or even extensively tested at all**. Use `make run` or `make run-grub` inside QEMU as the safe, recommended way to try NaoixOS.

## Project Structure

```
naoixos/
├── arch/       # GDT, IDT, ISR, PIC
├── boot/       # Bootloaders (Multiboot2 + legacy stage1/stage2)
├── drivers/    # VGA, PS/2 keyboard, serial
├── kernel/     # Kernel entry / main
├── shell/      # NaoixSH interactive shell
├── lib/        # Minimal freestanding libc-like helpers (kstring)
├── include/    # Headers
├── linker.ld / linker-legacy.ld
└── Makefile
```

## License

This project is licensed under the **GNU General Public License v2.0 (GPL-2.0)**.

In short, this means:
- You're free to use, study, and modify the code.
- If you distribute a modified version, **it must remain open source** under the same license.
- Please credit the original project when reusing or building upon it.

See the `LICENSE` file for the full text.

## Final Disclaimer

This software is provided "AS IS", without warranty of any kind, express or implied. The developer is not responsible for any damage, data loss, or hardware malfunction resulting from using or attempting to run this kernel, especially on real hardware it has never been tested on.

---

*NaoixOS - By Jack (Khalid)*
