#pragma once
#include <CGAL/Cartesian_d.h>
#include <CGAL/Min_sphere_of_points_d_traits_d.h>
#include <CGAL/Min_sphere_of_spheres_d.h>

#include "Vector_d.h"

namespace CGAL {
	typedef CGAL::Cartesian_d<double> Kernel; 
	typedef CGAL::Min_sphere_of_points_d_traits_d<Kernel, double, 30> Traits;
	typedef CGAL::Min_sphere_of_spheres_d<Traits> Min_sphere;
	typedef Kernel::Point_d Point;

	double _radius;
	LA::Vector_d<double> center;

	double get_radius() {
		return _radius;
	}

	LA::Vector_d<double>& get_center() {
		return center;
	}

	template <typename Vector>
	void solve(const std::vector<Vector>& points, int n, int d) {
		std::vector<Point> v;
		for (int i = 0; i < points.size(); ++i) {
			v.push_back(Point(d, points[i].cartesian_begin(), points[i].cartesian_end()));
		}
		Min_sphere ms(v.begin(), v.end());
		CGAL::_radius = ms.radius();
		CGAL::center = LA::Vector_d<double>(d, ms.center_cartesian_begin(), ms.center_cartesian_end());
	}
}

