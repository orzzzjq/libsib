#pragma once
#include <vector>
#include <stdio.h>

namespace IO {
// write a set of AABBs to file
template <typename T>
bool write_aabb(const char* filename, const std::vector<T>& aabbs, int d) {
	FILE* fp;
	fopen_s(&fp, filename, "w");
	if (fp) {
		int n = aabbs.size();
		fprintf(fp, "dimension: %d\n", d);
		fprintf(fp, "number of AABBs: %d\n", n);
		fprintf(fp, "---------------------------------------\n");
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < d; ++j) {
				fprintf(fp, "%.7e %.7e ", aabbs[i][j << 1], aabbs[i][j << 1 | 1]);
			}
			fprintf(fp, "\n");
		}
		return true;
	}
	return false;
}

// write a set of balls to file
template <typename T, typename FT>
bool write_ball(const char* filename, const std::vector<T>& centers, const std::vector<FT>& radii, int d) {
	FILE* fp;
	fopen_s(&fp, filename, "w");
	if (fp) {
		int n = centers.size();
		fprintf(fp, "dimension: %d\n", d);
		fprintf(fp, "number of balls: %d\n", n);
		fprintf(fp, "---------------------------------------\n");
		for (int i = 0; i < n; ++i) {
			fprintf(fp, "%.7e ", radii[i]);
			for (int j = 0; j < d; ++j) {
				fprintf(fp, "%.7e ", centers[i][j]);
			}
			fprintf(fp, "\n");
		}
		return true;
	}
	return false;
}

// write a set of polytopes to file
template <typename T>
bool write_poly(const char* filename, const std::vector<T>& p_points, const std::vector<int>& p_sizes, int d) {
	FILE* fp;
	fopen_s(&fp, filename, "w");
	if (fp) {
		int n = p_sizes.size();
		fprintf(fp, "dimension: %d\n", d);
		fprintf(fp, "number of polytopes: %d\n", n);
		fprintf(fp, "---------------------------------------\n");
		for (int i = 0; i < n; ++i) {
			fprintf(fp, "%d\n", p_sizes[i]);
		}
		for (int i = 0; i < p_points.size(); ++i) {
			for (int j = 0; j < d; ++j) {
				fprintf(fp, "%7e ", p_points[i][j]);
			}
			fprintf(fp, "\n");
		}
		return true;
	}
	return false;
}

// write a set of reduced polytopes to file
template <typename T>
bool write_rpoly(const char* filename, const std::vector<T>& p_points, const std::vector<int>& p_sizes, const std::vector<double>& p_params, int d) {
	FILE* fp;
	fopen_s(&fp, filename, "w");
	if (fp) {
		int n = p_sizes.size();
		fprintf(fp, "dimension: %d\n", d);
		fprintf(fp, "number of polytopes: %d\n", n);
		fprintf(fp, "---------------------------------------\n");
		for (int i = 0; i < n; ++i) {
			fprintf(fp, "%d %.7e\n", p_sizes[i], p_params[i]);
		}
		for (int i = 0; i < p_points.size(); ++i) {
			for (int j = 0; j < d; ++j) {
				fprintf(fp, "%7e ", p_points[i][j]);
			}
			fprintf(fp, "\n");
		}
		return true;
	}
	return false;
}

// write a point set to file
template <typename T>
bool write_ptset(const char* filename, const std::vector<T>& points, int n, int d) {
	FILE* fp;
	fopen_s(&fp, filename, "w");
	if (fp) {
		int n = points.size();
		fprintf(fp, "dimension: %d\n", d);
		fprintf(fp, "number of points: %d\n", n);
		fprintf(fp, "---------------------------------------\n");
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < d; ++j) {
				fprintf(fp, "%7e ", points[i][j]);
			}
			fprintf(fp, "\n");
		}
		return true;
	}
	return false;
}


// read a set of AABBs from file
template <typename T>
bool read_aabb(const char* filename, std::vector<T>& aabbs, int& n, int& d) {
	FILE* fp;
	fopen_s(&fp, filename, "r");
	const int buffer_size = 50000;
	char *buffer = (char*)malloc(sizeof(char) * buffer_size);
	if (fp) {
		// get dimension
		fgets(buffer, buffer_size, fp);
		sscanf_s(buffer, "%*[^:]: %d", &d);
		// get number of aabbs
		fgets(buffer, buffer_size, fp);
		sscanf_s(buffer, "%*[^:]: %d", &n);
		// get separation line
		fgets(buffer, buffer_size, fp);
		// read points
		aabbs.clear();
		std::vector<double> coord(d * 2);
		for (int i = 0; i < n; ++i) {
			fgets(buffer, buffer_size, fp);
			int total = 0, cur;
			for (int j = 0; j < d * 2; ++j) {
				if (sscanf_s(buffer + total, "%lf %n", &coord[j], &cur)) total += cur;
			}
			aabbs.push_back(T(d * 2, coord.begin(), coord.end()));
		}
		return true;
	}
	return false;
}

// read a set of balls from file
template <typename T, typename FT>
bool read_ball(const char* filename, std::vector<T>& centers, std::vector<FT>& radii, int& n, int& d) {
	FILE* fp;
	fopen_s(&fp, filename, "r");
	const int buffer_size = 50000;
	char* buffer = (char*)malloc(sizeof(char) * buffer_size);
	if (fp) {
		// get dimension
		fgets(buffer, buffer_size, fp);
		sscanf_s(buffer, "%*[^:]: %d", &d);
		// get number of balls
		fgets(buffer, buffer_size, fp);
		sscanf_s(buffer, "%*[^:]: %d", &n);
		// get separation line
		fgets(buffer, buffer_size, fp);
		// read points
		radii.clear();
		centers.clear();
		double radius;
		std::vector<double> coord(d);
		for (int i = 0; i < n; ++i) {
			fgets(buffer, buffer_size, fp);
			int total = 0, cur;
			if (sscanf_s(buffer + total, "%lf %n", &radius, &cur)) total += cur;
			for (int j = 0; j < d; ++j) {
				if (sscanf_s(buffer + total, "%lf %n", &coord[j], &cur)) total += cur;
			}
			radii.push_back(radius);
			centers.push_back(T(d, coord.begin(), coord.end()));
		}
		return true;
	}
	return false;
}

// read a set of polytopes from file
template <typename Poly, typename Vector>
bool read_poly(const char* filename, std::vector<Poly>& polys, int& n, int& d) {
	FILE* fp;
	fopen_s(&fp, filename, "r");
	const int buffer_size = 50000;
	char* buffer = (char*)malloc(sizeof(char) * buffer_size);
	if (fp) {
		// get dimension
		fgets(buffer, buffer_size, fp);
		sscanf_s(buffer, "%*[^:]: %d", &d);
		// get number of polys
		fgets(buffer, buffer_size, fp);
		sscanf_s(buffer, "%*[^:]: %d", &n);
		// get separation line
		fgets(buffer, buffer_size, fp);
		// read sizes of point sets
		std::vector<int> p_sizes(n);
		for (int i = 0; i < n; ++i) {
			fgets(buffer, buffer_size, fp);
			sscanf_s(buffer, "%d", &p_sizes[i]);
		}
		// read points
		std::vector<double> coord(d);
		std::vector<Vector> p_points;
		for (int i = 0; i < n; ++i) {
			p_points.clear();
			for (int j = 0; j < p_sizes[i]; ++j) {
				fgets(buffer, buffer_size, fp);
				int total = 0, cur;
				for (int k = 0; k < d; ++k) {
					if (sscanf_s(buffer + total, "%lf %n", &coord[k], &cur)) total += cur;
				}
				p_points.push_back(Vector(d, coord.begin(), coord.end()));
			}
			polys.push_back(Poly(d, p_sizes[i], p_points));
		}
		return true;
	}
	return false;
}

// read a set of reduced polytopes from file
template <typename RPoly, typename Vector>
bool read_rpoly(const char* filename, std::vector<RPoly>& polys, int& n, int& d) {
	FILE* fp;
	fopen_s(&fp, filename, "r");
	const int buffer_size = 50000;
	char* buffer = (char*)malloc(sizeof(char) * buffer_size);
	if (fp) {
		// get dimension
		fgets(buffer, buffer_size, fp);
		sscanf_s(buffer, "%*[^:]: %d", &d);
		// get number of polys
		fgets(buffer, buffer_size, fp);
		sscanf_s(buffer, "%*[^:]: %d", &n);
		// get separation line
		fgets(buffer, buffer_size, fp);
		// read sizes of point sets
		std::vector<int> p_sizes(n);
		std::vector<double> p_params(n);
		for (int i = 0; i < n; ++i) {
			fgets(buffer, buffer_size, fp);
			sscanf_s(buffer, "%d %lf", &p_sizes[i], &p_params[i]);
		}
		// read points
		std::vector<double> coord(d);
		std::vector<Vector> p_points;
		for (int i = 0; i < n; ++i) {
			p_points.clear();
			for (int j = 0; j < p_sizes[i]; ++j) {
				fgets(buffer, buffer_size, fp);
				int total = 0, cur;
				for (int k = 0; k < d; ++k) {
					if (sscanf_s(buffer + total, "%lf %n", &coord[k], &cur)) total += cur;
				}
				p_points.push_back(Vector(d, coord.begin(), coord.end()));
			}
			polys.push_back(RPoly(d, p_sizes[i], p_params[i], p_points));
		}
		return true;
	}
	return false;
}

// read a point set from file
template <typename T>
bool read_ptset(const char* filename, std::vector<T>& points, int& n, int& d) {
	FILE* fp;
	fopen_s(&fp, filename, "r");
	const int buffer_size = 10000;
	char buffer[buffer_size], other[buffer_size];
	if (fp) {
		// get dimension
		fgets(buffer, buffer_size, fp);
		sscanf_s(buffer, "%*[^:]: %d", &d);
		// get number of points
		fgets(buffer, buffer_size, fp);
		sscanf_s(buffer, "%*[^:]: %d", &n);
		// get separation line
		fgets(buffer, buffer_size, fp);
		// read points
		points.clear();
		std::vector<double> coord(d);
		for (int i = 0; i < n; ++i) {
			fgets(buffer, buffer_size, fp);
			int total = 0, cur;
			for (int j = 0; j < d; ++j) {
				if(sscanf_s(buffer + total, "%lf %n", &coord[j], &cur)) total += cur;
			}
			points.push_back(T(d, coord.begin(), coord.end()));
		}
		return true;
	}
	return false;
}
}
