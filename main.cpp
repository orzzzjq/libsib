#include <iostream>
#include <cstdlib>
#include <chrono>
#include <string>

typedef double FT;

#include "debug.h"

#include "data_io.h"
#include "DS/Vector_d.h"
#include "libsib.h"

using namespace std::chrono;

typedef DS::Vector_d<FT> Vector;

std::string *path, *file;
char data_filename[500];

//// AABB
//#include "DS/AABB_d.h"
//typedef DS::AABB_d<FT> AABB;
//std::vector<Vector> aabbs_data;
//std::vector<AABB> aabbs;
//
//// Ball
//#include "DS/Ball_d.h"
//typedef DS::Ball_d<FT> Ball;
//std::vector<Vector> ball_centers;
//std::vector<FT> ball_radii;
//std::vector<Ball> balls;
//
//// Polytope
//#include "DS/Polytope_d.h"
//typedef DS::Polytope_d<FT> Poly;
//std::vector<Poly> polys;
//
//// Reduced polytope
//#include "DS/ReducedPolytope_d.h"
//typedef DS::ReducedPolytope_d<FT> RPoly;
//std::vector<RPoly> rpolys;

// Ellipsoid
#include "DS/Ellipsoid_d.h"
typedef DS::Ellipsoid_d<FT> Ellip;
std::vector<Ellip> ellips;

int d, n, no = 0;

int main() {
	for (no = 0; no < 10; ++no) {
		sprintf_s(data_filename, "C:/_/Project/libsib-dev/data/ellip/ellip_16d_100_#%d.bin", no);

		if (!IO::read_ellip<Ellip, Vector>(data_filename, ellips, n, d)) {
			printf("Failed to load input points.\n");
			return EXIT_FAILURE;
		}

		auto start = high_resolution_clock::now();

		LIBSIB::solve<Ellip>(ellips, d, n);

		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(stop - start);

		printf("\n\n---- statistics ----\n");
		printf("dimension: %d\n", d);
		printf("number of objects: %d\n", n);
		printf("final radius: %.6e\n", LIBSIB::get_radius());
		printf("iteration count: %d\n", LIBSIB::get_iteration());
		printf("running time: %lld ms\n", duration.count());

		if (d == 2) DEBUG("center: (%.7e, %.7e)\n", LIBSIB::get_center()[0], LIBSIB::get_center()[1]);
	}
	return EXIT_SUCCESS;
}
