# ModRetro M64 board documents

Downloaded 2026-11-22. The links come from the ModRetro support knowledge base
(https://support.modretro.com/). modretro.com itself returns a Cloudflare challenge (HTTP 403) to curl.

| File | Source |
|---|---|
| `M64_MLB_SCH.pdf` (main board schematic, 31 pp, rev 8/12/2026, P/N 100-0610) | https://cdn.shopify.com/s/files/1/0829/2034/1806/files/M64_MLB_SCH.pdf |
| `M64_MLB_ASY.pdf` | https://cdn.shopify.com/s/files/1/0829/2034/1806/files/M64_MLB_ASY.pdf |
| `M64_CART_SCH.pdf` | https://cdn.shopify.com/s/files/1/0829/2034/1806/files/M64_CART_SCH.pdf |
| `M64_CART_ASY.pdf` | https://cdn.shopify.com/s/files/1/0829/2034/1806/files/M64_CART_ASY.pdf |

The PDFs are not committed because their licence is not clear:
- The KB article "M64 Open Source Files" states no licence.
- The GitHub repo `ModRetro/oss-m64-console` has a GPL-3.0 `LICENSE`. Its README is titled
  "M64 Console firmware", lists software as "Coming soon!", and links to the KB article for the
  hardware.

Download them again from the URLs in the table.

## Facts used by the plans

- **User / Expansion I/O, J25 (sheet 10, FPGA_BANK64):** Hirose FH12-16S-0.5SH(55), a 16-pin
  0.5 mm FFC connector.
    - Signals: `GPIO_BONUS_0..7` from XCAU15P bank 64, an HP bank at **1.8 V**.
    - Power: pins 1–3 carry `V_1P8_FPGA`. There is no 3.3 V or 5 V pin.
    - Ground: pins 4, 7, 10, 13 and 16.
    - The signal pins are 5, 6, 8, 9, 11, 12, 14 and 15, labelled SE50 (50 Ω single-ended).
- **HDMI (sheet 7):** the FPGA drives TMDS directly (`100_HDMI_FPGA_D*`) into an SN75DP159RGZT
  retimer (I2C address 0x5E), which drives connector J4. There is no HDMI transmitter chip, so the
  HDL has to produce the TMDS encoding and the audio data islands.
