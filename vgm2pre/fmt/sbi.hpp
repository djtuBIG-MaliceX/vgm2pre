#ifndef __SBI_H__
#define __SBI_H__

/*
 * Sound Blaster Instrument format (.SBI)
 */

struct sbi_inst_t {
	unsigned char modChar;    // OPL reg 0x20 {Mult, KSR, EG, VIB, Tremolo flags}
	unsigned char carChar;    // OPL reg 0x23 {Mult, KSR, EG, VIB, Tremolo flags}
	unsigned char modScale;   // OPL reg 0x40 {Keyscaling and Total Level}
	unsigned char carScale;   // OPL reg 0x43 {Keyscaling and Total Level}
	unsigned char modAttDec;  // OPL reg 0x60 {Attack and Decay level}
	unsigned char carAttDec;  // OPL reg 0x63 {ATtack and Decay level}
	unsigned char modSusRel;  // OPL reg 0x80 {Sustain and Release level}
	unsigned char carSusRel;  // OPL reg 0x83 {Sustain and Release level}
	unsigned char modWaveSel; // OPL reg 0xE0 {Waveform select}
	unsigned char carWaveSel; // OPL reg 0xE3 {Waveform select}
	unsigned char fbkconn;    // OPL reg 0xC0 {Feedback and AM/FM connection}
	unsigned char _padding[5];// unused padding for 16-byte alignment
};

struct sbi_t {
	unsigned char signature[4]; // ALWAYS "SBI" + 0x1A
	unsigned char name[32];     // Instrument title with NULL terminator
	sbi_inst_t inst;            // see "struct sbi_inst_t" above
};


#endif