#pragma once
#include <vector>
#include "Vector_d.h"
#include "product_cone.h"
#include "scmwu.h"
#include "debug.h"

template <typename FT>
inline FT max(const FT& a, const FT& b) {
	return (a > b) ? a : b;
}
template <typename FT>
inline FT min(const FT& a, const FT& b) {
	return (a < b) ? a : b;
}

namespace SES {
	typedef DS::Vector_d<double> Vector;

	std::vector<Vector> v; // the input points
	int n, d; // number of points and the dimension
	double eps = 1e-3; // the final accuracy, set by user

	double current_radius_sqr = std::numeric_limits<double>::max(); // record the current smallest radius square
	double current_radius = std::numeric_limits<double>::max(); // record the current smallest radius
	double scaled_val = 1.0; // all coordinates are scaled by this amount
	Vector current_center; // record the center with the current smallest radius
	Vector moving_dir; // the moving direction towards the origin
	
	const int win_size = 10;
	Vector past_radii(win_size); // record the result of past win_size iterations

	int last_t = 0; // the total #iterations that oracle successes when alpha >= OPT
	double b;
	Vector a, inside_v, y, acc_y, last_acc_y;
	ProductCone::Element rhs, p, m, acc_m, last_acc_m;

	FILE* fp;

	void reset() {
		v.clear();
		last_t = 0;
		current_radius_sqr = std::numeric_limits<double>::max();
		current_radius = std::numeric_limits<double>::max();
		current_center = Vector(d);
		inside_v = Vector(d);
		y = Vector(d);
		a = Vector(d);
		acc_y = Vector(d);
		last_acc_y = Vector(d);
		m = ProductCone::Element(n, d);
		acc_m = ProductCone::Element(n, d);
		last_acc_m = ProductCone::Element(n, d);
	}

	void pre_process(const std::vector<Vector>& points) {
		// scale the points such that ||v|| \le 1
		for (int i = 0; i < n; ++i)
			v.push_back(Vector(d, points[i].cartesian_begin(), points[i].cartesian_end()));
		// moving toward the origin
		moving_dir = Vector(d, v[0].cartesian_begin(), v[0].cartesian_end());
		double max_norm_sqr = 0;
		for (int i = 0; i < n; ++i) {
			v[i] -= moving_dir;
			max_norm_sqr = max(max_norm_sqr, v[i].squared_length());
		}
		// scaling the norms
		scaled_val = sqrt(max_norm_sqr);
		for (int i = 0; i < n; ++i) {
			v[i] /= scaled_val;
		}
		// construct rhs: (v_i; 0)
		std::vector<SOC::Element> members;
		for (int i = 0; i < n; ++i) {
			members.push_back(SOC::Element(d, v[i].cartesian_begin(), v[i].cartesian_end(), 0));
		}
		rhs = ProductCone::Element(members);
	}

	void post_process() {
		current_radius *= scaled_val;
		current_center *= scaled_val;
		current_center += moving_dir;
	}

	double get_radius() { return current_radius; }
	Vector& get_center() { return current_center; }

	int oracle(const Vector& a, double b, Vector& y) {
		inside_v.to_zero();
		int count = 0;
		for (int i = 0; i < n; ++i) {
			if (a * v[i] >= b) {
				inside_v += v[i];
				count++;
			}
		}
		if (count == 0) return 0;
		inside_v /= (count * 1.0);
		y.copy(inside_v);
		return count;
	}

	double update_solution(const Vector& y) {
		double max_dist_sqr = 0;
		for (int i = 0; i < n; ++i) {
			max_dist_sqr = max(max_dist_sqr, v[i].dist_sqr(y));
		}
		if (max_dist_sqr < current_radius_sqr) {
			current_radius_sqr = max_dist_sqr;
			current_radius = sqrt(current_radius_sqr);
			for (int i = 0; i < d; ++i) {
				current_center[i] = y[i];
			}
		}
		return sqrt(max_dist_sqr);
	}

	void compute_loss(const Vector& y, double alpha, ProductCone::Element& m) {
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < d; ++j) {
				m[i][j] = (y[j] - v[i][j]);
			}
			m[i].set_R(alpha);
		}
	}

	// line search
	int line_search(double alpha) {
		a.to_zero();
		for (int i = 0; i < p.size(); ++i) {
			a += p[i].get_x();
		}
		b = rhs * p - alpha * SQRT2INV;
		if (oracle(a, b, y) < n) return 0;
		compute_loss(y, alpha, m);
		acc_y.copy(y);

		int L = 1, U = 1, M;
		double multiplier;
		for (int i = 0; i < 30; ++i) {
			multiplier = (1LL << i) * 1.0;
			acc_m.copy(m);
			acc_m *= multiplier;
			SCMWU::update(acc_m, p, n, d);

			a.to_zero();
			for (int i = 0; i < p.size(); ++i) {
				a += p[i].get_x();
			}
			b = rhs * p - alpha * SQRT2INV;
			if (oracle(a, b, y) < n) {
				L = (1 << (i - 1));
				U = (1 << i);
				break;
			}
		}

		while (L < U - 1) {
			M = (L + U) >> 1;
			multiplier = M;
			acc_m.copy(m);
			acc_m *= multiplier;
			SCMWU::update(acc_m, p, n, d);

			a.to_zero();
			for (int i = 0; i < p.size(); ++i) {
				a += p[i].get_x();
			}
			b = rhs * p - alpha * SQRT2INV;
			
			if (oracle(a, b, y) < n) U = M;
			else L = M;
		}

		multiplier = U;
		acc_m.copy(m);
		acc_m *= multiplier;
		acc_y *= multiplier;
		SCMWU::update(acc_m, p, n, d);

		return U;
	}

	bool check(double alpha) {
		int t, T = min((int)1e+6, (int)(4 * log2(n) / (eps * eps)));
		
		// warm-start
		if (last_t == 0) {
			p = ProductCone::Element(n, d);
			p.normalize();
			acc_m.to_zero();
			acc_y.to_zero();
			t = line_search(alpha);
		}
		else {
			acc_m.copy(last_acc_m);
			acc_y.copy(last_acc_y);
			t = 0;
		}

		// early-stop
		int bad_count = 0;
		for (int i = 0; i < win_size; ++i)
			past_radii[i] = std::numeric_limits<double>::max();

		for (; t < T; ++t) {
			//DEBUG("-- iteration %d ", t + 1);
			// oracle
			RECORDSTART();
			a.to_zero();
			for (int i = 0; i < p.size(); ++i) {
				a += p[i].get_x();
			}
			b = rhs * p - alpha * SQRT2INV;
			if (!oracle(a, b, y)) {
				DEBUG("- oracle failed in iteration %d\n", t + 1);
				return false;
			}
			RECORDSTOP("oracle");
			// record solution
			RECORDSTART();
			acc_y += y;
			double r = update_solution(acc_y * (1.0 / (last_t + t + 1))), last_r;
			past_radii[t % win_size] = r;
			last_r = past_radii[(t + 1) % win_size];
			if (r < alpha) {
				last_t += t;
				last_acc_y.copy(acc_y);
				last_acc_m.copy(acc_m);
				DEBUG("+ Shortcut in iteration %d\n", t + 1);
				return true; // shortcut
			}
			if (fabs(last_r - r) < 1e-4 * last_r) {
				bad_count++;
				if (bad_count >= win_size) {
					last_t += t;
					last_acc_y.copy(acc_y);
					last_acc_m.copy(acc_m);
					DEBUG("+ No change in iteration %d\n", t + 1);
					return true;
				}
			}
			else bad_count = 0;
			RECORDSTOP("shortcut");
			// scmwu
			RECORDSTART();
			compute_loss(y, alpha, m);
			RECORDSTOP("compute_loss");
			RECORDSTART();
			acc_m += m;
			SCMWU::update(acc_m, p, n, d);
			RECORDSTOP("scmwu");
			//DEBUG("\n");
		}

		return true;
	}

	double init_upper_bound() {
		inside_v.to_zero();
		for (int i = 0; i < n; ++i) {
			inside_v += v[i];
		}
		inside_v /= (n * 1.0);
		y.copy(inside_v);
		double max_dist_sqr = 0;
		for (int i = 0; i < n; ++i) {
			max_dist_sqr = max(max_dist_sqr, v[i].dist_sqr(y));
		}

		return sqrt(max_dist_sqr);
	}

	void solve(const std::vector<Vector>& points, int n, int d) {
		SES::n = n, SES::d = d;
		SCMWU::eta = 1.0;
		reset();

		pre_process(points);
		DEBUG("scaled_val: %f\n", scaled_val);

		//fopen_s(&fp, "record.txt", "w");
		
		// binary search, main procedure
		double U = init_upper_bound(), L = 0.5, alpha;

		while ((U - L) >= eps * L) {
			alpha = (U + L) / 2.0;
			printf("U: %.6e, L: %.6e, alpha: %.6e, gap: %.4f\n", U, L, alpha, (U - L) / L);
			if (check(alpha)) U = min(alpha, current_radius);
			else L = alpha;
		}
		
		//if (fp) fclose(fp);
		post_process();

		DEBUG("-- final result --\n");
		DEBUG("Our_radius: %.6e\n", current_radius);
		DEBUG("Total #contributing iterations: %d\n", last_t + 1);
	}
}
