#include <CGAL/Cartesian_d.h>
#include <CGAL/Min_sphere_of_points_d_traits_d.h>
#include <CGAL/Min_sphere_of_spheres_d.h>
#include <CGAL/Random.h>
#include <CGAL/IO/read_points.h>
#include <CGAL/IO/write_points.h>

#include "points_io.h"

#include <iostream>
#include <cstdlib>
#include <chrono>

typedef CGAL::Cartesian_d<double> K;
typedef K::Point_d Point;

int n = 10, d = 1024, no = 0;

int main()
{
	char filename[500];
	CGAL::Random r;
	std::vector<Point> p, q;
	
	//for (d = 2; d <= (1 << 9); d <<= 1) {
		printf("%d\n", d);
		std::vector<double> coord(d);
		//for (int no = 0; no < 10; ++no) {
			p.clear();
			sprintf_s(filename, "C:/Projects/SoCG24_data/SmallestEnclosingSphere/data/d_64/ses_d_%d_n_%d_#%d.txt", d, n, no);
			for (int i = 0; i < n; ++i) {
				for (int j = 0; j < d; ++j) {
					coord[j] = r.get_double();
				}
				p.push_back(Point(d, coord.begin(), coord.end()));
			}

			if (!IO::write_points(filename, p, d))
				return EXIT_FAILURE;
			puts(filename);
		//}
	//}


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
