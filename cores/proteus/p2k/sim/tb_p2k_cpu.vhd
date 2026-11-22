-- Proteus 2000 CPU testbench (NVC): TG68K (68020 mode) + flash + DRAM + I/O stubs that mirror the
-- MAME research machine (third_party/mame/src/mame/emusys/emup2k.cpp). Writes a trace of every data
-- access (opcode fetches excluded) for comparison with the MAME bus trace (P2K_BUSTRACE):
--   <R|W> <word address hex> <data hex> <byte lanes: 3 = word, 2 = upper, 1 = lower> <PC hex>
-- Generics: FLASH_HEX (one 16-bit word per line, tools/p2k_flash_image.py --hex), TRACE file,
-- CYCLES to run. MBAR peripherals are a register file here (to be replaced by the MCF5206e blocks).
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use std.textio.all;

entity tb_p2k_cpu is
	generic (
		FLASH_HEX : string := "flash_init.hex";
		TRACE     : string := "rtl.trace";
		CYCLES    : natural := 2000000
	);
end entity;

architecture sim of tb_p2k_cpu is
	signal clk      : std_logic := '0';
	signal nreset   : std_logic := '0';
	signal addr     : std_logic_vector(31 downto 0);
	signal dout     : std_logic_vector(15 downto 0);
	signal din      : std_logic_vector(15 downto 0);
	signal nwr, nuds, nlds : std_logic;
	signal busstate : std_logic_vector(1 downto 0);
	signal ipl      : std_logic_vector(2 downto 0) := "111";

	type word_mem is array (natural range <>) of std_logic_vector(15 downto 0);
	shared variable flash : word_mem(0 to 16#FFFFF#);          -- 2 MB
	shared variable dram  : word_mem(0 to 16#3FFFFF#);         -- 8 MB
	shared variable hmem  : word_mem(0 to 16#7FFFF#);          -- H chips 1 MB
	shared variable g20   : word_mem(0 to 16#FFF#);            -- 0x77F000-0x780FFF
	shared variable g05   : word_mem(0 to 32 * 64 - 1);        -- [index][word]
	shared variable mbar  : word_mem(0 to 16#1FF#);            -- MBAR register file (stub)
	signal g05_index : std_logic_vector(15 downto 0) := (others => '0');
	signal open_bus  : std_logic_vector(31 downto 0) := (others => '0');
	signal last_fetch : std_logic_vector(31 downto 0) := (others => '0');
	signal pc_now    : std_logic_vector(31 downto 0) := (others => '0');
	signal adc_sel   : std_logic_vector(7 downto 0) := (others => '0');
begin
	clk <= not clk after 5 ns;

	cpu : entity work.TG68KdotC_Kernel
		generic map (SR_Read => 2, VBR_Stackframe => 2, extAddr_Mode => 2, MUL_Mode => 2, DIV_Mode => 2, ColdFire => 1,
		             BitField => 2, BarrelShifter => 1, MUL_Hardware => 1)
		port map (clk => clk, nReset => nreset, clkena_in => '1', data_in => din, IPL => ipl,
		          IPL_autovector => '1', berr => '0', CPU => "11", addr_out => addr, data_write => dout,
		          nWr => nwr, nUDS => nuds, nLDS => nlds, busstate => busstate, longword => open,
		          nResetOut => open, FC => open, clr_berr => open, skipFetch => open, regin_out => open,
		          CACR_out => open, VBR_out => open, MBAR_out => open, RAMBAR_out => open);

	-- load the flash image
	process
		file f : text;
		variable l : line;
		variable w : std_logic_vector(15 downto 0);
		variable i : natural := 0;
	begin
		file_open(f, FLASH_HEX, read_mode);
		while not endfile(f) and i <= flash'high loop
			readline(f, l);
			hread(l, w);
			flash(i) := w;
			i := i + 1;
		end loop;
		file_close(f);
		for k in mbar'range loop
			mbar(k) := (others => '0');
		end loop;
		mbar(16#6E# / 2) := x"3C1F";   -- CSCR0 reset value (PS1/PS0/AA from the IRQ pins: none)
		wait for 100 ns;
		nreset <= '1';
		wait;
	end process;

	-- combinational read data
	process (addr, busstate, g05_index, open_bus, last_fetch, adc_sel)
		variable a : unsigned(31 downto 0);
		variable v : std_logic_vector(15 downto 0);
	begin
		a := unsigned(addr);
		v := x"FFFF";
		if a < x"00200000" then
			v := flash(to_integer(a(20 downto 1)));
		elsif a >= x"02000000" and a < x"02200000" then
			v := flash(to_integer(a(20 downto 1)));
		elsif a >= x"01000000" and a < x"01800000" then
			v := dram(to_integer(a(22 downto 1)));
		elsif a >= x"20000000" and a < x"20000400" then
			v := mbar(to_integer(a(9 downto 1)));
		elsif a = x"00400000" then
			v := x"0000";                                       -- LCD: never busy
		elsif a >= x"00500000" and a < x"00600000" then
			v := hmem(to_integer(a(19 downto 1)));
		elsif a >= x"00600000" and a < x"00600080" then
			if a(6 downto 0) = "0001110" then
				v := g05_index;
			elsif a(6 downto 0) = "0011000" and g05_index(4 downto 0) = "11110" then
				v := x"0030";                                   -- G05 chip ID
			elsif a(6 downto 0) = "0011000" and g05_index(4 downto 0) = "11011" then
				v := x"0000";                                   -- encoder delta
			else
				v := g05(to_integer(unsigned(g05_index(4 downto 0))) * 64 + to_integer(a(6 downto 1)));
			end if;
		elsif a >= x"0077F000" and a < x"00781000" then
			v := g20(to_integer(a(12 downto 1)));
			if a(5 downto 1) = "01110" or a(5 downto 1) = "01111" then
				v := x"0000";                                   -- sound-memory read ports (no SIMM)
			end if;
		elsif a = x"00800000" then
			v := x"FFFF";
			if unsigned(last_fetch) >= x"02000000" then
				v(7) := '0';                                    -- boot-block bit-7 workaround (as MAME)
			end if;
		elsif a = x"00C00000" then
			if adc_sel(2 downto 0) = "010" then
				v := x"FFFF";                                   -- master volume full
			else
				v := x"0000";
			end if;
		elsif a = x"00F00000" then
			v := x"0000";                                       -- board ID
		elsif a >= x"50000000" and a < x"54000000" then
			if a(1) = '0' then
				v := open_bus(31 downto 16);
			else
				v := open_bus(15 downto 0);
			end if;
		end if;
		din <= v;
	end process;

	-- writes, trace
	process (clk)
		file tf : text open write_mode is TRACE;
		variable l : line;
		variable a : unsigned(31 downto 0);
		variable lanes : integer;
		variable cyc : natural := 0;
		impure function merge(old : std_logic_vector(15 downto 0)) return std_logic_vector is
			variable r : std_logic_vector(15 downto 0) := old;
		begin
			if nuds = '0' then r(15 downto 8) := dout(15 downto 8); end if;
			if nlds = '0' then r(7 downto 0) := dout(7 downto 0); end if;
			return r;
		end function;
		variable i : natural;
	begin
		if rising_edge(clk) and nreset = '1' then
			cyc := cyc + 1;
			if cyc = CYCLES then
				report "cycle limit" severity note;
				std.env.finish;
			end if;
			a := unsigned(addr);
			if busstate = "00" then
				last_fetch <= addr;
			end if;
			if busstate = "10" or busstate = "11" then
				lanes := 0;
				if nuds = '0' then lanes := lanes + 2; end if;
				if nlds = '0' then lanes := lanes + 1; end if;
				if busstate = "10" then write(l, string'("R ")); else write(l, string'("W ")); end if;
				hwrite(l, std_logic_vector(a(31 downto 1)) & '0');
				write(l, string'(" "));
				if busstate = "10" then hwrite(l, din); else hwrite(l, dout); end if;
				write(l, string'(" ") & integer'image(lanes) & " ");
				hwrite(l, last_fetch);
				writeline(tf, l);
			end if;
			if busstate = "11" then
				if a >= x"01000000" and a < x"01800000" then
					i := to_integer(a(22 downto 1)); dram(i) := merge(dram(i));
				elsif a >= x"20000000" and a < x"20000400" then
					i := to_integer(a(9 downto 1)); mbar(i) := merge(mbar(i));
				elsif a >= x"00500000" and a < x"00600000" then
					i := to_integer(a(19 downto 1)); hmem(i) := merge(hmem(i));
				elsif a >= x"00600000" and a < x"00600080" then
					if a(6 downto 0) = "0001110" then
						g05_index <= dout;
					else
						i := to_integer(unsigned(g05_index(4 downto 0))) * 64 + to_integer(a(6 downto 1)); g05(i) := merge(g05(i));
					end if;
				elsif a >= x"0077F000" and a < x"00781000" then
					i := to_integer(a(12 downto 1)); g20(i) := merge(g20(i));
				elsif a = x"00C00000" then
					adc_sel <= dout(15 downto 8);
				elsif a >= x"50000000" and a < x"54000000" then
					if a(1) = '0' then open_bus(31 downto 16) <= dout; else open_bus(15 downto 0) <= dout; end if;
				end if;
			end if;
		end if;
	end process;
end architecture;
