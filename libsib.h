#pragma once
#include <vector>
#include "debug.h"
#include "Vector_d.h"
#include "product_cone.h"
#include "scmwu.h"

namespace LIBSIB {
	using namespace std;
	typedef DS::Vector_d<FT> Vector;

	int d, n; // dimension, number of objects
	int MxT = 10000; // maximum number of iterations
	FT eta = 0.1; // stepsize
	
	Vector z, z_acc; // center of the intersecting ball, sum of the past iterates
	Vector z_avg, vi_avg; // average center, average vi
	vector<Vector> v, v_acc; // certificate in each object, sum of the past iterates
	ProductCone::Element y, m, m_acc; // spectraplex vector, loss vector, accumulated loss

	Vector h, z_best; // the coefficient of the linear function to be optimized
	FT val, val_v, val_z; // record the minimum value when optimizing over convex hull
	FT radius_t, radius_i, diff_ij;

	FT gap = 1e-3;
	const int window_size = 10;
	Vector past_values(window_size); // check if the value stablize

	// initialize the variables
	void init() {
		z = Vector(d);
		z_acc = Vector(d);

		z_avg = Vector(d);
		vi_avg = Vector(d);

		z_best = Vector(d);
		h = Vector(d);
		v.clear();
		v_acc.clear();
		for (int i = 0; i < n; ++i) {
			v.push_back(Vector(d));
			v_acc.push_back(Vector(d));
			v_acc.back().to_zero();
		}
		y = ProductCone::Element(n, d);
		m = ProductCone::Element(n, d);
		m_acc = ProductCone::Element(n, d);

		z_acc.to_zero();
		m.to_zero();
		m_acc.to_zero();
	}

	template <typename Object>
	void process(const vector<Object>& objects) {
		int t = 0;

		y.normalize();
		for (; t < MxT; ++t) {
			//DEBUG("-- iteration : %d | ", t + 1);

			// oracle: solve for z and v[i]
			h.to_zero();
			for (int i = 0; i < n; ++i) h += y[i].get_x(); // get coefficient vector
			val_z = numeric_limits<FT>::max();
			for (int i = 0; i < n; ++i) {
				objects[i].minimize(h, z);
				val = h * z;
				if (val < val_z) { // record the best value
					val_z= val;
					z_best.copy(z);
				}
			}
			z.copy(z_best); // the solution for z
			z_acc += z;

			val_v = 0;
			for (int i = 0; i < n; ++i) {
				h.to_zero();
				h -= y[i].get_x(); // coefficient is negative y[i]_bar
				objects[i].minimize(h, v[i]); // the solution for v[i]
				val_v += h * v[i];
				v_acc[i] += v[i];
			}

			// convergence criterion
			// TODO
			// compute the average center
			for (int j = 0; j < d; ++j) {
				z_avg[j] = z_acc[j] / (t + 1);
			}
			// compute the average vi, and the maximum distance (radius)
			radius_t = 0;
			for (int i = 0; i < n; ++i) {
				radius_i = 0;
				for (int j = 0; j < d; ++j) {
					vi_avg[j] = v_acc[i][j] / (t + 1);
					diff_ij = vi_avg[j] - z_avg[j];
					radius_i += diff_ij * diff_ij;
				}
				radius_t = radius_t < radius_i ? radius_i : radius_t;
			}

			// scmwu: update y
			m.to_zero();
			for (int i = 0; i < n; ++i) m[i].set_x(z - v[i]);
			m_acc += m;
			SCMWU::update(m_acc, y, n, d, eta);

			//DEBUG("\n");
		}

		radius_t = 0;
		DEBUG("center : (%.6e, %.6e)\n", z_avg[0], z_avg[1]);
		for (int i = 0; i < n; ++i) {
			radius_i = 0;
			for (int j = 0; j < d; ++j) {
				vi_avg[j] = v_acc[i][j] / (t + 1);
				diff_ij = vi_avg[j] - z_avg[j];
				radius_i += diff_ij * diff_ij;
			}
			radius_t = radius_t < radius_i ? radius_i : radius_t;
		}
		DEBUG("radius : %.6e\n", sqrt(radius_t));
	}

	// Solve the SIB problem for a generic type of input objects
	template <typename Object>
	void solve(const vector<Object>& objects, int d, int n) {
		LIBSIB::d = d, LIBSIB::n = n;
		init();
		process(objects);
	}
}