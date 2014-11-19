#ifndef __W_OPT_H__
#define __W_OPT_H__

#include "../common.hpp"

struct WinOptions : Window {
	/** layouts **/
	VerticalLayout 
		lMain, 
		lTogglesOPN, lOPN2, 
		lTogglesOPM, lOPM2,
		lTogglesOPL, lOPL2;
		
	HorizontalLayout ltOPN, ltOPM, ltOPL;
	TabFrame tabMain;
	Frame frTogglesOPN, frTogglesOPM, frTogglesOPL;
	/** widgets **/
	CheckLabel opnToggles[N::OPN::Types::Total];
	CheckLabel opmToggles[N::OPM::Types::Total];
	CheckLabel oplToggles[N::OPL::Types::Total];
	
	/** constructors **/
	WinOptions() : Window() {
		init();
		reflowStatic();
		reflow();
	}
	/** proc **/
	void init() {
		setWindowGeometry({128,128,400,256});
		setTitle("Preferences");
		lMain.setMargin(5);
		
		frTogglesOPN.setText("Output Formats");
		ltOPN.setMargin(5);
		lTogglesOPN.setMargin(5);
		opnToggles[N::OPN::Types::VGI].setText("VGI");
		opnToggles[N::OPN::Types::TFI].setText("TFI");
		opnToggles[N::OPN::Types::TYI].setText("TYI");
		opnToggles[N::OPN::Types::EIF].setText("EIF");
		opnToggles[N::OPN::Types::DMP].setText("DMP");
		opnToggles[N::OPN::Types::Y12].setText("Y12");
		opnToggles[N::OPN::Types::GYB].setText("GYB");
		
		frTogglesOPM.setText("Output Formats");
		ltOPM.setMargin(5);
		lTogglesOPM.setMargin(5);
		opmToggles[N::OPM::Types::OPM].setText("OPM");
		
		frTogglesOPL.setText("Output Formats");
		ltOPL.setMargin(5);
		lTogglesOPL.setMargin(5);
		oplToggles[N::OPL::Types::SBI].setText("SBI");
		oplToggles[N::OPL::Types::BNK].setText("BNK");
		
		initProc();
	}
	void initProc();
	void reflowStatic() {
		tabMain.append("OPN");
			tabMain.setLayout(0, ltOPN);
			frTogglesOPN.setLayout(lTogglesOPN);
			ltOPN.append(frTogglesOPN, {~0,~0}, 5);
			ltOPN.append(lOPN2, {~0,~0});
		tabMain.append("OPM");
			tabMain.setLayout(1, ltOPM);
			frTogglesOPM.setLayout(lTogglesOPM);
			ltOPM.append(frTogglesOPM, {~0,~0}, 5);
			ltOPM.append(lOPM2, {~0,~0});
		tabMain.append("OPL");
			tabMain.setLayout(2, ltOPL);
			frTogglesOPL.setLayout(lTogglesOPL);
			ltOPL.append(frTogglesOPL, {~0,~0}, 5);
			ltOPL.append(lOPL2, {~0,~0});
	}
	void reflow() {
		unsigned i;
		for (i=0; i<N::OPN::Types::Total; ++i) lTogglesOPN.append(opnToggles[i], {~0,0}, 2);
		for (i=0; i<N::OPM::Types::Total; ++i) lTogglesOPM.append(opmToggles[i], {~0,0}, 2);
		for (i=0; i<N::OPL::Types::Total; ++i) lTogglesOPL.append(oplToggles[i], {~0,0}, 2);
		append(lMain);
		lMain.append(tabMain, {~0,~0});
	}
};

#endif