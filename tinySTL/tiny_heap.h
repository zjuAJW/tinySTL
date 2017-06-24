#ifndef TINY_HEAP_H_
#define TINY_HEAP_H_
#include "tiny_type_traits.h"

namespace tinySTL {

	//push_heap
	template<class RandomAccessIterator>
	inline void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
		__push_heap_aux(first,last,distance_type(first),value_type(first));
	}

	template<class RandomAccessIterator,class Distance, class T>
	inline void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*) {
		__push_heap(first, Distance(last - first - 1), Distance(0), T(*(last - 1)));
	}

	template<class RandomAccessIterator, class T, class Distance>
	inline void __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value) {
		auto parent = (holeIndex - 1) / 2;
		while (holeIndex > topIndex && *(first + parent) < value) {
			*(first + holeIndex) = *(first + parent);
			holeIndex = parent;
			parent = (holeIndex - 1) / 2;
		}
		*(first + holeIndex) = value;
	}

	//push_heap with compare
	template<class RandomAccessIterator, class Compare>
	inline void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
		__push_heap_aux(first, last, comp, distance_type(first), value_type(first));
	}

	template<class RandomAccessIterator, class Distance, class T, class Compare>
	inline void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Compare comp, Distance*, T*) {
		__push_heap(first, Distance(last - first - 1), Distance(0), T(*(last - 1)),comp);
	}

	template<class RandomAccessIterator, class T, class Distance, class Compare>
	inline void __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value, Compare comp) {
		auto parent = (holeIndex - 1) / 2;
		while (holeIndex > topIndex && comp(*(first + parent), value)) {
			*(first + holeIndex) = *(first + parent);
			holeIndex = parent;
			parent = (holeIndex - 1) / 2;
		}
		*(first + holeIndex) = value;
	}

	//pop_heap
	template<class RandomAccessIterator>
	inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
		__pop_heap_aux(first, last, value_type(first));
	}


	template<class RandomAccessIterator, class T>
	inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*) {
		__pop_heap(first, last - 1, last - 1, distance_type(first), T(*(last - 1)));
	}

	template<class RandomAccessIterator, class Distance, class T>
	inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, Distance *, T value) {
		*result = *first;
		__adjust_heap(first, Distance(0),Distance(last - first), value);
	}

	template<class RandomAccessIterator, class Distance, class T>
	inline void __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value) {
		auto child = (holeIndex + 1) * 2;
		while (child <= len) {
			if (child == len || *(first + child) < *(first + child - 1))
				--child;
			if (*(first + child) < value) {
				break;
			}
			else {
				*(first + holeIndex) = *(first + child);
				holeIndex = child;
				child = (holeIndex + 1) * 2;
			}
		}
		*(first + holeIndex) = value;
	}

	//pop heap with compare
	template<class RandomAccessIterator, class Compare>
	inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
		__pop_heap_aux(first, last, comp,value_type(first));
	}


	template<class RandomAccessIterator, class T, class Compare>
	inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Compare comp, T*) {
		__pop_heap(first, last - 1, last - 1, comp, distance_type(first), T(*(last - 1)));
	}

	template<class RandomAccessIterator, class Distance, class T, class Compare>
	inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, Compare comp, Distance *, T value) {
		*result = *first;
		__adjust_heap(first, Distance(0), Distance(last - first), value, comp);
	}

	template<class RandomAccessIterator, class Distance, class T, class Compare>
	inline void __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value, Compare comp) {
		auto child = (holeIndex + 1) * 2;
		while (child <= len) {
			if (child == len || comp(*(first + child), *(first + child - 1)))
				--child;
			if (comp(*(first + child), value)) {
				break;
			}
			else {
				*(first + holeIndex) = *(first + child);
				holeIndex = child;
				child = (holeIndex + 1) * 2;
			}
		}
		*(first + holeIndex) = value;
	}
	//sort_heap
	template<class RandomAccessIterator>
	void sort_heap(RandomAccessIterator first, RandomAccessIterator last) {
		while (last - first > 1) 
			pop_heap(first, last--);
	}

	template<class RandomAccessIterator, class Compare>
	void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
		while (last - first > 1)
			pop_heap(first, last--, comp);
	}

	//make_heap
	template<class RandomAccessIterator>
	inline void make_heap(RandomAccessIterator first, RandomAccessIterator last) {
		__make_heap(first,last,distance_type(first),value_type(first));
	}

	template<class RandomAccessIterator, class Distance, class T>
	inline void __make_heap(RandomAccessIterator first, RandomAccessIterator last, Distance*,T*) {
		auto len = last - first;
		if (len < 2) return;
		auto holeIndex = (len - 1 - 1) / 2;
		while (holeIndex >= 0) {
			__adjust_heap(first, holeIndex, len, *(first + holeIndex));
			--holeIndex;
		}
	}

	//make_heap with compare
	template<class RandomAccessIterator, class Compare>
	inline void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
		__make_heap(first, last, comp, distance_type(first), value_type(first));
	}

	template<class RandomAccessIterator, class Distance, class T, class Compare>
	inline void __make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp, Distance*, T*) {
		auto len = last - first;
		if (len < 2) return;
		auto holeIndex = (len - 1 - 1) / 2;
		while (holeIndex >= 0) {
			__adjust_heap(first, holeIndex, len, *(first + holeIndex), comp);
			--holeIndex;
		}
	}


}



#endif
