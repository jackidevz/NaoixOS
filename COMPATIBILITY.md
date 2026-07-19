# CPU Compatibility

NaoixOS `v0.6.8-rc1` has been tested against the following QEMU `-cpu`
models (via the Limbo x86 PC frontend). It also boots and runs fine
with only **4 MB of RAM**.

> **Note:** All testing is QEMU/Limbo emulation only. NaoixOS has
> **never** been tested on real hardware — see the warnings in
> [README.md](README.md).

| CPU Model   | Status | CPU Model   | Status |
|-------------|:------:|-------------|:------:|
| Default     | ✅     | n270        | ✅     |
| 486         | ❌     | Nehalem     | ✅     |
| athlon      | ✅     | Opteron_G1  | ✅     |
| Broadwell   | ✅     | Opteron_G2  | ✅     |
| Conroe      | ✅     | Opteron_G3  | ✅     |
| Cooperlake  | ✅     | Opteron_G4  | ✅     |
| Core2Duo    | ✅     | Opteron_G5  | ✅     |
| CoreDuo     | ✅     | Penryn      | ✅     |
| Denverton   | ✅     | Pentium     | ❌     |
| Dhyana      | ✅     | Pentium 2   | ✅     |
| EPYC        | ✅     | Pentium 3   | ✅     |
| Haswell     | ✅     | Phenom      | ✅     |
| IvyBridge   | ✅     | Qemu32      | ✅     |
| KnightsMill | ✅     | Qemu64      | ✅     |
| kvm32       | ✅     | SandyBridge | ✅     |
| kvm64       | ✅     | Snowridge   | ✅     |
|             |        | Westmere    | ✅     |

**29 / 31 passing.**

## Known failures

- **486** and **Pentium** both fail on a confirmed missing `CMOV`
  instruction. This is a known limitation (see [README.md](README.md)
  → "What's Missing") and is **intentionally left unfixed** for now,
  since NaoixOS targets newer CPU baselines.

## RAM

NaoixOS boots and runs fine with **4 MB of RAM**, on any of the CPU
models above. There's currently no memory management, paging, or heap
allocator, so its footprint stays small.

---

*Compatibility data collected by Jack via QEMU/Limbo, 2026-07-19.*

