#ifndef _GLOBALS_H
#define _GLOBALS_H

#define RTM_28MHZ				0x03

#define WFRAMES					4

#define MMU_AUDIO				MMU_C000
#define PAGE_AUDIO				14

#define MMU_SFX0				MMU_4000
#define PAGE_SFX0				32

#define MMU_MUSIC0				MMU_4000
#define PAGE_MUSIC0				33

#define MMU_MUSIC1				MMU_4000
#define PAGE_MUSIC1				34

#define printCls() printf("%c", 12)
#define printAt(col, row, str) printf("\x16%c%c%s", (col), (row), (str))

extern void breakpoint();

#endif
