/*
 * astron.h
 *
 * Copyright (C) 2001-2005 Mark Broadhead
 *
 * This file is part of DAPHNE, a laserdisc arcade game emulator
 *
 * DAPHNE is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * DAPHNE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


//astron.h

#ifndef ASTRON_H
#define ASTRON_H

#include <stdint.h>
#include "game.h"

#define ASTRON_OVERLAY_W 256	// width of overlay
#define ASTRON_OVERLAY_H 256 // height of overlay

#define ASTRON_COLOR_COUNT 256

#define BASE_VID_MEM_ADDRESS 0xf000
#define BASE_SPRITE_MEM_ADDRESS 0xc000

// START modified Mame code
#define SPR_Y_TOP		0
#define SPR_Y_BOTTOM	1
#define SPR_X_LO		2
#define SPR_X_HI		3
#define SPR_SKIP_LO		4
#define SPR_SKIP_HI		5
#define SPR_GFXOFS_LO	6
#define SPR_GFXOFS_HI	7
// END modified Mame code

enum
{
	S_AB_PLAYER_SHIP, S_AB_PLAYER_FIRE, S_AB_ENEMY_FIRE, S_AB_ALARM1, S_AB_ALARM2, S_AB_ALARM3, S_AB_ALARM4
};

enum
{	
	S_GR_FIRE, S_GR_ENEMY_CANNON, S_GR_ENEMY_MINION, S_GR_ENEMY_ATTACK, S_GR_ALARM1, S_GR_ALARM2, S_GR_ALARM3, S_GR_ALARM4
};

class astron : public game
{
public:
	astron();
	void do_irq(unsigned int);		// does an IRQ tick
	void do_nmi();		// does an NMI tick
	uint8_t cpu_mem_read(uint16_t addr);			// memory read routine
	void cpu_mem_write(uint16_t addr, uint8_t value);		// memory write routine
	uint8_t port_read(uint16_t port);		// read from port
	void port_write(uint16_t port, uint8_t value);		// write to a port
	virtual void input_enable(uint8_t);
	virtual void input_disable(uint8_t);
	void video_repaint();	// function to repaint video
	void palette_calculate();
	bool set_bank(uint8_t, uint8_t);
	virtual void write_ldp(uint8_t, uint16_t);
	virtual uint8_t read_ldp(uint16_t);
   unsigned get_libretro_button_map(unsigned id);
   const char *get_libretro_button_name(unsigned id);
protected:
	int current_bank;
	void recalc_palette();
	void draw_sprite(int);
	uint8_t rombank[0x8000];	
	uint8_t character[0x1000];	
	uint8_t sprite[0x10000];	
	uint8_t bankprom[0x200];
	uint8_t miscprom[0x240];
	SDL_Color palette_lookup[4096];		// all possible color entries
	uint8_t m_transparent_color;	// which color is to be transparent
	bool palette_modified;		// has our palette been modified?
	bool compress_palette; // can we compress the palette into one 8 bit surface?
	bool used_sprite_color[256];
	uint8_t mapped_tile_color[256];
	uint8_t ldp_output_latch;	// holds data to be sent to the LDV1000
	uint8_t ldp_input_latch;	// holds data that was retrieved from the LDV1000

	uint8_t banks[4];				// astron's banks
		// bank 1 is switches
		// bank 2 is switches
		// bank 3 is dip switch 1
		// bank 4 is dip switch 2
};

class astronh : public astron
{
public:
	astronh();
	void astronh_nmi();		// clocks the 8251
	void do_nmi();		// does an NMI tick
	void write_ldp(uint8_t, uint16_t);
	uint8_t read_ldp(uint16_t);
private:
	void write_8251_data(uint8_t);
	uint8_t read_8251_data(void);
	void write_8251_control(uint8_t);
	uint8_t read_8251_status(void);
	bool transmit_enable;
	bool recieve_enable;
	void clock_8251(void);
	bool rxrdy;
	bool txrdy;
};

class galaxy : public astronh
{
public:
	galaxy();
};

class galaxyp : public astron
{
public:
	galaxyp();
};

class blazer : public astron
{
public:
	blazer();
};

class cobraab : public astronh
{
public:
	cobraab();
	void input_enable(uint8_t);
	void input_disable(uint8_t);
	void patch_roms();
};

#endif
