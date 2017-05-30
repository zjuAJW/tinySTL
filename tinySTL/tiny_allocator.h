#pragma once

#ifndef TINY_ALLOCATOR_H_
#define TINY_ALLOCATOR_H_

#include "tiny_alloc.h"

namespace tinySTL {
	template<class T, class alloc> class Allocator {
	public:
		static T* allocate(size_t n) {
			return n == 0 ? 0 : (T*)alloc::allocate(n * sizeof(T));
		}

		static T* allocate() {
			return (T*)alloc::allocate(sizeof(T))
		}

		static void deallocate(T* p, size_t n) {
			if (n != 0)
				alloc::deallocate(p, n * sizeof(T));
		}

		static void deallocate(T* p) {
			alloc::deallocate(p, sizeof(T));
		}
	};
}



#endif
