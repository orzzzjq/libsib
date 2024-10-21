#pragma once

#include "product_cone.h"

namespace SCMWU {
	double eta = 0.1; // step size
	typedef ProductCone::Element K;

	void update(const K& acc_m, K& p, int n, int d) {
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < d; ++j) {
				p[i][j] = acc_m[i][j] * (-eta);
			}
			p[i].set_R(acc_m[i].get_R() * (-eta));
		}
		p.exp();
		p.normalize();
	}
}
