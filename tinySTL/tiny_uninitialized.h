#ifndef TINY_UNINITIALIZED_H_
#define TINY_UNINITIALIZED_H_

#include "tiny_type_traits.h"
#include "tiny_construct.h"
#include "tiny_iterator.h"

namespace tinySTL {

	/*-----------------uninitialized_copy----------------------------------------------------*/
	template <class InputIterator, class ForwardIterator>
	inline ForwardIterator 
	uninitalized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
		return __uninitialized_copy(first, last, result, value_type(first));
	}

	template <class InputIterator, class ForwardIterator, class T>
	inline ForwardIterator 
	__uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T*) {
		typedef typename __type_traits<T>::is_POD_type POD_type;
		return __uninitialized_copy_aux(first, last, result, POD_type());
	}

	template <class InputIterator, class ForwardIterator>
	inline ForwardIterator 
	__uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type) {
		memmove(result, first, (last - first) * sizeof(*first));
		return result + (last - first);
	}

	template <class InputIterator, class ForwardIterator>
	inline ForwardIterator 
	__uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __false_type) {
		while (first < last) {
			construct(&*result, *first);
			++result;
			++first;
		}
		return result;
	}

	/*-------------------uninitialized_fill-----------------------------*/
	template<class ForwardIterator,class T>
	inline void
	uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value) {
		__uninitialized_fill(first, last, value,value_type(first));
	}

	template<class ForwardIterator, class T, class T1>
	inline void
	__uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value, T1*) {
		typedef typename __type_traits<T1>::is_POD_type POD_type;
		__uninitialized_fill_aux(first, last, value,POD_type());
	}

	template<class ForwardIterator, class T>
	inline void
	__uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, __true_type) {
		//TODO:这里在完成算法之后可以改成相应的算法
		while (first < last) {
			*first = value;
			++first;
		}
	}

	template<class ForwardIterator, class T>
	inline void
	__uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, __false_type) {
		while (first < last) {
			construct(&*first, value);
			++first;
		}
	}

	/*-------------------uninitialized_fill_n-----------------------------*/
	template<class ForwardIterator, class Size, class T>
	inline ForwardIterator
	uninitialized_fill_n(ForwardIterator first, Size n, const T& value) {
		return __uninitialized_fill_n(first, n, value, value_type(first));
	}

	template<class ForwardIterator, class Size, class T, class T1>
	inline ForwardIterator
	__uninitialized_fill_n(ForwardIterator first, Size n, const T& value, T1*) {
		typedef typename __type_traits<T1>::is_POD_type POD_type;
		return __uninitialized_fill_n_aux(first, n, value, POD_type());
	}

	template<class ForwardIterator, class Size, class T>
	inline ForwardIterator
	__uninitialized_fill_n(ForwardIterator first, Size n, const T& value, __false_type) {
		for (int i = 0; i < N; ++i) {
			*first = value;
			++first;
		}
		return first;
	}

	template<class ForwardIterator, class Size, class T>
	inline ForwardIterator
	__uninitialized_fill_n(ForwardIterator first, Size n, const T& value, __true_type) {
		for (int i = 0; i < N; ++i) {
			construct(&*first, value);
			++first;
		}
		return first;
	}
}



#endif
