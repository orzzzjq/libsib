#include <iostream>
#include <cstdlib>
#include <chrono>
#include <string>

#include "points_io.h"
#include "Vector_d.h"
#include "ses_solver.h"

#define SOLVER SES
#define ARGS

using namespace std::chrono;

typedef LA::Vector_d<double> Vector;

std::string *path, *file;
char data_filename[500];
FILE* stat_fp;

std::vector<Vector> points;
int n = 100000, d = 1024, no = 0;

void print_help() {
	printf("** PDSCP-ST Smallest Enclosing Sphere (beta)\n");
	printf("-h: help\n");
	printf("-p [path]: input data path\n");
	printf("-f [file]: data file name\n");
}

bool parse(char** argv) {
	for (int i = 1; i < 5; i += 2) {
		if (strlen(argv[i]) < 2) return false;
		switch (argv[i][1]) {
			case 'p': path = new std::string(argv[i + 1]); break;
			case 'f': file = new std::string(argv[i + 1]); break;
			default: return false;
		}
	}
	return true;
}

int main(int argc, char** argv) {
	// arguments: -p [path] -f [file]
	if (argc != 5 || !parse(argv)) {
		print_help();
		return EXIT_FAILURE;
	}

	sprintf_s(data_filename, "%s/%s", path->c_str(), file->c_str());

	if (!IO::read_points(data_filename, points, n, d)) {
		printf("Failed to load input points.\n");
		return EXIT_FAILURE;
	}

	auto start = high_resolution_clock::now();

	SOLVER::solve(points, n, d);

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);

	printf("\nfinal radius: %.10e\n", SOLVER::get_radius());
	printf("running time: %lld ms\n", duration.count());

	return EXIT_SUCCESS;
}
