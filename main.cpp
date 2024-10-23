#include <iostream>
#include <cstdlib>
#include <chrono>
#include <string>

typedef double FT;

#include "debug.h"

#include "data_io.h"
#include "Vector_d.h"
#include "libsib.h"

using namespace std::chrono;

typedef DS::Vector_d<FT> Vector;

std::string *path, *file;
char data_filename[500];

// AABB
#include "AABB_d.h"
typedef DS::AABB_d<FT> AABB;
std::vector<Vector> aabbs_data;
std::vector<AABB> aabbs;

// Ball
#include "Ball_d.h"
typedef DS::Ball_d<FT> Ball;
std::vector<Vector> ball_centers;
std::vector<FT> ball_radii;
std::vector<Ball> balls;

// Polytope
#include "Polytope_d.h"
typedef DS::Polytope_d<FT> Poly;
std::vector<Poly> polys;

int d = 2, n = 3, no = 0;

int main() {
	sprintf_s(data_filename, "C:/_/Project/libsib-dev/data/poly/poly_2d_100_#0.txt");

	if (!IO::read_poly<Poly, Vector>(data_filename, polys, n, d)) {
		printf("Failed to load input points.\n");
		return EXIT_FAILURE;
	}

	auto start = high_resolution_clock::now();

	LIBSIB::solve<Poly>(polys, d, n);

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);

	printf("\n\n---- statistics ----\n");
	printf("dimension: %d\n", d);
	printf("number of objects: %d\n", n);
	printf("final radius: %.6e\n", LIBSIB::get_radius());
	printf("iteration count: %d\n", LIBSIB::get_iteration());
	printf("running time: %lld ms\n", duration.count());

	if (d == 2) DEBUG("center: (%.2e, %.2e)\n", LIBSIB::get_center()[0], LIBSIB::get_center()[1]);

	return EXIT_SUCCESS;
}
