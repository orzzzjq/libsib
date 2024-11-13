#pragma once
#include "Vector_d.h"

namespace DS {
	template <typename FT>
	class Object {
	public:
		//virtual ~Object() {};
		Object() {}
		virtual void minimize(const Vector_d<FT>& h, Vector_d<FT>& x) const { }
	};
}
