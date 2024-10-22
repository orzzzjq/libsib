#include <iostream>
#include <cstdlib>
#include <chrono>
#include <string>

typedef double FT;

#include "data_io.h"
#include "Vector_d.h"
#include "AABB_d.h"
#include "libsib.h"

using namespace std::chrono;

typedef DS::Vector_d<FT> Vector;
typedef DS::AABB_d<FT> AABB;

std::string *path, *file;
char data_filename[500];

std::vector<Vector> aabbs_data;
std::vector<AABB> aabbs;
int d = 2, n = 3, no = 0;

int main() {
	sprintf_s(data_filename, "C:/_/Project/libsib-dev/data/aabb/aabb_64d_1000_#0.txt");

	if (!IO::read_aabb(data_filename, aabbs_data, n, d)) {
		printf("Failed to load input points.\n");
		return EXIT_FAILURE;
	}

	for (int i = 0; i < n; ++i) {
		aabbs.push_back(AABB(d, aabbs_data[i]));
	}
	
	auto start = high_resolution_clock::now();

	LIBSIB::solve<AABB>(aabbs, d, n);

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);

	printf("\n\n---- statistics ----\n");
	printf("dimension: %d\n", d);
	printf("number of objects: %d\n", n);
	printf("final radius: %.6e\n", LIBSIB::get_radius());
	printf("iteration count: %d\n", LIBSIB::get_iteration());
	printf("running time: %lld ms\n", duration.count());

	return EXIT_SUCCESS;
}
