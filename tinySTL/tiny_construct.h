#ifndef CONSTRUCT_H_
#define CONSTRUCT_H_

#include <new.h>
#include "tiny_type_traits.h"

namespace tinySTL {
	template <typename T1, typename T2>
	void construct(T1* p, const T2& value) {
		new(p) T1(value);
	}

	template<typename T>
	void destory(T* p) {
		p->~T();
	}

	template<typename ForwardInterator>
	void destory(ForwardInterator first, ForwardInterator last) {
		__destory(first, last, value_type(first));
	}

	template<typename ForwardInterator, typename T>
	inline void __destory(ForwardInterator first, ForwardInterator last, T* p) {
		typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
		__destory_aux(first, last, trivial_destructor());
	}

	template<typename ForwardInterator>
	inline void __destory_aux(ForwardInterator first, ForwardInterator last, __true_type) {}


	template<typename ForwardInterator>
	inline void __destory_aux(ForwardInterator first, ForwardInterator last, __false_type) {
		while (first < last) {
			destory(&*first);
		}
	}

	
}





#endif

