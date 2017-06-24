#pragma once
#ifndef TINY_FUNCTIONAL_H_
#define TINY_FUNCTIONAL_H_

namespace tinySTL {

	template<class T>
	class less{
	public:
		bool operator()(const T& lhs, const T& rhs) {
			return lhs < rhs;
		}
	};
}

#endif
