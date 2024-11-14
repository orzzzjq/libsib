#pragma once
#include "Vector_d.h"
#include "Object.h"

namespace DS {
	template <typename FT>
	class Point_d : public Vector_d<FT>, public Object<FT> {
	public:
		Point_d() {}
		Point_d(int d) : Vector_d<FT>(d) {}
		template <typename InputIterator>
		Point_d(int d, InputIterator begin, InputIterator end) : Vector_d<FT>(d, begin, end) {}

		// minimize a linear function <h,x> (indeed, return the point)
		void minimize(const Vector_d<FT>& h, Vector_d<FT>& x) const {
			x.copy(*this);
		}
	};
}
