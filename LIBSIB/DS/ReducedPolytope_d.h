#pragma once
#include "Vector_d.h"
#include "Polytope_d.h"
#include <vector>
#include <algorithm>

namespace DS {
	template <typename FT>
	class ReducedPolytope_d : public Polytope_d<FT> {
	private:
		FT nv;
	public:
		ReducedPolytope_d() {}
		ReducedPolytope_d(int d, int m, FT nv, std::vector<Vector_d<FT>>& points) : Polytope_d<FT>(d, m, points) {
			this->nv = nv;
		}

		FT get_parameter() { return this->nv; }

		// minimize a linear function <h,x> over the reduced polytope
		void minimize(const Vector_d<FT>& h, Vector_d<FT>& x) const {
			std::vector<std::pair<FT, int >> vals;
			for (int i = 0; i < this->get_size(); ++i) {
				vals.push_back({ h * this->operator[](i), i});
			}
			std::sort(begin(vals), end(vals), std::less<std::pair<FT, int>>());

			x.to_zero();
			int contribute = floor(1.0 / this->nv);
			for (int i = 0; i < contribute; ++i) {
				x += this->operator[](vals[i].second);
			}
			x *= this->nv;
			if (this->nv * contribute < 1.0 - 1e-12) {
				FT rest = 1.0 - this->nv * contribute;
				x += this->operator[](contribute) * rest;
			}
		}
	};
}
