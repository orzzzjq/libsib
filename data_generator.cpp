//#include <CGAL/Cartesian_d.h>
//#include <CGAL/Min_sphere_of_points_d_traits_d.h>
//#include <CGAL/Min_sphere_of_spheres_d.h>
//#include <CGAL/Random.h>
//#include <CGAL/IO/read_points.h>
//#include <CGAL/IO/write_points.h>
#include "Vector_d.h"

#include <boost/random.hpp>
#include <boost/random/uniform_on_sphere.hpp>

#include "data_io.h"

#include <iostream>
#include <cstdlib>
#include <chrono>

typedef DS::Vector_d<double> Point;

typedef boost::random::mt19937 gen_type;
gen_type rand_gen;


// generate a random point on unit sphere
Point direction_gen(int d) {
	boost::uniform_on_sphere<double> unif_sphere(d);
	boost::variate_generator<gen_type&, boost::uniform_on_sphere<double>> random_on_sphere(rand_gen, unif_sphere);
	std::vector<double> coord = random_on_sphere();
	return Point(d, coord.begin(), coord.end());
}

// generate unit cubes
/*
	data structure : every AABB is a (d*2)-dimensional Point
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

		// check the norm of the closest point to the origin
		//double smallest = std::numeric_limits<double>::max();
		//for (int i = 0; i < n; ++i) {
		//	double sum = 0;
		//	for (int j = 0; j < d; ++j) {
		//		if (fabs(aabbs[i][j << 1]) < fabs(aabbs[i][j << 1 | 1])) {
		//			sum += aabbs[i][j << 1] * aabbs[i][j << 1];
		//		}
		//		else {
		//			sum += aabbs[i][j << 1 | 1] * aabbs[i][j << 1 | 1];
		//		}
		//	}
		//	smallest = std::min(smallest, sum);
		//}
		//printf("smallset norm: %f, sqrt(d)/2: %f\n", sqrt(smallest), sqrt(d) / 2.0);
	}
}

int main()
{
	int d = 2, n = 1000, num = 1;

	rand_gen.seed(std::time(0));

	for (d = 2; d < 1025; d <<= 1) {
		aabb_gen("C:/_/Project/libsib-dev/data/aabb", d, n, num);
	}

	//char filename[500];
	//CGAL::Random r;

	//std::vector<Point> p, q;

	////for (d = 2; d <= (1 << 9); d <<= 1) {
	//	printf("%d\n", d);
	//	std::vector<double> coord(d);
	//	//for (int no = 0; no < 10; ++no) {
	//		p.clear();
	//		sprintf_s(filename, "C:/_/Project/libsib-dev/data/ptset/ptset_%dd_%d_#%d.txt", d, n, no);
	//		for (int i = 0; i < n; ++i) {
	//			for (int j = 0; j < d; ++j) {
	//				coord[j] = r.get_double(-1, 1);
	//			}
	//			p.push_back(Point(d, coord.begin(), coord.end()));
	//		}

	//		if (!IO::write_points(filename, p, d))
	//			return EXIT_FAILURE;
	//		puts(filename);
	//	//}
	////}


	//// Test read_points()
	//int nn, dd;
	//if (!IO::read_points(filename, q, nn, dd))
	//	return EXIT_FAILURE;

	//assert(nn == n);
	//assert(dd == d);
	//for (int i = 0; i < n; ++i) {
	//	for (int j = 0; j < d; ++j) {
	//		if (fabs(p[i][j] - q[i][j]) > 1e-7) {
	//			printf("Failed, difference: %e\n", fabs(p[i][j] - q[i][j]));
	//			return EXIT_FAILURE;
	//		}
	//	}
	//}

	return 0;
}
