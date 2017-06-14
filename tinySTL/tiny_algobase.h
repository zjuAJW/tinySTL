#ifndef TINY_ALGOBASE_H_
#define TINY_ALGOBASE_H_

#include <cstring>
#include "tiny_type_traits.h"
#include "tiny_iterator.h"

namespace tinySTL {
	/*-----------------------------copy-------------------------*/

	template<class InputIterator, class OutputIterator>
	OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
		return __copy_dispatch(first, last, result);
	}

	//inline char* copy(char * first, char * last, char* result) {
	//	memmove(result,first,last - first);
	//	return result + (last - first);
	//}
	//
	//inline wchar_t* copy(wchar_t * first, wchar_t * last, wchar_t* result) {
	//	memmove(result, first, sizeof(wchar_t)*(last - first));
	//	return result + (last - first);
	//}

	template<class InputIterator, class OutputIterator>
	OutputIterator __copy_dispatch(InputIterator first, InputIterator last, OutputIterator result) {
		return __copy(first, last, result, iterator_category(first));
	}

	template<class T>
	T* __copy_dispatch(T* first, T* last, T* result) {
		typedef typename __type_traits<T>::has_trivial_assignment_operator trivial_assignment;
		return __copy_t(first, last, result, trivial_assignment());
	}

	template<class T>
	T* __copy_dispatch(const T* first, T* last, T* result) {
		typedef typename __type_traits<T>::has_trivial_assignment_operator trivial_assignment;
		return __copy_t(first, last, result, trivial_assignment());
	}

	template<class InputIterator, class OutputIterator>
	OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag) {
		for (; first != last; ++first, ++result)
			*result = *first;
		return result;
	}

	template<class RandomAccessIterator, class OutputIterator>
	OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_access_iterator_tag) {
		return __copy_d(first, last, result, distance_type(first));
	}

	template<class InputIterator, class OutputIterator, class Distance>
	OutputIterator __copy_d(InputIterator first, InputIterator last, OutputIterator result, Distance*) {
		Distance n = last - first;
		for (; n > 0; --n, ++result, ++first)
			*result = *first;
		return result;
	}

	template<class T>
	T* __copy_t(const T* first, const T* last, T* result, __true_type) {
		memmove(result, first, sizeof(T) * (last - first));
		return result + (last - first);
	}

	template<class T>
	T* __copy_t(const T* first, const T* last, T* result, __false_type) {
		return __copy_d(first, last, result, ptrdiff_t*(0));
	}

	/*-----------------------------copy_backward-------------------------*/

	template<class BidirectionalIterator1, class BidirectionalIterator2>
	BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result) {
		return __copy_backward_dispatch(first, last, result);
	}

	//inline char* copy_backward(char * first, char * last, char* result) {
	//	memmove(result, first, last - first);
	//	return result + (last - first);
	//}
	//
	//inline wchar_t* copy_backward(wchar_t * first, wchar_t * last, wchar_t* result) {
	//	memmove(result, first, sizeof(wchar_t)*(last - first));
	//	return result + (last - first);
	//}

	template<class BidirectionalIterator1, class BidirectionalIterator2>
	BidirectionalIterator2 __copy_backward_dispatch(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result) {
		return __copy_backward(first, last, result, iterator_category(first));
	}

	template<class T>
	T* __copy_backward_dispatch(T* first, T* last, T* result) {
		typedef typename __type_traits<T>::has_trivial_assignment_operator trivial_assignment;
		return __copy_backward_t(first, last, result, trivial_assignment());
	}

	template<class T>
	T* __copy_backward_dispatch(const T* first, T* last, T* result) {
		typedef typename __type_traits<T>::has_trivial_assignment_operator trivial_assignment;
		return __copy_backward_t(first, last, result, trivial_assignment());
	}

	template<class BidirectionalIterator1, class BidirectionalIterator2>
	BidirectionalIterator2 __copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result, bidirectional_iterator_tag) {
		while (first != last;)
			*--result = *--last;
		return result;
	}

	template<class RandomAccessIterator, class BidirectionalIterator>
	BidirectionalIterator __copy_backward(RandomAccessIterator first, RandomAccessIterator last, BidirectionalIterator result, random_access_iterator_tag) {
		return __copy_backward_d(first, last, result, distance_type(first));
	}

	template<class BidirectionalIterator1, class BidirectionalIterator2, class Distance>
	BidirectionalIterator2 __copy_backward_d(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result, Distance*) {
		Distance n = last - first;
		for(; n > 0; --n)
			*--result = *--last;
		return result;
	}

	template<class T>
	T* __copy_backward_t(const T* first, const T* last, T* result, __true_type) {
		memmove(result, first, sizeof(T) * (last - first));
		return result - (last - first);
	}

	template<class T>
	T* __copy_backward_t(const T* first, const T* last, T* result, __false_type) {
		return __copy_backward_d(first, last, result, ptrdiff_t*(0));
	}
}


#endif