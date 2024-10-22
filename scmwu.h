#pragma once

#include "product_cone.h"

namespace SCMWU {
	typedef ProductCone::Element K;

	void update(const K& acc_m, K& p, int n, int d, FT eta) {
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < d; ++j) {
				p[i][j] = acc_m[i][j] * eta;
			}
			p[i].set_R(acc_m[i].get_R() * eta);
		}
		p.exp();
		p.normalize();
	}
}
