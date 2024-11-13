#pragma once
#include <iostream>
#include <math.h>
#include <memory.h>

namespace DS {
	template <typename FT>
	class Vector_d {
	private:
		FT* v; // vector array
		int d; // dimension
	public:
		Vector_d() {}
		Vector_d(int d) {
			this->d = d;
			this->v = (FT*)malloc(d * sizeof(FT));
			for (int i = 0; i < d; ++i) this->v[i] = 0;
		}
		template <typename InputIterator>
		Vector_d(int d, InputIterator begin, InputIterator end) {
			this->d = d;
			this->v = (FT*)malloc(d * sizeof(FT));
			int i = 0;
			for (InputIterator it = begin; it != end && i < d; ++it, ++i) {
				this->v[i] = (*it);
			}
		}
		//~Vector_d() { if(this->v) free(this->v); }

		int dimension() const { return this->d; }
		FT* cartesian_begin() const { return this->v; }
		FT* cartesian_end() const { return this->v + d; }
		FT squared_length() const {
			FT res = 0;
			for (int i = 0; i < d; ++i) res += this->v[i] * this->v[i];
			return res;
		}
		FT length() const {
			return sqrt(this->squared_length());
		}

		FT cartesian(int i) const { return this->v[i]; }
		FT& operator[](int i) { return this->v[i]; }
		FT operator[](int i) const { return this->v[i]; }

		void to_zero() {
			for (int i = 0; i < d; ++i) this->v[i] = 0;
		}

		void normalize() {
			FT length = this->length();
			for (int i = 0; i < d; ++i) this->v[i] /= length;
		}

		void copy(const Vector_d<FT>& other) {
			if (this->d != other.dimension()) {
				std::cerr << "Vector_d: not in the same dimension.\n";
				throw std::invalid_argument("Vector_d: not in the same dimension.");
			}
			for (int i = 0; i < d; ++i) this->v[i] = other[i];
		}

		void operator+=(const Vector_d<FT>& other) {
			if (this->d != other.dimension()) {
				std::cerr << "Vector_d: not in the same dimension.\n";
				throw std::invalid_argument("Vector_d: not in the same dimension.");
			}
			for (int i = 0; i < d; ++i) this->v[i] += other[i];
		}
		void operator-=(const Vector_d<FT>& other) {
			if (this->d != other.dimension()) {
				std::cerr << "Vector_d: not in the same dimension.\n";
				throw std::invalid_argument("Vector_d: not in the same dimension.");
			}
			for (int i = 0; i < d; ++i) this->v[i] -= other[i];
		}
		void operator*=(FT k) {
			for (int i = 0; i < d; ++i) this->v[i] *= k;
		}
		void operator/=(FT k) {
			if (k == 0) {
				std::cerr << "Vector_d: divided by zero.\n";
				throw std::invalid_argument("Vector_d: divided by zero.");
			}
			for (int i = 0; i < d; ++i) this->v[i] /= k;
		}
		Vector_d<FT> operator+(const Vector_d<FT>& other) const {
			if (this->d != other.dimension()) {
				std::cerr << "Vector_d: not in the same dimension.\n";
				throw std::invalid_argument("Vector_d: not in the same dimension.");
			}
			Vector_d<FT> res_v(d);
			for (int i = 0; i < d; ++i) res_v[i] = this->v[i] + other[i];
			return res_v;
		}
		Vector_d<FT> operator-(const Vector_d<FT>& other) const {
			if (this->d != other.dimension()) {
				std::cerr << "Vector_d: not in the same dimension.\n";
				throw std::invalid_argument("Vector_d: not in the same dimension.");
			}
			Vector_d<FT> res_v(d);
			for (int i = 0; i < d; ++i) res_v[i] = this->v[i] - other[i];
			return res_v;
		}
		Vector_d<FT> operator*(FT k) const {
			Vector_d<FT> res_v(d);
			for (int i = 0; i < d; ++i) res_v[i] = this->v[i] * k;
			return res_v;
		}
		Vector_d<FT> operator/(FT k) const {
			if (k == 0) {
				std::cerr << "Vector_d: divided by zero.\n";
				throw std::invalid_argument("Vector_d: divided by zero.");
			}
			Vector_d<FT> res_v(d);
			for (int i = 0; i < d; ++i) res_v[i] = this->v[i] / k;
			return res_v;
		}
		// inner product
		FT operator*(const Vector_d<FT>& other) const {
			if (this->d != other.dimension()) {
				std::cerr << "Vector_d: not in the same dimension.\n";
				throw std::invalid_argument("Vector_d: not in the same dimension.");
			}
			FT res = 0;
			for (int i = 0; i < d; ++i) res += this->v[i] * other[i];
			return res;
		}

		double dist_sqr(const Vector_d<FT>& other) const {
			if (this->d != other.dimension()) {
				std::cerr << "Vector_d: not in the same dimension.\n";
				throw std::invalid_argument("Vector_d: not in the same dimension.");
			}
			double diff, res = 0.0;
			for (int i = 0; i < d; ++i) {
				diff = this->v[i] - other[i];
				res += diff * diff;
			}
			return res;
		}

		double dist(const Vector_d<FT>& other) const {
			return sqrt(this->dist_sqr(other));
		}

		// minimize a linear function <h,x> (indeed, return the point)
		void minimize(const Vector_d<FT>& h, Vector_d<FT>& x) const {
			x.copy(*this);
		}
	};
}

template <typename FT>
DS::Vector_d<FT> operator*(FT k, const DS::Vector_d<FT>& v) {
	int d = v.dimension();
	DS::Vector_d<FT> res_v(d);
	for (int i = 0; i < d; ++i) res_v[i] = v[i] * k;
	return res_v;
}
