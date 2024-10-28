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

// AABB
#include "DS/AABB_d.h"

//// Ball
//#include "DS/Ball_d.h"
//typedef DS::Ball_d<FT> Ball;

//// Polytope
//#include "DS/Polytope_d.h"
//typedef DS::Polytope_d<FT> Poly;
//std::vector<Poly> polys;

//// Reduced polytope
//#include "DS/ReducedPolytope_d.h"
//typedef DS::ReducedPolytope_d<FT> RPoly;
//std::vector<RPoly> rpolys;

//// Ellipsoid
//#include "DS/Ellipsoid_d.h"
//typedef DS::Ellipsoid_d<FT> Ellip;
//std::vector<Ellip> ellips;

typedef DS::AABB_d<FT> Object;
std::vector<Object> objects;

int d, n, no = 0;

int main() {
	sprintf_s(data_filename, "C:/_/Project/libsib-dev/data/aabb/aabb_2d_100_#%d.txt", no);

	if (!IO::read_aabb<Object, Vector>(data_filename, objects, n, d)) {
		printf("Failed to load input points.\n");
		return EXIT_FAILURE;
	}

	auto start = high_resolution_clock::now();

	LIBSIB::solve<Object>(objects, d, n);

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);

	printf("\n\n---- statistics ----\n");
	printf("dimension: %d\n", d);
	printf("number of objects: %d\n", n);
	printf("final radius: %.6e\n", LIBSIB::get_radius());
	printf("iteration count: %d\n", LIBSIB::get_iteration());
	printf("running time: %lld ms\n", duration.count());

	if (d == 2) DEBUG("center: (%.7e, %.7e)\n", LIBSIB::get_center()[0], LIBSIB::get_center()[1]);
	
	return EXIT_SUCCESS;
}
