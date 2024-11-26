#include <iostream>
#include <cstdlib>
#include <chrono>
#include <string>

using namespace std::chrono;

typedef double FT;

#include "LIBSIB/debug.h"
#include "LIBSIB/data_io.h"
#include "LIBSIB/libsib.h"

#include "LIBSIB/DS/Vector_d.h"
#include "LIBSIB/DS/Object.h"
#include "LIBSIB/DS/Point_d.h"
#include "LIBSIB/DS/Polytope_d.h"
#include "LIBSIB/DS/ReducedPolytope_d.h"
#include "LIBSIB/DS/AABB_d.h"
#include "LIBSIB/DS/Ball_d.h"
#include "LIBSIB/DS/Ellipsoid_d.h"

typedef DS::Vector_d<FT> Vector;
typedef DS::Object<FT> Object;

std::vector<Object*> objects;
bool (*read_file)(const char*, std::vector<Object*>&, int&, int&) = NULL;

bool tolog = false;
std::string type, *path, *file, *logfile;
char data_filename[500];
int d, n, no = 0;

#define args
#ifdef args
void print_help() {
	puts("LIBSIB: Compute smallest intersecting balls in arbitrary dimensions.\n");
	puts("How to use:");
	puts("  libsib [type] -f [file] [*options]\n");
	puts("Arguments:");
	puts("  -h: print help page");
	puts("  [type]: type of objects to be intersected, currently support");
	puts("          ©Ä point: a set of points");
	puts("          ©Ä poly: a set of convex polytopes");
	puts("          ©Ä rpoly: a set of reduced polytopes");
	puts("          ©Ä aabb: a set of axis-aligned bounding boxes");
	puts("          ©Ä ball: a set of Euclidean balls");
	puts("          ©º ellip: a set of ellipsoids");
	puts("  -f [file]: (compulsory) specify the name of the data file, usually a binary file *.bin");
	puts("  -p [path]: (optional) specify the path to the input data, the current directory by default");
	puts("  -l [logfile]: (optional) output the statistics to a log file\n");
}

bool parse(int argc, char** argv) {
	if (argc < 4) {
		print_help();
		return false;
	}

	type = std::string(argv[1]);
	if (type == "point") read_file = IO::read_pset<DS::Point_d<FT>, Vector>;
	else if (type == "poly") read_file = IO::read_poly<DS::Polytope_d<FT>, Vector>;
	else if (type == "rpoly") read_file = IO::read_rpoly<DS::ReducedPolytope_d<FT>, Vector>;
	else if (type == "aabb") read_file = IO::read_aabb<DS::AABB_d<FT>, Vector>;
	else if (type == "ball") read_file = IO::read_ball<DS::Ball_d<FT>, Vector>;
	else if (type == "ellip") read_file = IO::read_ellip<DS::Ellipsoid_d<FT>, Vector>;
	else {
		printf("Error: LIBSIB does not support object type %s.\n", type.c_str());
		puts("---");
		print_help();
		return false;
	}

	for (int i = 2; i < argc; i += 2) {
		if (strlen(argv[i]) < 2) {
			print_help();
			return false;
		}
		switch (argv[i][1]) {
		case 'h': print_help(); return false;
		case 'p': path = new std::string(argv[i + 1]); break;
		case 'f': file = new std::string(argv[i + 1]); break;
		case 'l': logfile = new std::string(argv[i + 1]); tolog = true; break;
		default: return false;
		}
	}

	if (path == NULL) {
		path = new std::string(".");
	}

	return true;
}

int main(int argc, char** argv) {
	if (!parse(argc, argv)) {
		//print_help();
		return EXIT_FAILURE;
	}

	sprintf_s(data_filename, "%s/%s", path->c_str(), file->c_str());
#else
int main() {
	read_file = IO::read_ellip<DS::Ellipsoid_d<FT>, Vector>;
	sprintf_s(data_filename, "C:/_/Project/libsib-dev/data/visual/ellip_2d_8_#0.bin");
#endif
	if (!read_file || !read_file(data_filename, objects, n, d)) {
		printf("Error: Failed to load the data in %s.\n", data_filename);
		puts("---");
		print_help();
		return EXIT_FAILURE;
	}

	LIBSIB::set_gap(1e-4);
#ifdef TRAJECTORY
	LIBSIB::set_trajectory_file("../visualizer/ellip_trajectory.txt");
#endif

	auto start = high_resolution_clock::now();

	LIBSIB::solve(objects, d, n);

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);

	FILE* logfp = stdout;
	if (tolog) fopen_s(&logfp, logfile->c_str(), "a");

	if (logfp) {
		DEBUG("\n\n");
		fprintf(logfp, "---- statistics ----\n");
		fprintf(logfp, "type of object: %s\n", type.c_str());
		fprintf(logfp, "dimension: %d\n", d);
		fprintf(logfp, "number of objects: %d\n", n);
		fprintf(logfp, "final radius: %.6e\n", LIBSIB::get_radius());
		fprintf(logfp, "iteration count: %d\n", LIBSIB::get_iteration());
		fprintf(logfp, "running time: %lld ms\n", duration.count());
	}

	if (d == 2) DEBUG("center: (%.7e, %.7e)\n", LIBSIB::get_center()[0], LIBSIB::get_center()[1]);
	
	return EXIT_SUCCESS;
}
