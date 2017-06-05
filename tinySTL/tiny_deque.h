#ifndef TINY_DEQUE_H_
#define TINY_DEQUE_H_

#include "tiny_iterator.h"

namespace tinySTL {
	inline size_t deque_buf_size(size_t s) {
		return s > 512 ? 1 : 512 / s;
	}
	template<class T>
	struct deque_iterator : public iterator<random_access_iterator_tag, T>{
		typedef deque_iterator<T> self;
		typedef T** map_pointer;
		typedef size_t size_type;

		size_type buffer_size() { return deque_buf_size(sizeof(T); }
		void set_node(map_pointer new_node) {
			node = new_node;
			first = *new_node;
			last = first + difference_type(buffer_size());
		}

		pointer cur, first, last;
		map_pointer node;

		reference operator*() { return *cur; }
		pointer operator->() { return &(operator*()); }

		difference_type operator-(const self& x) {
			return difference_type((node - x.node - 1) * buffer_size()) + (cur - first) + (x.last - x.cur);
		}
		self& operator++();
		self operator++(int);
		self& operator--();
		self operator--(int);
		self& operator+=(difference_type n);
		self operator+(defference_type n);
		self& operator-=(difference_type n);
		self operator-(defference_type n);

		reference operator[](difference_type n){ return *(*this + n); }

		bool operator==(const self& x) const { return cur == x.cur; }
		bool operator!=(const self& x) const { return !(*this = x); }
		bool operator<(const self& x) const {
			return x.node == node ? (cur < x.cur) : (node < x.node);
		}

	};

	template<class T, class alloc = Alloc>
	class deque {
	public:
		typedef T value_type;
		typedef value_type* pointer;
		typedef T& reference;
		typedef size_t size_type;
		typedef deque_iterator<T> iterator;

	protected:
		typedef pointer* map_pointer;
		map_pointer map;
		size_type map_size;
		iterator start;
		iterator finish;

	public:
		iterator begin() { return start; }
		iterator end() { return finish; }
		reference operator[](size_type n) { return start[n]; }
		reference front() { return *start; }
		reference back() { return *(finish - 1); }
		size_type size() const { return finish - start; }
		size_type max_size() const { return size_type(-1); } //这个什么意思？
		bool empty() const { return start == finish; }

	};

	//implement of deque_iterator
	template<class T>
	deque_iterator<T>::self& deque_iterator<T>::operator++() {
		++cur;
		if (cur == last) {
			set_node(node + 1);
			cur = first;
		}
		return *this;
	}

	template<class T>
	deque_iterator<T>::self deque_iterator<T>::operator++(int) {
		auto tmp = *this;
		++*this;
		return tmp;
	}

	template<class T>
	deque_iterator<T>::self& deque_iterator<T>::operator--() {
		if (cur == first) {
			set_node(node - 1);
			cur = last;
		}
		--cur;
		return *this;
	}

	template<class T>
	deque_iterator<T>::self deque_iterator<T>::operator--(int) {
		auto tmp = *this;
		--*this;
		return tmp;
	}

	template<class T>
	deque_iterator<T>::self& deque_iterator<T>::operator+=(difference_type n) {
		difference_type offset = cur - first + n;
		if (offset >=0 && offset < buffer_size()) {
			cur += n;
		}
		else {
			difference_type node_offset = 
				offset > 0 ? offset / difference_type(buffer_size()):-difference_type((-offset - 1) / buffer_size() - 1);
		}

	}

	template<class T>
	deque_iterator<T>::self deque_iterator<T>::operator+(defference_type n) {
		auto tmp = *this;
		return tmp += n;
	}
	
	template<class T>
	deque_iterator<T>::self& deque_iterator<T>::operator-=(difference_type n) {
		return *this += -n;
	}
	
	template<class T>
	deque_iterator<T>::self deque_iterator<T>::operator-(defference_type n) {
		auto tmp = *this;
		return tmp -= n;
	}

}// end of namespace tinySTL

#endif