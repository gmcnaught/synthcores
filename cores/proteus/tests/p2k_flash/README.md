# Proteus 2000 user flash SIMM test

`tools/p2k_flash_test.py --simm DIR` (docs/p2k-mame-plan.md step 5). DIR holds a stand-in SIMM built
with a user flash ROM ID (106-119) and a board flash image:

```sh
# spec with "id": 106 (tests/p2k_simm_demo.json with the ID changed), built from the local samples
tools/p2k_build_simm.py spec106.json DIR
cp .../build/p2k/simm/demo/flash_clean.nv DIR/
P2K_MAME=... P2K_ROMPATH=... tools/p2k_flash_test.py --simm DIR
```

`rename_106_108.keys` drives the front panel (`P2K_KEYS`): Save/Copy, eight data-knob steps to
RENAME FLASH SIMM, six cursor-right presses to the ID field, ID +2, Enter, Enter. The test checks
that the OS writes the SIMM (Big header and Baby library), that the written images carry ID 108
with valid block checksums, and that after a restart bank MSB 108 plays a note, identically in two
runs and as reproduced by `p2k/model/p2k_render`. Passed 2026-11-23.
