#pragma once
#include "Vector_d.h"

namespace DS {
	template <typename FT>
	class AABB_d {
	private:
		Vector_d<FT> l, u; // lower and upper
		int d; // dimension
	public:
		AABB_d() {}
		AABB_d(int d, Vector_d<FT>& data) {
			this->d = d;
			this->l = Vector_d<FT>(d);
			this->u = Vector_d<FT>(d);
			for (int i = 0; i < d; ++i) {
				this->l[i] = data[i << 1];
				this->u[i] = data[i << 1 | 1];
			}
		}
		
		int dimension() const { return this->d; }
		Vector_d<FT> lower_vector() { return this->l; }
		Vector_d<FT> upper_vector() { return this->u; }
	};
}
