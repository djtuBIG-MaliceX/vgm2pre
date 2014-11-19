#ifndef __PRE_YM3812_H__
#define __PRE_YM3812_H__

struct ym3812_op_t {
	unsigned char mul;
	unsigned char tl;
	unsigned char rs;
	unsigned char ar;
	unsigned char am;
	unsigned char dr;
	unsigned char sr;
	unsigned char sl;
	unsigned char rr;
};

struct ym3812_ch_t {
	struct ym3812_op_t op[2];
	unsigned char algo;
	unsigned char fback;
	unsigned char vibrato;
	unsigned char tremolo;
};


#endif