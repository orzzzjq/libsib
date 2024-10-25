#include "DS/Vector_d.h"
#include "DS/Ellipsoid_d.h"

#include <boost/random.hpp>
#include <boost/random/uniform_on_sphere.hpp>

#include <random>
#include <Eigen/Dense>

#include "data_io.h"

#include <iostream>
#include <cstdlib>
#include <chrono>

typedef DS::Vector_d<double> Point;
typedef Eigen::MatrixXd Matrix;

typedef boost::random::mt19937 gen_type;
gen_type rand_gen;


// generate a random point on unit sphere
Point direction_gen(int d) {
	boost::uniform_on_sphere<double> unif_sphere(d);
	boost::variate_generator<gen_type&, boost::uniform_on_sphere<double>> random_on_sphere(rand_gen, unif_sphere);
	std::vector<double> coord = random_on_sphere();
	return Point(d, coord.begin(), coord.end());
}

double uniform(double l, double r) {
	boost::uniform_real<double> unif(l, r);
	boost::variate_generator<gen_type&, boost::uniform_real<double>> random(rand_gen, unif);
	return random();
}

/*	Generate unit cubes
	
	Data structure : every AABB is a (d*2)-dimensional Point
organized as
	( lower bound of the 1st dimension,   ... [0]
	upper bound of the first diemsnion,   ... [1]
		...,
	lower bound of the d-th dimension,    ... [(d-1)*2]
	upper bound of the d-th dimension )   ... [(d-1)*2 + 1]
*/
void aabb_gen(const char* path, int d, int n, int num) {
	char filename[500];
	for (int no = 0; no < num; ++no) {
		sprintf_s(filename, "%s/aabb_%dd_%d_#%d.txt", path, d, n, no);
		// generate n boxes
		std::vector<Point> aabbs;
		for (int i = 0; i < n; ++i) {
			auto direction = direction_gen(d) * sqrt(d);
			std::vector<double> coord;
			for (int j = 0; j < d; ++j) {
				coord.push_back(direction[j] - 0.5);
				coord.push_back(direction[j] + 0.5);
			}
			aabbs.push_back(Point(d * 2, coord.begin(), coord.end()));
		}
		IO::write_aabb(filename, aabbs, d);
		puts(filename);
	}
}

/*	Generate unit balls

	Data structure : every ball is represented as:
	(radius, center)
*/
void ball_gen(const char* path, int d, int n, int num) {

	char filename[500];
	for (int no = 0; no < num; ++no) {
		sprintf_s(filename, "%s/ball_%dd_%d_#%d.txt", path, d, n, no);
		// generate n balls
		std::vector<Point> centers;
		std::vector<float> radii;
		for (int i = 0; i < n; ++i) {
			centers.push_back(direction_gen(d) * 4);
			radii.push_back(1.0);
		}
		IO::write_ball(filename, centers, radii, d);
		puts(filename);
	}
}

/*	Generate random polytopes

	Data structure : every polytope is represented by a point set in the unit ball
*/
void poly_gen(const char* path, int d, int n, int m, int num) {
	char filename[500];
	for (int no = 0; no < num; ++no) {
		sprintf_s(filename, "%s/poly_%dd_%d_#%d.txt", path, d, n, no);
		// generate n polytopes
		std::vector<Point> p_points;
		std::vector<int> p_sizes;
		for (int i = 0; i < n; ++i) {
			p_sizes.push_back(m);
			auto direction = direction_gen(d) * 4;
			for (int j = 0; j < m; ++j) {
				p_points.push_back(direction_gen(d) * sqrt(uniform(0, 1)) + direction);
			}
		}
		IO::write_poly(filename, p_points, p_sizes, d);
		puts(filename);
	}
}

/*	Generate reduced polytopes

	Data structure : every polytope is represented by a point set in the unit ball,
the constraint parameter is 1/x for each reduced polytope, where x ~ Unif(1, mi).
*/
void rpoly_gen(const char* path, int d, int n, int m, int num) {
	char filename[500];
	for (int no = 0; no < num; ++no) {
		sprintf_s(filename, "%s/rpoly_%dd_%d_#%d.txt", path, d, n, no);
		// generate n polytopes
		std::vector<Point> p_points;
		std::vector<int> p_sizes;
		std::vector<double> p_params;
		for (int i = 0; i < n; ++i) {
			p_sizes.push_back(m);
			p_params.push_back(1.0 / uniform(1, m));
			auto direction = direction_gen(d) * 4;
			for (int j = 0; j < m; ++j) {
				p_points.push_back(direction_gen(d) * sqrt(uniform(0, 1)) + direction);
			}
		}
		IO::write_rpoly(filename, p_points, p_sizes, p_params, d);
		puts(filename);
	}
}

/*	Generate ellipsoids

	Data structure : every ellipsoid is represented by its center c and a 
positive definite matrix Q, and the ellipsoids is defined as:
	(x - c)^T Q^{-1} (x - c)
*/
void ellip_gen(const char* path, int d, int n, int num) {
	std::mt19937 generator;
	generator.seed(std::time(0));
	std::uniform_real_distribution<double> distribution(-1.0, 1.0);
	auto random = [&]() { return distribution(generator); };

	char filename[500];
	for (int no = 0; no < num; ++no) {
		sprintf_s(filename, "%s/ellip_%dd_%d_#%d.bin", path, d, n, no);
		// generate n ellipsoids
		std::vector<Point> centers;
		std::vector<Matrix> mats;
		centers.clear();
		mats.clear();
		Matrix A;
		for (int i = 0; i < n; ++i) {
			centers.push_back(direction_gen(d) * 4);
			A = Matrix::NullaryExpr(d, d, random);
			mats.push_back(A * A.transpose());
		}
		IO::write_ellip(filename, centers, mats, d);
		puts(filename);
	}
}

int main()
{
	int d = 2, n = 5, num = 1;

	rand_gen.seed(std::time(0));

	ellip_gen("C:/_/Project/libsib-dev/data/ellip", d, n, num);

	return 0;
}
