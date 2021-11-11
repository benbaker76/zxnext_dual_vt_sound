#include <arch/zxn.h>
#include <arch/zxn/color.h>
#include <arch/zxn/esxdos.h>
#include <errno.h>
#include <input.h>
#include <z80.h>
#include <intrinsic.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "soundbank.h"
#include "bank.h"
#include "sound.h"
#include "globals.h"
#include "dma.h"

unsigned char m_tick;
unsigned long m_milliseconds;
unsigned char m_timer;

static void hardware_init(void)
{
	// Make sure the Spectrum ROM is paged in initially.
	//IO_7FFD = IO_7FFD_ROM0;

	// Put Z80 in 28 MHz turbo mode.
	ZXN_NEXTREG(REG_TURBO_MODE, RTM_28MHZ);

	// Disable RAM memory contention.
	ZXN_NEXTREGA(REG_PERIPHERAL_3, ZXN_READ_REG(REG_PERIPHERAL_3) | RP3_DISABLE_CONTENTION | RP3_ENABLE_TURBOSOUND);
}

static void wait_key(void)
{
	in_wait_nokey();
	in_wait_key();
	in_wait_nokey();
}

IM2_DEFINE_ISR_8080(isr)
{
	// update the clock
	++m_tick;
	
	m_milliseconds += 20; // 50 Hz
}

static void wait(void)
{
	while (abs(m_tick - m_timer) < WFRAMES)
		intrinsic_halt();

	m_timer = m_tick;
}

static void isr_init(void)
{
	// Set up IM2 interrupt service routine:
	// Put Z80 in IM2 mode with a 257-byte interrupt vector table located
	// at 0x6000 (before CRT_ORG_CODE) filled with 0x61 bytes. Install an
	// empty interrupt service routine at the interrupt service routine
	// entry at address 0x8181.

	intrinsic_di();
	im2_init((void *)0x8000);
	memset((void *)0x8000, 0x81, 257);

	z80_bpoke(0x8181, 0xc3);
	z80_wpoke(0x8182, (unsigned int)isr);
	intrinsic_ei();
}

static void background_create(void)
{
	zx_border(INK_WHITE);
	zx_cls(INK_BLACK | PAPER_WHITE);
}

int main(void)
{
	int key = 0;
	int effect_index = 0;

	hardware_init();
	background_create();

    printCls();
    printAt(8, 10, "Enjoy the music!\n");
    printAt(3, 12, "Press any key to cycle sfx\n");

	isr_init();
	music_init();
	sfx_init();
	audio_isr_init();

	while (true)
	{
        if (in_inkey() != 0)
        {
            in_wait_nokey();

            sfx_play(effect_index);

            if (++effect_index == 6)
                effect_index = 0;
        }
	}
	
	// Trig a soft reset. The Next hardware registers and I/O ports will be reset by NextZXOS after a soft reset.
	ZXN_NEXTREG(REG_RESET, RR_SOFT_RESET);

	return 0;
}

//****************************