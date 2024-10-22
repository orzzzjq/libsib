#include <iostream>
#include <cstdlib>
#include <chrono>
#include <string>

#include "data_io.h"
#include "Vector_d.h"
#include "AABB_d.h"
#include "ses_solver.h"

#define SOLVER SES
#define ARGS

using namespace std::chrono;

typedef double FT;
typedef DS::Vector_d<FT> Vector;
typedef DS::AABB_d<FT> AABB;

std::string *path, *file;
char data_filename[500];

std::vector<Vector> aabbs_data;
std::vector<AABB> aabbs;
int d = 2, n = 3, no = 0;

int main() {
	sprintf_s(data_filename, "C:/_/Project/libsib-dev/data/aabb/aabb_2d_3_#0.txt");

	if (!IO::read_aabb(data_filename, aabbs_data, n, d)) {
		printf("Failed to load input points.\n");
		return EXIT_FAILURE;
	}

	aabbs.resize(n);
	for (int i = 0; i < n; ++i) {
		aabbs[i] = AABB(d, aabbs_data[i]);
	}

	auto start = high_resolution_clock::now();

	//SOLVER::solve(aabbs, n, d);

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);

	printf("\nfinal radius: %.10e\n", SOLVER::get_radius());
	printf("running time: %lld ms\n", duration.count());

	return EXIT_SUCCESS;
}
