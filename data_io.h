#pragma once
#include <vector>
#include <stdio.h>
#include <fstream>
#include <Eigen/Dense>

namespace IO {
typedef Eigen::MatrixXd Matrix;

// write a set of AABBs to text file
template <typename T>
bool write_aabb_txt(const char* filename, const std::vector<T>& aabbs, int d) {
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

// write a set of AABBs to binary file
template <typename T>
bool write_aabb(const char* filename, const std::vector<T>& aabbs, int d) {
	int n = aabbs.size();
	std::ofstream out(filename, std::ios::out | std::ios::binary | std::ios::trunc);
	if (!out.fail()) {
		out.write((char*)&d, sizeof(int));
		out.write((char*)&n, sizeof(int));
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < d * 2; ++j) {
				double x = aabbs[i][j];
				out.write((char*)(&x), sizeof(double));
			}
		}
		out.close();
		return true;
	}
	return false;
}

// write a set of balls to text file
template <typename T, typename FT>
bool write_ball_txt(const char* filename, const std::vector<T>& centers, const std::vector<FT>& radii, int d) {
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

// write a set of balls to binary file
template <typename T, typename FT>
bool write_ball(const char* filename, const std::vector<T>& centers, const std::vector<FT>& radii, int d) {
	int n = centers.size();
	std::ofstream out(filename, std::ios::out | std::ios::binary | std::ios::trunc);
	if (!out.fail()) {
		out.write((char*)&d, sizeof(int));
		out.write((char*)&n, sizeof(int));
		for (int i = 0; i < n; ++i) {
			double x = radii[i];
			out.write((char*)(&x), sizeof(double));
			for (int j = 0; j < d; ++j) {
				x = centers[i][j];
				out.write((char*)(&x), sizeof(double));
			}
		}
		out.close();
		return true;
	}
	return false;
}

// write a set of polytopes to text file
template <typename T>
bool write_poly_txt(const char* filename, const std::vector<T>& p_points, const std::vector<int>& p_sizes, int d) {
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

// write a set of polytopes to binary file
template <typename T>
bool write_poly(const char* filename, const std::vector<T>& p_points, const std::vector<int>& p_sizes, int d) {
	int n = p_sizes.size();
	std::ofstream out(filename, std::ios::out | std::ios::binary | std::ios::trunc);
	if (!out.fail()) {
		out.write((char*)&d, sizeof(int));
		out.write((char*)&n, sizeof(int));
		for (int i = 0; i < n; ++i) {
			int x = p_sizes[i];
			out.write((char*)(&x), sizeof(int));
		}
		for (int i = 0; i < p_points.size(); ++i) {
			for (int j = 0; j < d; ++j) {
				double x = p_points[i][j];
				out.write((char*)(&x), sizeof(double));
			}
		}
		out.close();
		return true;
	}
	return false;
}

// write a set of reduced polytopes to text file
template <typename T>
bool write_rpoly_txt(const char* filename, const std::vector<T>& p_points, const std::vector<int>& p_sizes, const std::vector<double>& p_params, int d) {
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

// write a set of reduced polytopes to binary file
template <typename T>
bool write_rpoly(const char* filename, const std::vector<T>& p_points, const std::vector<int>& p_sizes, const std::vector<double>& p_params, int d) {
	int n = p_sizes.size();
	std::ofstream out(filename, std::ios::out | std::ios::binary | std::ios::trunc);
	if (!out.fail()) {
		out.write((char*)&d, sizeof(int));
		out.write((char*)&n, sizeof(int));
		for (int i = 0; i < n; ++i) {
			int x = p_sizes[i];
			out.write((char*)(&x), sizeof(int));
			double y = p_params[i];
			out.write((char*)(&y), sizeof(double));
		}
		for (int i = 0; i < p_points.size(); ++i) {
			for (int j = 0; j < d; ++j) {
				double x = p_points[i][j];
				out.write((char*)(&x), sizeof(double));
			}
		}
		out.close();
		return true;
	}
	return false;
}

// write a set of ellipsoids to text file
template <typename T>
bool write_ellip_txt(const char* filename, const std::vector<T>& centers, const std::vector<Matrix>& mats, int d) {
	FILE* fp;
	fopen_s(&fp, filename, "w");
	if (fp) {
		int n = centers.size();
		fprintf(fp, "dimension: %d\n", d);
		fprintf(fp, "number of ellipsoids: %d\n", n);
		fprintf(fp, "---------------------------------------\n");
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < d; ++j) {
				fprintf(fp, "%.7e ", centers[i][j]);
			}
			fprintf(fp, "\n");
		}
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < d; ++j) {
				for (int k = 0; k < d; ++k) {
					fprintf(fp, "%.7e ", mats[i].coeff(j, k));
				}
				fprintf(fp, "\n");
			}
		}
		return true;
	}
	return false;
}

// write a set of ellipsoids to binary file
template <typename T>
void write_ellip(const char* filename, const std::vector<T>& centers, const std::vector<Matrix>& mats, int d) {
	int n = centers.size();
	std::ofstream out(filename, std::ios::out | std::ios::binary | std::ios::trunc);
	out.write((char*)&d, sizeof(int));
	out.write((char*)&n, sizeof(int));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < d; ++j) {
			double x = centers[i][j];
			out.write((char*)&x, sizeof(double));
		}
	}
	for (int i = 0; i < n; ++i) {
		out.write((char*)mats[i].data(), d * d * sizeof(typename Matrix::Scalar));
	}
	out.close();
}

// read a set of AABBs from binary file
template <typename AABB, typename Vector>
bool read_aabb(const char* filename, std::vector<AABB>& aabbs, int& n, int& d) {
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (!in.fail()) {
		in.read((char*)(&d), sizeof(int));
		in.read((char*)(&n), sizeof(int));
		std::vector<double> coord(d * 2);
		Vector data;
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < d * 2; ++j) {
				in.read((char*)(&coord[j]), sizeof(double));
			}
			data = Vector(d * 2, coord.begin(), coord.end());
			aabbs.push_back(AABB(d, data));
		}
		in.close();
		return true;
	}
	return false;
}

// read a set of balls from binary file
template <typename Ball, typename Vector >
bool read_ball(const char* filename, std::vector<Ball>& balls, int& n, int& d) {
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (!in.fail()) {
		in.read((char*)(&d), sizeof(int));
		in.read((char*)(&n), sizeof(int));
		Vector center;
		double radius;
		std::vector<double> coord(d);
		for (int i = 0; i < n; ++i) {
			in.read((char*)(&radius), sizeof(double));
			for (int j = 0; j < d; ++j) {
				in.read((char*)(&coord[j]), sizeof(double));
			}
			center = Vector(d, coord.begin(), coord.end());
			balls.push_back(Ball(d, center, radius));
		}
		in.close();
		return true;
	}
	return false;
}

// read a set of polytopes from binary file
template <typename Poly, typename Vector>
bool read_poly(const char* filename, std::vector<Poly>& polys, int& n, int& d) {
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (!in.fail()) {
		in.read((char*)(&d), sizeof(int));
		in.read((char*)(&n), sizeof(int));
		std::vector<int> p_sizes(n);
		for (int i = 0; i < n; ++i) {
			in.read((char*)(&p_sizes[i]), sizeof(int));
		}
		std::vector<double> coord(d);
		std::vector<Vector> p_points;

		for (int i = 0; i < n; ++i) {
			p_points.clear();
			for (int j = 0; j < p_sizes[i]; ++j) {
				for (int k = 0; k < d; ++k) {
					in.read((char*)(&coord[k]), sizeof(double));
				}
				p_points.push_back(Vector(d, coord.begin(), coord.end()));
			}
			polys.push_back(Poly(d, p_sizes[i], p_points));
		}
		in.close();
		return true;
	}
	return false;
}

// read a set of reduced polytopes from binary file
template <typename RPoly, typename Vector>
bool read_rpoly(const char* filename, std::vector<RPoly>& rpolys, int& n, int& d) {
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (!in.fail()) {
		in.read((char*)(&d), sizeof(int));
		in.read((char*)(&n), sizeof(int));
		std::vector<int> p_sizes(n);
		std::vector<double> p_params(n);
		for (int i = 0; i < n; ++i) {
			in.read((char*)(&p_sizes[i]), sizeof(int));
			in.read((char*)(&p_params[i]), sizeof(double));
		}

		std::vector<double> coord(d);
		std::vector<Vector> p_points;
		for (int i = 0; i < n; ++i) {
			p_points.clear();
			for (int j = 0; j < p_sizes[i]; ++j) {
				for (int k = 0; k < d; ++k) {
					in.read((char*)(&coord[k]), sizeof(double));
				}
				p_points.push_back(Vector(d, coord.begin(), coord.end()));
			}
			rpolys.push_back(RPoly(d, p_sizes[i], p_params[i], p_points));
		}
		in.close();
		return true;
	}
	return false;
}



// read a set of ellipsoids from binary file
template <typename Ellip, typename Vector>
bool read_ellip(const char* filename, std::vector<Ellip>& ellips, int& n, int& d) {
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (!in.fail()) {
		in.read((char*)(&d), sizeof(int));
		in.read((char*)(&n), sizeof(int));
		std::vector<double> coord(d);
		std::vector<Vector> centers;
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < d; ++j) {
				in.read((char*)(&coord[j]), sizeof(double));
			}
			centers.push_back(Vector(d, coord.begin(), coord.end()));
		}
		Matrix Q(d, d);
		for (int i = 0; i < n; ++i) {
			in.read((char*)Q.data(), d * d * sizeof(typename Matrix::Scalar));
			ellips.push_back(Ellip(d, centers[i], Q));
		}
		in.close();
		return true;
	}
	return false;
}
}
