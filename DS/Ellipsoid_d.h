#pragma once
#include "Vector_d.h"
#include <Eigen/Dense>


namespace DS {
	typedef Eigen::MatrixXd Matrix;

	template <typename FT>
	class Ellipsoid_d {
	private:
		Vector_d<FT> c; // center
		Matrix Q; // (inverse) of the positive definite matrix
		int d; // dimension
	public:
		Ellipsoid_d() {}
		Ellipsoid_d(int d, Vector_d<FT>& center, Matrix& Q) {
			this->d = d;
			this->c = Vector_d<FT>(d);
			this->c.copy(center);
			this->Q = Q;
		}
		
		int dimension() const { return this->d; }
		Vector_d<FT> get_center() { return this->c; }
		Vector_d<FT>& get_center() const { return this->c; }
		Matrix get_matrix() const { return this->Q; }

		// minimize a linear function <h,x> over the ellipsoid
		void minimize(const Vector_d<FT>& h, Vector_d<FT>& x) const {
			Matrix H(this->d, 1);
			for (int i = 0; i < this->d; ++i) {
				H(i, 0) = h[i];
			}
			x.copy(this->c);
			Matrix S = H.transpose() * this->Q * H;
			double norm = sqrt(S(0, 0));
			if (norm > 1e-12) {
				S = this->Q * H / norm;
				for (int i = 0; i < this->d; ++i) {
					x[i] -= S(i, 0);
				}
			}
		}
	};
}
