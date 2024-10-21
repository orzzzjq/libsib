#pragma once
#include <vector>
#include "Vector_d.h"

#define SQRT2 1.4142135623730951
#define SQRT2INV 0.7071067811865475

namespace SOC {
	typedef LA::Vector_d<double> Vector;

	class Element {
	private:
		int d;
		Vector x;
		double R;
		double eigenval[2];
		Vector* idempotent[2];
	public:
		Element() {
			this->d = 2;
			this->x = Vector(d);
			this->R = SQRT2;
		}
		Element(int d) {
			this->d = d;
			this->x = Vector(d);
			this->R = SQRT2;
			this->idempotent[0] = new Vector(d);
			this->idempotent[1] = new Vector(d);
		}
		template <typename T>
		Element(const T& x, double R) {
			this->d = x.dimension();
			this->x = Vector(d, x.cartesian_begin(), x.cartesian_end());
			this->R = R;
			this->idempotent[0] = new Vector(d);
			this->idempotent[1] = new Vector(d);
		}
		template <typename InputIterator>
		Element(int d, InputIterator begin, InputIterator end, double R) {
			this->d = d;
			this->x = Vector(d, begin, end);
			this->R = R;
			this->idempotent[0] = new Vector(d);
			this->idempotent[1] = new Vector(d);
		}

		void to_zero() {
			this->x -= this->x;
			this->R = 0;
		}

		double dimension() const { return this->d; }
		Vector get_x() const { return this->x; }
		double get_R() const { return this->R; }
		double x_norm() const { return sqrt(this->x.squared_length()); }
		double norm() const { return sqrt(this->x.squared_length() + this->R * this->R); }
		double trace() const { return SQRT2 * this->R; }

		void set_x(const Vector& x) { this->x = x; }
		void set_R(const double R) { this->R = R; }

		double& operator[](int i) { return this->x[i]; }
		double operator[](int i) const { return this->x[i]; }

		void copy(const Element& other) {
			this->x.copy(other.get_x());
			this->R = other.get_R();
		}

		Element operator+(const Element& other) const {
			if (this->d != other.dimension()) {
				std::cerr << "SOC (+): not in the same dimension.\n";
				throw std::invalid_argument("SOC (+): not in the same dimension.");
			}
			Vector new_x = this->x + other.get_x();
			double new_R = this->R + other.get_R();
			return Element(new_x, new_R);
		}
		Element operator-(const Element& other) const {
			if (this->d != other.dimension()) {
				std::cerr << "SOC (-): not in the same dimension.\n";
				throw std::invalid_argument("SOC (-): not in the same dimension.");
			}
			Vector new_x = this->x - other.get_x();
			double new_R = this->R - other.get_R();
			return Element(new_x, new_R);
		}
		Element operator*(double k) const {
			Vector new_x = this->x * k;
			double new_R = this->R * k;
			return Element(new_x, new_R);
		}
		Element operator/(double k) const {
			if (k == 0) {
				std::cerr << "SOC (/): divider is 0.\n";
				throw std::invalid_argument("SOC (/): divider is 0.");
			}
			Vector new_x = this->x / k;
			double new_R = this->R / k;
			return Element(new_x, new_R);
		}
		void operator+=(const Element& other) {
			if (this->d != other.dimension()) {
				std::cerr << "SOC (+=): not in the same dimension.\n";
				throw std::invalid_argument("SOC (+=): not in the same dimension.");
			}
			this->x += other.get_x();
			this->R += other.get_R();
		}
		void operator-=(const Element& other) {
			if (this->d != other.dimension()) {
				std::cerr << "SOC (-=): not in the same dimension.\n";
				throw std::invalid_argument("SOC (-=): not in the same dimension.");
			}
			this->x -= other.get_x();
			this->R -= other.get_R();
		}
		void operator*=(double k) {
			this->x *= k;
			this->R *= k;
		}
		void operator/=(double k) {
			if (k == 0) {
				std::cerr << "SOC (/=): divider is 0.\n";
				throw std::invalid_argument("SOC (/=): divider is 0.");
			}
			this->x /= k;
			this->R /= k;
		}
		// inner product
		double operator*(const Element& other) const {
			if (this->d != other.dimension()) {
				std::cerr << "SOC (inner product): not in the same dimension.\n";
				throw std::invalid_argument("SOC (inner product): not in the same dimension.");
			}
			return this->x * other.get_x() + this->R * other.get_R();
		}

		void spectral_decompose() {
			double x_norm = this->x_norm();
			eigenval[0] = (this->R + x_norm) * SQRT2INV;
			eigenval[1] = (this->R - x_norm) * SQRT2INV;
			if (x_norm != 0) {
				double x_norm_inv = 1. / x_norm;
				for (int i = 0; i < d; ++i) {
					double val = this->x[i] * x_norm_inv * SQRT2INV;
					(*idempotent[0])[i] = val;
					(*idempotent[1])[i] = -val;
				}
			}
			else {
				(*idempotent[0]) *= 0.;
				(*idempotent[1]) *= 0.;
				(*idempotent[0])[0] = 1;
				(*idempotent[1])[0] = -1;
			}
		}

		void spectral_assembling() {
			this->R = (eigenval[0] + eigenval[1]) * SQRT2INV;
			for (int i = 0; i < d; ++i) {
				for (int i = 0; i < d; ++i) {
					this->x[i] = (*idempotent[0])[i] * eigenval[0] + (*idempotent[1])[i] * eigenval[1];
				}
			}
		}

		double get_eigenval(int i) const { return this->eigenval[i]; }
		void set_eigenval(int i, double val) { this->eigenval[i] = val; }

		void print_spectral_decomposition() {
			this->spectral_decompose();
			printf("-- spectral decomposition --\n");
			for (int i = 0; i < 2; ++i) {
				printf("JordanFrame_1: %e (", eigenval[i]);
				for (int j = 0; j < d; ++j)
					printf("%e ", (*idempotent[i])[j]);
				printf("; %e)\n", SQRT2INV);
			}
		}

		// exponentiation
		void exp() {
			// spectral decomposition
			this->spectral_decompose();
			// spectral assembling w. exponential eigenvals
			eigenval[0] = std::exp(eigenval[0]);
			eigenval[1] = std::exp(eigenval[1]);
			this->R = (eigenval[0] + eigenval[1]) * SQRT2INV;
			for (int i = 0; i < d; ++i) {
				this->x[i] = (*idempotent[0])[i] * eigenval[0] + (*idempotent[1])[i] * eigenval[1];
			}
		}
	};
	
	std::ostream& operator<<(std::ostream& os, Element const& x) {
		double xi = 0;

		os << "(";
		for (int i = 0; i < x.get_x().dimension(); ++i) {
			os << x.get_x()[i] << " ";
		}
		os << "; " << x.get_R() << ")";

		return os;
	}

	struct JordanFrame {
		double eigenval = 0;
		Element idempotent;
		JordanFrame() {}
		JordanFrame(double eigenval, Element idempotent) : eigenval(eigenval), idempotent(idempotent) {}
	};

	std::vector<JordanFrame> spectral_decompose(const Element& x) {
		std::vector<JordanFrame> res(2);
		double x_norm = x.x_norm();
		res[0].eigenval = (x.get_R() + x_norm) / SQRT2;
		res[1].eigenval = (x.get_R() - x_norm) / SQRT2;
		if (x_norm != 0) {
			Vector normalized_x = x.get_x() / x_norm;
			res[0].idempotent = Element(normalized_x, 1.) / SQRT2;
			res[1].idempotent = Element((-1.) * normalized_x, 1.) / SQRT2;
		}
		else {
			std::vector<double> coord(x.dimension(), 0.);
			coord[0] = 1.;
			res[0].idempotent = Element(x.dimension(), coord.begin(), coord.end(), 1.) / SQRT2;
			coord[0] = -1.;
			res[1].idempotent = Element(x.dimension(), coord.begin(), coord.end(), 1.) / SQRT2;
		}
		return res;
	}

	Element spectral_assemble(const std::vector<JordanFrame>& frame) {
		int d = frame[0].idempotent.dimension();
		Element res(Vector(d), 0);
		for (int i = 0; i < frame.size(); ++i)
			res += frame[i].idempotent * frame[i].eigenval;
		return res;
	}

	void test() {
		auto x = Element(2);
		std::cout << x << std::endl;

		std::vector<double> vec{ 1.20, 23.45 };
		auto y = Element(2, vec.begin(), vec.end(), 10);

		vec = std::vector<double>{ 3,4,5,6 };
		y = Element(4, vec.begin(), vec.end(), 10);

		x.print_spectral_decomposition();

		//auto y_frame = spectral_decompose(y);
		y.print_spectral_decomposition();

		puts("--- exponential ---");
		x.exp();
		std::cout << x << std::endl;

		puts("--- exponential ---");
		y.exp();
		std::cout << y << std::endl;
	}
}
