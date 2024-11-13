#pragma once
#include "Vector_d.h"
#include "Object.h"
#include <vector>

namespace DS {
	template <typename FT>
	class Polytope_d : public Object<FT> {
	private:
		std::vector<Vector_d<FT>> p;
		int d, m; // dimension, size of point set
	public:
		Polytope_d() {}
		Polytope_d(int d, int m, std::vector<Vector_d<FT>>& points) {
			this->d = d;
			this->m = m;
			this->p.clear();
			for (int i = 0; i < m; ++i) {
				this->p.push_back(Vector_d<FT>(d));
				this->p.back().copy(points[i]);
			}
		}
		
		int dimension() const { return this->d; }
		int get_size() const { return this->m; }

		Vector_d<FT> operator[](int idx) const { return this->p[idx]; }
		Vector_d<FT>& operator[](int idx) { return this->p[idx]; }

		// minimize a linear function <h,x> over the convex polytope
		void minimize(const Vector_d<FT>& h, Vector_d<FT>& x) const {
			int id_min = 0;
			FT val, val_min = std::numeric_limits<FT>::max();
			for (int i = 0; i < m; ++i) {
				val = h * this->p[i];
				if (val < val_min) {
					val_min = val;
					id_min = i;
				}
			}
			x.copy(this->p[id_min]);
		}
	};
}
