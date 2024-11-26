#pragma once

#include <vector>
#include "soc.h"

namespace ProductCone {
	class Element {
	private:
		int N; // number of component cones
		std::vector<SOC::Element> members;
	public:
		Element() {}
		Element(int N) {
			this->N = N;
			for (int i = 0; i < N; ++i) 
				this->members.push_back(SOC::Element());
		}
		Element(int N, int d) {
			this->N = N;
			for (int i = 0; i < N; ++i) 
				this->members.push_back(SOC::Element(d));
		}
		Element(const std::vector<SOC::Element>& members) {
			this->N = members.size();
			this->members = members;
		}


		int get_N() const { return this->N; }
		int size() const { return this->N; }
		SOC::Element operator[](int idx) const { return this->members[idx]; }
		SOC::Element& operator[](int idx) { return this->members[idx]; }

		void to_zero() {
			for (int i = 0; i < this->N; ++i)
				this->members[i].to_zero();
		}

		void copy(const Element& other) {
			for (int i = 0; i < this->N; ++i) {
				this->members[i].copy(other[i]);
			}
		}

		double trace() const {
			double res = 0;
			for (int i = 0; i < this->N; ++i)
				res += this->members[i].trace();
			return res;
		}

		Element operator*(double k) const {
			std::vector<SOC::Element> new_members;
			for (int i = 0; i < this->N; ++i) {
				new_members.push_back(this->members[i] * k);
			}
			return Element(new_members);
		}
		void operator+=(const Element& other) {
			for (int i = 0; i < this->N; ++i) {
				this->members[i] += other[i];
			}
		}
		void operator*=(double k) {
			for (int i = 0; i < this->N; ++i) {
				this->members[i] *= k;
			}
		}
		void operator/=(double k) {
			for (int i = 0; i < this->N; ++i) {
				this->members[i] /= k;
			}
		}
		// inner product
		double operator*(const Element& other) {
			double res = 0;
			for (int i = 0; i < this->N; ++i) {
				res += this->members[i] * other[i];
			}
			return res;
		}

		void exp_normalize() {
			double trace = 0.0;
			for (int i = 0; i < this->N; ++i) {
				this->members[i].spectral_decompose();
				trace += std::exp(members[i].get_eigenval(0)) + std::exp(members[i].get_eigenval(1));
			}
			for (int i = 0; i < this->N; ++i) {
				double exp_val_1, exp_val_2;
				exp_val_1 = std::exp(members[i].get_eigenval(0)) / trace;
				exp_val_2 = std::exp(members[i].get_eigenval(1)) / trace;
				members[i].set_eigenval(0, exp_val_1);
				members[i].set_eigenval(1, exp_val_2);
				members[i].spectral_assembling();
				//members[i].spectral_decompose();
				//if (members[i].get_eigenval(0) < 0 || members[i].get_eigenval(1) < 0) {
				//	printf("p not in the cone, eigenvals bf asm: %.2e %.2e, eigenvals af asm: %.2e %.2e\n", exp_val_1, exp_val_2, members[i].get_eigenval(0), members[i].get_eigenval(1));
				//}
			}
		}

		void exp() {
			for (int i = 0; i < this->N; ++i) {
				this->members[i].exp();
			}
		}

		void normalize() {
			double trace = this->trace();
			this->operator/=(trace);
			//for (int i = 0; i < this->N; ++i) {
			//	double x_norm = this->members[i].x_norm();
			//	if (x_norm > this->members[i].get_R()) {
			//		this->members[i].set_R(x_norm);
			//	}
			//}
		}
	};
}
