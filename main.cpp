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

#define args

bool tolog = false;
std::string *path, *file, *logfile;
char data_filename[500];

// Polytope
//#include "DS/Polytope_d.h"
//typedef DS::Polytope_d<FT> Object;
//bool (*read_file)(const char *, std::vector<Object>&, int&, int&) = IO::read_poly<Object, Vector>;

// Reduced polytope
//#include "DS/ReducedPolytope_d.h"
//typedef DS::ReducedPolytope_d<FT> Object;
//bool (*read_file)(const char *, std::vector<Object>&, int&, int&) = IO::read_rpoly<Object, Vector>;

// AABB
//#include "DS/AABB_d.h"
//typedef DS::AABB_d<FT> Object;
//bool (*read_file)(const char*, std::vector<Object>&, int&, int&) = IO::read_aabb<Object, Vector>;

// Ball
#include "DS/Ball_d.h"
typedef DS::Ball_d<FT> Object;
bool (*read_file)(const char*, std::vector<Object>&, int&, int&) = IO::read_ball<Object, Vector>;

// Ellipsoid
//#include "DS/Ellipsoid_d.h"
//typedef DS::Ellipsoid_d<FT> Object;
//bool (*read_file)(const char*, std::vector<Object>&, int&, int&) = IO::read_ellip<Object, Vector>;

std::vector<Object> objects;

int d, n, no = 0;

#ifdef args
bool parse(int argc, char** argv) {
	for (int i = 1; i < argc; i += 2) {
		if (strlen(argv[i]) < 2) return false;
		switch (argv[i][1]) {
		case 'p': path = new std::string(argv[i + 1]); break;
		case 'f': file = new std::string(argv[i + 1]); break;
		case 'l': logfile = new std::string(argv[i + 1]); tolog = true; break;
		default: return false;
		}
	}
	return true;
}

int main(int argc, char** argv) {
	// arguments: -p [path] -f [file] -l [log]
	if (!parse(argc, argv)) {
		//print_help();
		return EXIT_FAILURE;
	}

	sprintf_s(data_filename, "%s/%s", path->c_str(), file->c_str());
#else
int main() {
	sprintf_s(data_filename, "C:/_/Project/libsib-dev/data/ellip/varying_d/ellip_2d_100_#0.bin");
#endif
	if (!read_file(data_filename, objects, n, d)) {
		printf("Failed to load input points.\n");
		return EXIT_FAILURE;
	}

	auto start = high_resolution_clock::now();

	LIBSIB::solve<Object>(objects, d, n);

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);

	FILE* logfp = stdout;
	if (tolog) fopen_s(&logfp, logfile->c_str(), "a");

	if (logfp) {
		fprintf(logfp, "\n\n---- statistics ----\n");
		fprintf(logfp, "dimension: %d\n", d);
		fprintf(logfp, "number of objects: %d\n", n);
		fprintf(logfp, "final radius: %.6e\n", LIBSIB::get_radius());
		fprintf(logfp, "iteration count: %d\n", LIBSIB::get_iteration());
		fprintf(logfp, "running time: %lld ms\n", duration.count());
	}

	if (d == 2) DEBUG("center: (%.7e, %.7e)\n", LIBSIB::get_center()[0], LIBSIB::get_center()[1]);
	
	return EXIT_SUCCESS;
}
