#pragma once
#include <vector>
#include <stdio.h>

namespace IO {
template <typename T>
bool write_points(const char* filename, const std::vector<T>& points, int d) {
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

template <typename T>
bool read_points(const char* filename, std::vector<T>& points, int& n, int& d) {
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
