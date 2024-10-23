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

// polytopes


int d = 2, n = 3, no = 0;

int main() {
	sprintf_s(data_filename, "C:/_/Project/libsib-dev/data/ball/ball_1024d_10000_#0.txt");

	if (!IO::read_ball(data_filename, ball_centers, ball_radii, n, d)) {
		printf("Failed to load input points.\n");
		return EXIT_FAILURE;
	}
	
	balls.clear();
	for (int i = 0; i < n; ++i) {
		balls.push_back(Ball(d, ball_centers[i], ball_radii[i]));
	}

	auto start = high_resolution_clock::now();

	LIBSIB::solve<Ball>(balls, d, n);

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
