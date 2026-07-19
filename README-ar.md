# NaoixOS — Bare-Metal x86 Hobby Kernel

![License](https://img.shields.io/badge/license-GPL--2.0-blue)




![Version](https://img.shields.io/badge/version-v0.6.8--rc1-orange)




![Tested](https://img.shields.io/badge/tested-QEMU%20only-red)




![Platform](https://img.shields.io/badge/arch-x86%20(i686)-lightgrey)



![Status](https://img.shields.io/badge/status-experimental-yellow)

**الإصدار (Version):** `v0.6.8-rc1`

**الشِل المدمج (Embedded Shell):** NaoixSH `v0.2.3-patchV2`

**المطوّر (Developer):** Jack (Khalid Oumouh)

**يمكنك قراءة التوثيق بلغات متعددة [هنا](#documentation)**

---

## ⚠️ تحذيرات هامة قبل أي شيء (Read Before Anything Else)

> **هذا المشروع تجريبي بالكامل وهو مشروع هواية.**

- **لم يتم تجربة هذه النواة إلا على معالجات افتراضية (QEMU)** بأنواع وموديلات مختلفة. **لم تُختبر إطلاقاً على أي جهاز حقيقي (real hardware).**
- **أنا لا أتحمّل أي مسؤولية عن أي فشل أو عطل أو ضرر قد يلحق بجهازك الحقيقي** في حال قمت بتجربة تشغيل NaoixOS عليه (سواء عبر فلاشة USB، قرص مرن، أو أي وسيلة إقلاع حقيقية). التجربة على جهاز حقيقي تكون **على مسؤوليتك الشخصية بالكامل**.
- **أطوّر هذا المشروع فقط في وقت فراغي** (أنا طالب في الثانوية أحضّر للباكالوريا)، لذا **لا تتوقع تحديثات مستقرة أو منتظمة**.
- **هذا الإصدار غير مكتمل (incomplete)** وهو إصدار تجريبي (`-rc1`).
- **لا توجد إدارة ذاكرة (memory management) بعد** — لا paging، لا heap allocator، لا حماية للذاكرة.
- **ملف الـ ISO الناتج من `make iso` غير مضمون** لأنني لم أجرّبه شخصياً بنفسي حتى الآن. استخدمه على مسؤوليتك.
- **اهم تحذير المرجو قرائته بحذر: لا توجد اي طريقة لإيقاف التشغيل/اعادة التشغيل**

> **اقرا مجددا لتفهم المخاطر**

إذا كنت تقرأ هذا وتفهم المخاطر، تفضّل بالمتابعة 🙂

---

## ما هو NaoixOS؟ (What is NaoixOS?)

NaoixOS هي نواة (kernel) بدائية لمعمارية x86، مبنية من الصفر كمشروع تعليمي/هواية لفهم كيفية عمل أنظمة التشغيل على مستوى منخفض جداً (low-level / bare-metal). يتم تطويرها وتصريفها (cross-compiled) من جهاز Android عبر Termux، باستخدام Clang/LLD مستهدفة `i686-elf`.

## ما الذي تفعله NaoixOS حالياً؟ (Current Features)

- **إقلاع مزدوج (dual boot path):**
  - Multiboot2 عبر GRUB (`make iso` / `make run-grub`)
  - Bootloader قديم مخصص (legacy stage1 + stage2) لبيئات مثل Limbo ARM emulator (`make img`)
- **GDT (Global Descriptor Table)** — إعداد أساسي للـ segmentation.
- **IDT (Interrupt Descriptor Table) + ISRs** — معالجة المقاطعات (interrupts) والاستثناءات (exceptions).
- **PIC (Programmable Interrupt Controller)** — إعادة توجيه (remapping) للمقاطعات الأساسية.
- **تعريف شاشة VGA نصية (VGA text-mode driver)** — وضع 80x25، مع ألوان.
- **تعريف لوحة مفاتيح PS/2** — يدعم كشف تلقائي بين Set 1 و Set 2.
- **منفذ تسلسلي (Serial port driver)** — للتصحيح (debugging) عبر COM1.
- **شِل تفاعلي مدمج اسمه NaoixSH** — يحتوي أوامر مثل:
- `help`, `about`, `clear`, `ver`, `colors`
- Easter eggs مخفية (مثل `hidden-easter-egg` ووضع المطوّر `jackmasterdev`)
- **مكتبة أساسية (`kstring`)** — دوال string بسيطة داخل بيئة freestanding.

## ما الذي لا تفعله بعد؟ (What's Missing)

- لا إدارة ذاكرة (لا paging، لا malloc/heap حقيقي).
- لا نظام ملفات (filesystem).
- لا معالجة متعددة (multitasking / scheduler).
- لا دعم لأي أجهزة تخزين حقيقية (disk drivers).
- تم تأكيد فشل تعليمة `CMOV` على معالجات 486/Pentium الافتراضية، وتُرك هذا الأمر عمداً غير مُصلح حالياً (النواة تستهدف معالجات أحدث).

## سجل التغييرات (changelog)

- حاليا سجل التغييرات مكتوب بالانجليزية فقط، لقراءته، اضغط ![هنا](CHANGELOG.md)

## البناء والتشغيل (Build & Run)

يتطلب المشروع `clang` + `ld.lld` (أو `gcc` كبديل)، و `qemu-system-i386` للتجربة، و `grub-mkrescue` لصناعة ISO.

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

**تذكير:** خيار `make iso` ينتج ملف ISO **لم يتم اختباره فعلياً على أي جهاز أو حتى بشكل موسّع**. استخدم `make run` أو `make run-grub` داخل QEMU كطريقة التجربة الآمنة والموصى بها.

## البنية (Project Structure)

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

## Documentation
### التوثيق

- ![Arabic](README-ar.md)
- ![English](README.md)

## الحقوق والترخيص (License)

هذا المشروع مرخّص تحت رخصة **GNU General Public License v2.0 (GPL-2.0)**.

باختصار، هذا يعني:
- يمكنك استخدام، دراسة، وتعديل الكود بحرية.
- إذا وزّعت نسخة معدّلة، **يجب أن تبقى مفتوحة المصدر** تحت نفس الرخصة.
- يُرجى نسبة الفضل الأصلي للمشروع عند إعادة الاستخدام أو الاشتقاق منه.

راجع ملف `LICENSE` للنص الكامل.

## إخلاء المسؤولية النهائي (Final Disclaimer)

هذا البرنامج مقدَّم "كما هو" (AS IS) دون أي ضمان من أي نوع، صريح أو ضمني. المطوّر غير مسؤول عن أي ضرر، فقدان بيانات، أو عطل في العتاد (hardware) ينتج عن استخدام أو تجربة هذه النواة، خصوصاً على أجهزة حقيقية لم يتم اختبارها عليها إطلاقاً.

---

*NaoixOS — بصنع Jack (Khalid)*
