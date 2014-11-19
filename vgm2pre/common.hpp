#ifndef __N_COMMON_H__
#define __N_COMMON_H__

#include "fmt/tfi.hpp"
#include "fmt/tyi.hpp"
#include "fmt/ym12.hpp"
#include "fmt/gyb.hpp"

#include "fmt/sbi.hpp"
#include "fmt/bnk.hpp"

namespace N {

	namespace OPN {
		enum Types : unsigned {
			VGI = 0,
			TFI,
			TYI,
			EIF,
			DMP,
			Y12,
			GYB,
			Total
		};
	}

	namespace OPM {
		enum Types : unsigned {
			OPM = 0,
			Total
		};
	}

	namespace OPL {
		enum Types : unsigned {
			SBI = 0,
			BNK,
			Total
		};
	}

}

#include "fmt/opm.hpp"

#endif