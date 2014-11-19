#ifndef __BNK_H__
#define __BNK_H__

#include <nall/nall.hpp>
#include <nall/file.hpp>
#include <nall/gzip.hpp>

using namespace nall;

/*
 * Structures below stolen from MF2ROL (C) 1991 Alejandro Kapauan
 * Also referenced format info from
 * http://www.shikadi.net/moddingwiki/BNK_Format_(Ad_Lib)
 * note: some information was incomplete/incorrect on site
 */
typedef struct 
{
	unsigned char  ksl;
	unsigned char  multiple;
	unsigned char  feedback; /* used by operator 0 only */
	unsigned char  attack;
	unsigned char  sustain;
	unsigned char  eg;
	unsigned char  decay;
	unsigned char  releaseRate;
	unsigned char  totalLevel;
	unsigned char  am;
	unsigned char  vib;
	unsigned char  ksr;
	unsigned char  con;      /* used by operator 0 only */
} bnkop_t;

typedef struct 
{
	unsigned char      mode;
	unsigned char      percVoice;
	bnkop_t   op0;
	bnkop_t   op1;
	unsigned char      wave0;            /* wave form for operator 0 */
	unsigned char      wave1;            /* wave form for operator 1 */
} bnktimbre_t;

#endif