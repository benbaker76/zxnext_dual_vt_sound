SECTION BANK_14

EXTERN AYFX_INIT
EXTERN AYFX_PLAY
EXTERN AYFX_FRAME
EXTERN VT_START

PUBLIC _afx_init
PUBLIC _afx_play
PUBLIC _afx_frame
PUBLIC _afx_play_isr
PUBLIC _afx_set_play_isr_enabled

DEFC AY_SFX         = 1
DEFC AY_SFX2        = 2
DEFC AY_MUSIC1      = 3 ; Main music channel (in game)
DEFC AY_MUSIC2      = 2

DEFC _IO_AY_REG     = 0xfffd
DEFC VT_PLAY = VT_START + 5

_silence_ay:
    ld hl,$FFBF                 ;initialize AY
    ld e,$15
silenceAyInit:
    dec e
    ld b,h
    out (c),e                   ;AY_SOUND_CONTROL_PORT
    ld b,l
    out (c),d                   ;AY_REGISTER_WRITE_PORT
    jr nz,silenceAyInit
    ret

_afx_init:
    ; hl contains afx bank address
    di
    push af
    push bc
    push de
    push hl
    push ix
    call AYFX_INIT
    pop ix
    pop hl
    pop de
    pop bc
    pop af
    ei
    ret

_afx_play:
    di
    push af
    push bc
    push de
    push hl
    push ix
    ld a,l
    call AYFX_PLAY
    pop ix
    pop hl
    pop de
    pop bc
    pop af
    ei
    ret

_afx_frame:
    di
    push af
    push bc
    push de
    push hl
    push ix
    call AYFX_FRAME
    pop ix
    pop hl
    pop de
    pop bc
    pop af
    ei
    ret

_afx_play_isr:
    di
    push af
    push bc
    push de
    push hl
    ex af,af'
    exx
    push af
    push bc
    push de
    push hl
    push ix
    push iy

play:

    call AYFX_FRAME

end:
    pop iy
    pop ix
    pop hl
    pop de
    pop bc
    pop af
    exx
    ex af,af'
    pop hl
    pop de
    pop bc
    pop af
    ei
    reti

_afx_set_play_isr_enabled:
    ; l contains enablement/disablement parameter
    ld a,l
    ld (_afx_play_isr_enabled),a
    ret

_afx_play_isr_enabled:
    DEFB 1

_frame_counter:
    DEFB 0
