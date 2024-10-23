#pragma once
#include "Vector_d.h"

namespace DS {
	template <typename FT>
	class Ball_d {
	private:
		Vector_d<FT> c; // center
		FT r; // radius
		int d; // dimension
	public:
		Ball_d() {}
		Ball_d(int d, Vector_d<FT>& center, FT radius) {
			this->d = d;
			this->c = Vector_d<FT>(d);
			this->c.copy(center);
			this->r = radius;
		}
		
		int dimension() const { return this->d; }
		Vector_d<FT> get_center() { return this->c; }
		Vector_d<FT>& get_center() const { return this->c; }
		FT get_radius() { return this->r; }

		// minimize a linear function <h,x> over the ball
		void minimize(const Vector_d<FT>& h, Vector_d<FT>& x) const {
			for (int i = 0; i < this->d; ++i) {
				x[i] = -h[i];
			}
			if (x.length() > 1e-12) {
				x.normalize();
				x *= this->r;
				x += this->c;
			}
			else x.copy(this->c);
		}
	};
}
