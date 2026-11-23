// license:BSD-3-Clause
// copyright-holders:emu-proteus-xr-MiSTer contributors
/*
    Proteus 2000 user flash SIMM: the Intel/Sharp command subset OS 2.26 uses on sound-memory flash
    (docs/p2k-findings.md "User flash SIMMs"; docs/p2k-mame-plan.md step 5). The MAME research machine
    uses it; it is also the contract for the core's writable SIMM slots.

    The OS reaches sound memory through the G20 ports of voice 0: port B (+0x34/+0x36 word address,
    +0x1E data) writes, port A (+0x30/+0x32, +0x1C) reads, one discarded read after each address write.
    Latch 0xB00000 bit 6 selects the Baby device (Proteus library) instead of Big (samples).

    Devices of a flash slot [O: OS device table 0x0102E0CE, open calls 0x100DA68 / 0x100EF00]:
      Baby  one x16 chip, maker 0x00B0, device 0x00D0, 1 M words, 32 K-word blocks, word program
      Big   four x16 chips in series, maker 0x0089, device 0x0015, 4 M words each, 64 K-word blocks,
            buffered program (0xE8); chip = word address bits 23-22
    Commands (low byte of the data; the OS writes them doubled, 0x4040): 0x40/0x10 program word,
    0x20 + 0xD0 erase block, 0xE8 + count + data + 0xD0 buffered program, 0x50 clear status,
    0x70 read status, 0x90 read ID, 0xFF read array; anything else (the JEDEC unlock writes 0xAA/0x55,
    0xF0) returns to read array. Program and erase complete at once: status 0x0080 (ready, no error);
    the OS only polls. Programming ANDs the data into the array, erase sets a block to 0xFFFF.
*/
#ifndef P2K_FLASH_H
#define P2K_FLASH_H

#include <cstdint>

namespace p2k {

class intel_flash
{
public:
	// mem: the chip's array (words); offsets passed to read/write are word offsets within the chip
	void configure(uint16_t *mem, uint32_t words, uint16_t maker, uint16_t device, uint32_t block_words, bool buffered)
	{
		m_mem = mem;
		m_words = words;
		m_maker = maker;
		m_device = device;
		m_block = block_words;
		m_buffered = buffered;
		m_mode = READ_ARRAY;
	}

	bool dirty() const { return m_dirty; }
	void clear_dirty() { m_dirty = false; }

	uint16_t read(uint32_t a) const
	{
		switch (m_mode)
		{
		case READ_ID:
			return (a & 1) ? m_device : m_maker;
		case READ_STATUS:
		case PROGRAM:
		case ERASE_SETUP:
		case BUF_COUNT:
		case BUF_DATA:
		case BUF_CONFIRM:
			return 0x0080;                 // SR / XSR bit 7: ready, no error bits
		default:
			return a < m_words ? m_mem[a] : 0xffff;
		}
	}

	void write(uint32_t a, uint16_t data)
	{
		const uint8_t cmd = data & 0xff;
		switch (m_mode)
		{
		case PROGRAM:
			program(a, data);
			m_mode = READ_STATUS;
			return;
		case ERASE_SETUP:
			if (cmd == 0xd0)
			{
				const uint32_t base = a - a % m_block;
				for (uint32_t i = 0; i < m_block && base + i < m_words; i++)
					m_mem[base + i] = 0xffff;
				m_dirty = true;
			}
			m_mode = READ_STATUS;
			return;
		case BUF_COUNT:
			m_count = (data & 0xff) + 1;
			m_got = 0;
			m_mode = BUF_DATA;
			return;
		case BUF_DATA:
			if (m_got < 32)
			{
				m_buf_addr[m_got] = a;
				m_buf[m_got] = data;
			}
			if (++m_got >= m_count)
				m_mode = BUF_CONFIRM;
			return;
		case BUF_CONFIRM:
			if (cmd == 0xd0)
				for (unsigned i = 0; i < m_count && i < 32; i++)
					program(m_buf_addr[i], m_buf[i]);
			m_mode = READ_STATUS;
			return;
		default:
			break;
		}
		switch (cmd)
		{
		case 0x40: case 0x10: m_mode = PROGRAM; break;
		case 0x20: m_mode = ERASE_SETUP; break;
		case 0xe8: m_mode = m_buffered ? BUF_COUNT : READ_ARRAY; break;
		case 0x70: m_mode = READ_STATUS; break;
		case 0x50: break;                      // clear status: no error bits are ever set
		case 0x90: m_mode = READ_ID; break;
		default: m_mode = READ_ARRAY; break;   // 0xFF, and the AMD-style 0xAA/0x55/0xF0
		}
	}

private:
	enum mode { READ_ARRAY, READ_ID, READ_STATUS, PROGRAM, ERASE_SETUP, BUF_COUNT, BUF_DATA, BUF_CONFIRM };

	void program(uint32_t a, uint16_t data)
	{
		if (a < m_words)
		{
			m_mem[a] &= data;
			m_dirty = true;
		}
	}

	uint16_t *m_mem = nullptr;
	uint32_t m_words = 0, m_block = 1;
	uint16_t m_maker = 0, m_device = 0;
	bool m_buffered = false, m_dirty = false;
	mode m_mode = READ_ARRAY;
	unsigned m_count = 0, m_got = 0;
	uint32_t m_buf_addr[32]{};
	uint16_t m_buf[32]{};
};

} // namespace p2k

#endif // P2K_FLASH_H
