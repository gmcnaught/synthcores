# Reference model (`ref/`, `third_party/nuked-sc55`)

The reference is jcmoyer/Nuked-SC55, a git submodule pinned at
`66d1272` (2026-10-17). `ref/CMakeLists.txt` builds it without the SDL/rtmidi
frontend:

| Target | What |
|---|---|
| `sc55_backend`, `sc55-render` | the backend as released (decoder2 on) |
| `sc55_backend_traced`, `sc55-render-traced` | a copy patched by `ref/patch_ref.py`: decoder2 off, observation hooks |

```bash
cmake -S ref -B out/ref -G Ninja -DCMAKE_BUILD_TYPE=Release
ninja -C out/ref
out/ref/sc55-render -d firmware --romset mk1-v1.21 --reset gm -o out.wav song.mid
```

## Hooks (`ref_hooks.h`)

`g_ref_hooks.bus(ctx, write, address, data)` sees every byte the MCU reads or
writes through `MCU_Read`/`MCU_Write`, in order; `g_ref_hooks.cpu_done(ctx)`
is called in `MCU_Step` after the instruction and before the devices advance;
`g_ref_hooks.error_traps` counts `MCU_ErrorTrap` calls. With no hook set the
patched build renders the same audio as the released one
(`tests/test_ref_patch.py`).

decoder2 caches decoded instructions, so its code fetches bypass `MCU_Read`.
The traced build turns it off: every code byte is then a bus read, which the
CPU lockstep bench compares one for one.

## Checks done in M0

| Check | Result |
|---|---|
| mk1-v1.21 ROMs against the romset table (`tools/romset.py check firmware`) | all five match [OBS] |
| decoder2 on vs off, avmidi 01, 05, 0A, mk1-v1.21, `--reset gm` | identical SHA-256 (`c5235208…`, `7921c51e…`, `1bc443bb…`) [OBS] |
| hooked build vs released build, avmidi 0A | identical [OBS] |
| jcmoyer's own integration hashes | mk2-v1.01 only; not runnable without mk2 ROMs [UNK] |

## Model facts used by the HDL

* One `MCU_Step` = interrupt check, one instruction (or none while asleep),
  `cycles += 12`, then PCM, timers, UART (mk1: no sub-MCU), A/D, and the mk1
  gate array's LCD counter (`mcu.cpp` `MCU_Step`) [OBS].
* The renderer runs mk1 at 600 ns per step, mk2 at 500 ns
  (`R_NSPerStep`): 20 MHz and 24 MHz reference cycles [OBS].
* Stack accesses use page 0 (`MCU_PushStack` → `MCU_Write16(mcu, r7, …)`),
  16-bit accesses are two byte accesses at `address & ~1`, high byte first
  [OBS].
