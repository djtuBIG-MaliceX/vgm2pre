#include "sub.ym3812.hpp"

#ifndef _YM3812_BNK_
#define _YM3812_BNK_
#endif

void SubYM3812::init() {
	unsigned i;
	for (i = 0; i<N::OPL::Types::Total; ++i) {
		memset(&insts[i], 0x00, sizeof(ins_t));
		memset(&instsPrev[i], 0xff, sizeof(ins_t));
	}
}
void SubYM3812::add(ins_t *ins, int fid) {
	ins_t i;
	memcpy(&i, ins, sizeof(ins_t));
	i.file_id = fid;
	insList.append(i);
}

int SubYM3812::find(ins_t *ins) {
	int i;
	if (!ins->ch.op[0].ar&&!ins->ch.op[1].ar) return -1;
	if (ins->ch.op[0].tl>0x7e&&ins->ch.op[1].tl>0x7e) return -1;
	for (i = 0; i<insList.size(); ++i) {
		if (!memcmp(ins, &insList[i], sizeof(ins_t))) return i;
	}
	return -1;
}

int SubYM3812::slot(ins_t *ins) {
	const int slotMap[8] = { 0x08,0x08,0x08,0x08,0x0c,0x0e,0x0e,0x0f };
	return slotMap[ins->ch.algo&7];
}

bool SubYM3812::compare(ins_t *ins1, ins_t *ins2, bool use_ext_cmp=false) {
	ym3812_op_t *op1, *op2;
	int i, sl;
	if (ins1->ch.algo!=ins2->ch.algo) return false;
	if (ins1->ch.fback!=ins2->ch.fback) return false;
	if (use_ext_cmp) {
		if (ins1->ch.pms!=ins2->ch.pms) return false;
		if (ins1->ch.ams!=ins2->ch.ams) return false;
	}
	sl = slot(ins1);
	if (!(sl&1)) if(ins1->ch.op[0].tl!=ins2->ch.op[0].tl) return false;
	if (!(sl&2)) if(ins1->ch.op[1].tl!=ins2->ch.op[1].tl) return false;
	for (i = 0; i<2; ++i) {
		op1 = &ins1->ch.op[i];
		op2 = &ins2->ch.op[i];
		if (op1->ar!=op2->ar) return false;
		if (op1->dr!=op2->dr) return false;
		if (op1->sr!=op2->sr) return false;
		if (op1->rr!=op2->rr) return false;
		if (op1->sl!=op2->sl) return false;
		if (op1->mul!=op2->mul) return false;
		if (op1->dt!=op2->dt) return false;
		if (op1->ssg!=op2->ssg) return false;
		if (op1->rs!=op2->rs) return false;
		if (use_ext_cmp&&op1->am!=op2->am) return false;
	}
	return true;
}

void SubYM3812::write(int bank, int reg, int val, int f) {
	ym3812_op_t *op;
	/*int dtTable[8]={0,1,2,3,0,-1,-2,-3};*/
	int ch;

	//if (reg==0xb3) return;	// INVALID

	/*
	if (!bank) {
		switch (reg) {
			case 0x2b: //DAC on/off
				dacOn = (val&0x80)?true:false;
				return;
			case 0x28: //key on/off
				ch = val&7;
				if (ch>3) --ch;
				if (ch==5&&dacOn) return;
				if (val&0xf0) {
					if (memcmp(&instsPrev[ch],&insts[ch],sizeof(ins_t))) {
						if (find(&insts[ch])<0) add(&insts[ch],f);
					}
					memcpy(&instsPrev[ch],&insts[ch],sizeof(ins_t));
				}
				return;
		}
	}*/

	/**** early exits { ****/
	//if (reg<0x30) return;
	//if ((reg&3)==3) return;
	/**** } ****/

	ch = (reg&3);
	op = &insts[ch].ch.op[(reg>>2)&3];
	// TODO capture registers
	/*switch (reg&0xf0) {
		case 0x20: break;	// BANK 0 registers ignored
		case 0x30:	//DT1,MUL
			op->dt = (val>>4)&7;
			op->detune = dtTable[op->dt];
			op->mul = val&0x0f;
			break;
		case 0x40:	//TL
			op->tl = val&0x7f;
			break;
		case 0x50:	//RS,AR
			op->rs = (val>>6)&3;
			op->ar = val&0x1f;
			break;
		case 0x60:	//AM,D1R
			op->am = (val>>7)&1;
			op->dr = val&0x1f;
			break;
		case 0x70:	//D2R
			op->sr = val&0x1f;
			break;
		case 0x80:	//D1L,RR
			op->sl = val>>4;
			op->rr = val&0x0f;
			break;
		case 0x90:	//SSG-EG
			op->ssg = val&0x0f;
			break;
		case 0xb0:
			if (reg<0xb3) {//FB,ALGO
				insts[ch].ch.algo = val&7;
				insts[ch].ch.fback = (val>>3)&7;
			}
			else if (reg==0xb3) break;	// UNUSED
			else if (reg<0xb7) {//PAN,PMS,AMS
				insts[ch].ch.pms = val&7;
				insts[ch].ch.ams = (val>>4)&3;
			}
			break;
		case 0xa0: break;	// YM3812 set freq
		default: break;
	}*/
}

unsigned SubYM3812::group(bool use_ext_cmp=false) {
	unsigned u = 0;
	int j, id, i, z, v, mv, sl;
	z = insList.size();
	// initialize inst list IDs
	i = z; while (--i>=0) insList[i].id = -1;
	// search for same insts w/different volume
	for (i = 0; i<z; ++i) {
		if (insList[i].id<0) {
			insList[i].id = u++;
			for (j = 0; j<z; ++j) {
				if (i==j) continue;
				if (compare(&insList[i],&insList[j],use_ext_cmp)) insList[j].id = insList[i].id;
			}
		}
	}
	// search for loudest of same insts
	for (j = 0; j<u; ++j) {
		//_log({"Grouping #",j,"..."});
		id = -1;
		mv = 0;
		for (i = 0; i<z; ++i) {
			if (insList[i].id==j) {
				sl = slot(&insList[i]);
				v = 0;
				if (sl&1) v += (127-insList[i].ch.op[0].tl);
				if (sl&2) v += (127-insList[i].ch.op[1].tl);
				if (sl&4) v += (127-insList[i].ch.op[2].tl);
				if (sl&8) v += (127-insList[i].ch.op[3].tl);
				if (v>mv) {id = i; mv = v;}
			}
		}
		if (id>=0) {
			//_log({"found ",id,"\n"});
			for (i = 0; i<z; ++i) {
				if (insList[i].id==j) {
					if (i!=id) insList[i].id = -1;
				}
			}
		}
		else _log({"No master presets found for #",j," :(\n"});
	}
	return u;
}

void SubYM3812::process(const lstring &fl, const V2PConfiguration &cfg) {
	string fn_edit;
	unsigned j,i;
	unsigned z = insList.size();
	unsigned used = group(cfg.use_ext_cmp_ym3812);
	_log({"OK: ",used," OPL instrument(s) were found\n"});
	bank.version = 2;
	bank.melody.insts.reset();
	bank.drums.insts.reset();
	bool canDumpOne = false;
	for (unsigned cd=0; cd<N::OPL::Types::BNK; ++cd) {
		if (!cfg.disabledOPL[cd]&&cfg.dumpOPL[cd]) {canDumpOne = true; break;}
	}
	for (j = 0; j<used; ++j) {
		//_log({"Looking for ins ",j,"...\n"});
		for (i = 0; i<z; ++i) {
			//_log({"ins ",i," = ",insList[i].id,"\n"});
			if (insList[i].id==j) {
				if (insList[i].file_id>fl.size()) {
					_log({"File ID ",insList[i].file_id," is out of bounds (",fl.size(),"), skipping...\n"});
					continue;
				}
				fn_edit = {basename(fl[insList[i].file_id]),"_",j};
#ifdef _YM3812_BNK_
				if (!addToBank(insList[i],notdir(fn_edit),false,0)) {
					_log({"ERR: Can't add '",fn_edit,"' to OPL bank\n"});
				}
				else _log({"OK: Added '",fn_edit,"' to OPL bank\n"});
#endif
				//_log({"Saving ",notdir(fn_edit),"..."});
				if (canDumpOne) {
					if (!save(insList[i], fn_edit, cfg)) {
						_log({"ERR: Can't save file '",fn_edit,"'\n"});
						break;
					}
				}
				//else _log({"done!\n"});
			}
		}
	}
#ifdef _YM3812_BNK_
	fn_edit = {"bank-opl-",(unsigned)time(NULL)};
	if (!cfg.disabledOPL[N::OPL::Types::BNK]&&cfg.dumpOPL[N::OPL::Types::BNK]&&bank.melody.insts.size()>0) {
		if (!saveBNK(fn_edit)) {
			_log({"ERR: Can't save OPL Bank file '",fn_edit,"' of size ",bank.melody.insts.size(),"\n"});
		}
	}
#endif
}

bool SubYM3812::save(const ins_t &ins, const string &fn, const V2PConfiguration &cfg) {
	unsigned ret = 0;
	
	if (cfg.dumpOPL[N::OPL::Types::SBI]) {++ret; if (!saveSBI(ins, fn)) --ret;}
	
	if (cfg.dumpOPL[N::OPL::Types::BNK]) {++ret; if (!saveBNK(fn)) --ret;}
	
	return ret>0;
}

#ifdef _YM3812_BNK_
bool SubYM3812::addToBank(const ins_t &ins, const string &fn, bool as_drum=false, uint8_t extra=0) {
	BNK_ins_t gins;
	gins.name = fn;
	unsigned aa = 0, pp = 0;
	uint8_t s;
	for (aa = 0; aa<4; ++aa) {gins.reg[pp++] = ins.ch.op[aa].mul|(ins.ch.op[aa].dt<<4);}
	for (aa = 0; aa<4; ++aa) {gins.reg[pp++] = ins.ch.op[aa].tl;}
	for (aa = 0; aa<4; ++aa) {gins.reg[pp++] = ins.ch.op[aa].ar|(ins.ch.op[aa].rs<<6);}
	for (aa = 0; aa<4; ++aa) {gins.reg[pp++] = ins.ch.op[aa].dr|(ins.ch.op[aa].am<<7);}
	for (aa = 0; aa<4; ++aa) {gins.reg[pp++] = ins.ch.op[aa].sr;}
	for (aa = 0; aa<4; ++aa) {gins.reg[pp++] = ins.ch.op[aa].rr|(ins.ch.op[aa].sl<<4);}
	for (aa = 0; aa<4; ++aa) {gins.reg[pp++] = ins.ch.op[aa].ssg;}
	gins.reg[pp++] = ins.ch.algo|(ins.ch.fback<<3);
	gins.reg[pp++] = ins.ch.pms|(ins.ch.ams<<4)|0xc0;
	gins.reg[pp++] = extra;	// as melody: transpose, loads as (x+128)%256; as drum: note map
	gins.reg[pp++] = 0;	// padding byte
	if (as_drum) {
		s = bank.drums.insts.size();
		if (s<256) {
			if (s<128) bank.drums.instMap[s] = s;
			bank.drums.insts.append(gins);
		}
		else return false;
	}
	else {
		s = bank.melody.insts.size();
		if (s<256) {
			if (s<128) bank.melody.instMap[s] = s;
			bank.melody.insts.append(gins);
		}
		else return false;
	}
	return true;
}
bool SubYM3812::saveBNK(const string &fn) {
	unsigned fs = 262;//3+2+2*0x80+1;	// Header + Ins Count + Mappings + Ins Data + Ins Names + CheckSum
	unsigned aa = 0, pp;
	unsigned len;
	unsigned vb = bank.version==1?0x1e:0x20;
	//for (aa = 0; aa<bank.melody.insts.size(); ++aa) {
	vector<string> tmpArr;
	for (auto &inst : bank.melody.insts) {
		//if (bank.version==1) fs += 0x1e;
		//else fs += 0x20;
		tmpArr.append(inst.name);
		len = inst.name.length(); if (len>255) len = 255;
		fs += vb+1+len;
		//fs += 1+bank.melody.insts[aa].name.length();
	}
	for (auto &inst : bank.drums.insts) {
		//if (bank.version==1) fs += 0x1e;
		//else fs += 0x20;
		tmpArr.append(inst.name);
		len = inst.name.length(); if (len>255) len = 255;
		fs += vb+1+len;
	}
	fs += 4;
	uint8_t dataBNK[fs];
	pp = 0;
	dataBNK[pp++] = 26; dataBNK[pp++] = 12;
	dataBNK[pp++] = bank.version;
	dataBNK[pp++] = bank.melody.insts.size();
	dataBNK[pp++] = bank.drums.insts.size();
	for (aa = 0; aa<128; ++aa) {
		dataBNK[pp++] = bank.melody.instMap[aa];
		dataBNK[pp++] = bank.drums.instMap[aa];
	}
	if (bank.version==2) {
		dataBNK[pp++] = bank.lfo;
	}
	//for (aa = 0; aa<bank.melody.insts.size(); ++aa) {
	for (auto &inst : bank.melody.insts) {
		if (bank.version==1) {
			memcpy(dataBNK+pp, inst.reg, 0x1d);
			dataBNK[pp+0x1d] = inst.reg[0x1e];
			pp += 0x1e;
		}
		else {
			memcpy(dataBNK+pp, inst.reg, 0x20);
			pp += 0x20;
		}
		//len = inst.name.length(); if (len>255) len = 255;
		//uint8_t tmp[len+1]; tmp[0] = len&0xff;
		//memcpy(tmp+1, (const char*)inst.name, len);
		// TODO: copy substr(inst[aa].name, 0, min(inst[aa].name.length(), 255)) into tempArr
		//tmpArr.append(inst.name);
	}
	//for (aa = 0; aa<bank.drums.insts.size(); ++aa) {
	for (auto &inst : bank.drums.insts) {
		if (bank.version==1) {
			memcpy(dataBNK+pp, inst.reg, 0x1d);
			dataBNK[pp+0x1d] = inst.reg[0x1e];
			pp += 0x1e;
		}
		else {
			memcpy(dataBNK+pp, inst.reg, 0x20);
			pp += 0x20;
		}
		//len = idrm.name.length(); if (len>255) len = 255;
		//uint8_t tmp[len+1]; tmp[0] = len&0xff;
		//memcpy(tmp+1, (const char*)idrm.name, len);
		// TODO: copy substr(inst[aa].name, 0, min(inst[aa].name.length(), 255)) into tempArr
		//tmpArr.append(idrm.name);
	}
	// TODO: append tempArr to dataBNK
	for (auto &t : tmpArr) {
		len = t.length(); if (len>255) len = 255;
		dataBNK[pp++] = len;
		memcpy(dataBNK+pp, t.data(), len);
		pp += len;
	}
	uint32_t chk = BNK_file_t::calc_checksum(pp, dataBNK);
	if (pp<fs) dataBNK[pp] = 0;
	//memcpy(dataBNK+pp, 0x0, 0x04);
	//memcpy(dataBNK+pp, chk, 0x04);
	return file::write({fn,".BNK"}, dataBNK, sizeof(dataBNK));
	//return false;
}
#endif
