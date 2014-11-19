#ifndef __SUB_YM3812_H__
#define __SUB_YM3812_H__

#include <nall/nall.hpp>
using namespace nall;

#include "fmt/pre.ym3812.hpp"
#include "config.hpp"

#ifndef _YM3812_BNK_
#define _YM3812_BNK_
#endif

struct SubYM3812 {
	struct ins_t {
		int id;
		int file_id;
		ym3812_ch_t ch;
	};
	ins_t insts[N::OPL::Types::Total];
	ins_t instsPrev[N::OPL::Types::Total];
	vector<ins_t> insList;
	bnk_file_t bank;	// FOR BNK
	//int insListCount;
	int insListSize;
	bool dacOn;
	void init();
	void add(ins_t *ins, int file_id);
	int find(ins_t *ins);
	int slot(ins_t *ins);
	bool compare(ins_t *ins1, ins_t *ins2, bool use_ext_cmp);
	void write(int bank, int reg, int val, int f);
	unsigned group(bool use_ext_cmp);
	void process(const lstring &fl, const V2PConfiguration &cfg);
	bool save(const ins_t &ins, const string &fn, const V2PConfiguration &cfg);
	bool saveSBI(const ins_t &ins, const string &fn);
	/*bool saveVGI(const ins_t &ins, const string &fn);
	bool saveTFI(const ins_t &ins, const string &fn);
	bool saveTYI(const ins_t &ins, const string &fn);
	bool saveY12(const ins_t &ins, const string &fn);
	bool saveEIF(const ins_t &ins, const string &fn);
	bool saveDMP(const ins_t &ins, const string &fn);*/
#ifdef _YM3812_BNK_
	bool addToBank(const ins_t &ins, const string &fn, bool as_drum, uint8_t extra);
	bool saveBNK(const string &fn);
#endif
	private:
		void _log(const string &s) {print(s);}
};

#endif