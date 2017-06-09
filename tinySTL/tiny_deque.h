#ifndef TINY_DEQUE_H_
#define TINY_DEQUE_H_

#include "tiny_iterator.h"
#include "tiny_uninitialized.h"
#include "tiny_allocator.h"
#include "tiny_construct.h"
#include <algorithm>
#include <math.h>

namespace tinySTL {

	enum { __INITIAL_DEQUE_MAP_SIZE = 8 };

	inline size_t deque_buf_size(size_t s) {
		return s > 512 ? 1 : 512 / s;
	}

	template<class T>
	struct deque_iterator : public iterator<random_access_iterator_tag, T>{
		typedef deque_iterator<T> self;
		typedef T** map_pointer;
		typedef size_t size_type;

		size_type buffer_size() const { return deque_buf_size(sizeof(T)); }

		void set_node(map_pointer new_node) {
			node = new_node;
			first = *new_node;
			last = first + difference_type(buffer_size());
		}

		pointer cur, first, last;
		map_pointer node;

		reference operator*() const { return *cur; }
		pointer operator->() const { return &(operator*()); }

		difference_type operator-(const self& x) const {
			return difference_type(buffer_size())*(node - x.node - 1) + (cur - first) + (x.last - x.cur);
		}
		self& operator++();
		self operator++(int);
		self& operator--();
		self operator--(int);
		self& operator+=(difference_type n);
		self operator+(difference_type n) const;
		self& operator-=(difference_type n);
		self operator-(difference_type n) const ;

		reference operator[](difference_type n) const { return *(*this + n); }

		bool operator==(const self& x) const { return cur == x.cur; }
		bool operator!=(const self& x) const { return !(*this == x); }
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
		typedef ptrdiff_t difference_type;

		//构造函数
		deque() :map_size(0), map(0) { create_map_and_nodes(); }
		deque(size_type n, const T& value):map(0),map_size(0){ fill_initialize(n, value); }
		

	protected:
		typedef Allocator<T, alloc> data_allocator;
		typedef Allocator<pointer, alloc> map_allocator;
		typedef pointer* map_pointer;
		map_pointer map;
		size_type map_size;
		iterator start;
		iterator finish;

		void fill_initialize(size_type n, const T& value);
		void create_map_and_nodes(size_type n = 1);
		pointer allocate_node() { return data_allocator::allocate(buffer_size()); }
		void deallocate_node(pointer p) { data_allocator::deallocate(p,buffer_size()); }
		size_type buffer_size() const { return deque_buf_size(sizeof(T)); }

		void reserve_map_at_back(size_type nodes_to_add = 1);
		void reserve_map_at_front(size_type nodes_to_add = 1);
		void reallocate_map(size_type nodes_to_add, bool add_at_front);
		void push_back_aux(const T& value);
		void push_front_aux(const T& value);
		void pop_back_aux();
		void pop_front_aux();

		

	public:
		iterator begin() { return start; }
		iterator end() { return finish; }
		reference operator[](size_type n) { return start[n]; }
		reference front() { return *start; }
		reference back() { return *(finish - 1); }
		size_type size() const { return finish - start; }
		size_type max_size() const { return size_type(-1); } //这个什么意思？
		bool empty() const { return start == finish; }
		void push_back(const T& value);
		void push_front(const T& value);
		void pop_back();
		void pop_front();
		void clear();
		iterator erase(iterator pos);
		iterator erase(iterator first,iterator last);
		

	};

	//implement of deque_iterator
	template<class T>
	typename deque_iterator<T>::self& deque_iterator<T>::operator++() {
		++cur;
		if (cur == last) {
			set_node(node + 1);
			cur = first;
		}
		return *this;
	}

	template<class T>
	typename deque_iterator<T>::self deque_iterator<T>::operator++(int) {
		auto tmp = *this;
		++*this;
		return tmp;
	}

	template<class T>
	typename deque_iterator<T>::self& deque_iterator<T>::operator--() {
		if (cur == first) {
			set_node(node - 1);
			cur = last;
		}
		--cur;
		return *this;
	}

	template<class T>
	typename deque_iterator<T>::self deque_iterator<T>::operator--(int) {
		auto tmp = *this;
		--*this;
		return tmp;
	}

	template<class T>
	typename deque_iterator<T>::self& deque_iterator<T>::operator+=(difference_type n) {
		difference_type offset = cur - first + n;
		if (offset >=0 && offset < difference_type(buffer_size())) {
			cur += n;
		}
		else {
			difference_type node_offset = offset > 0 ? offset / difference_type(buffer_size()):-difference_type((-offset - 1) / buffer_size() - 1);
		}
		return *this;
	}

	template<class t>
	typename deque_iterator<t>::self deque_iterator<t>::operator+(difference_type n) const {
		auto tmp = *this;
		return tmp += n;
	}
	
	template<class t>
	typename deque_iterator<t>::self& deque_iterator<t>::operator-=(difference_type n) {
		return *this += -n;
	}
	
	template<class t>
	typename deque_iterator<t>::self deque_iterator<t>::operator-(difference_type n) const {
		auto tmp = *this;
		return tmp -= n;
	}


	//implement of deque
	template<class T,class alloc>
	void deque<T,alloc>::fill_initialize(size_type n, const T& value) {
		create_map_and_nodes(n);
		//map_pointer cur;
		/*for (cur = start.node; cur < finish.node; ++cur)
			uninitialized_fill(*cur, *cur + buffer_size(), value);
		uninitialized_fill(finish.first,finish.cur,value);*/
		uninitialized_fill(start, finish,value);
	}

	template<class T, class alloc>
	void deque<T,alloc>::create_map_and_nodes(size_type n = 1) {
		size_type num_nodes = n / buffer_size() + 1;
		map_size = std::max(num_nodes + 2, size_t(__INITIAL_DEQUE_MAP_SIZE));
		map = map_allocator::allocate(map_size);
		map_pointer nstart = map + (map_size - num_nodes) / 2; //从中间位置开始存放元素，两头留有空余
		map_pointer nfinish = nstart + num_nodes - 1;
		map_pointer cur;
		try {
			for (cur = nstart; cur <= nfinish; ++cur)
				*cur = allocate_node();
		}
		catch (...) {
			destory(nstart, cur);
			map_allocator::deallocate(map, map_size);
		}

		start.set_node(nstart);
		finish.set_node(nfinish);
		start.cur = start.first;
		finish.cur = finish.first + n % buffer_size();
	}

	template<class T, class alloc>
	void deque<T,alloc>::push_back(const T& value) {
		if (finish.cur != finish.last - 1) {
			construct(finish.cur, value);
			++finish;
		}
		else
			push_back_aux(value);
	}

	template<class T, class alloc>
	void deque<T,alloc>::push_back_aux(const T& value) {
		reserve_map_at_back();
		*(finish.node + 1) = allocate_node();
		construct(finish.cur, value);
		++finish;
	}

	template<class T, class alloc>
	void deque<T,alloc>::push_front(const T& value) {
		if (start.cur != start.first)
			construct(--start.cur, value);
		else
			push_front_aux(value);
	}

	template<class T, class alloc>
	void deque<T,alloc>::push_front_aux(const T& value) {
		reserve_map_at_front();
		*(start.node - 1) = allocate_node();
		--start;
		construct(start.cur, value);
	}
	
	template<class T, class alloc>
	void deque<T,alloc>::reserve_map_at_back(size_type nodes_to_add = 1){
		if (nodes_to_add + 1 > map_size - (finish.node - map))
			reallocate_map(nodes_to_add,false);
	}

	template<class T, class alloc>
	void deque<T,alloc>::reserve_map_at_front(size_type nodes_to_add = 1) {
		if (nodes_to_add > start.node - map)
			reallocate_map(nodes_to_add, true);
	}

	template<class T, class alloc>
	void deque<T,alloc>::reallocate_map(size_type nodes_to_add, bool add_at_front) {
		auto old_nodes_num = finish.node - start.node + 1;
		auto new_nodes_num = old_nodes_num + nodes_to_add;
		map_pointer new_start;
		if (map_size > 2 * new_nodes_num) {
			new_start = map + (map_size - new_nodes_num) / 2 + (add_at_front ?nodes_to_add : 0); //这里注意运算符的优先级，后边的括号必须加上。。。
			if (new_start < start.node)
				std::copy(start.node, finish.node + 1, new_start);
			else
				std::copy_backward(start.node, finish.node + 1, new_start + old_nodes_num);
		}
		else {
			auto new_map_size = map_size + std::max(map_size, nodes_to_add) + 2;
			auto new_map = map_allocator::allocate(new_map_size);
			new_start = new_map + (new_map_size - new_nodes_num) / 2 + (add_at_front ? nodes_to_add : 0);
			std::copy(start.node, finish.node + 1, new_start);
			map_allocator::deallocate(map, map_size);
			map = new_map;
			map_size = new_map_size;
		}
		start.set_node(new_start);
		finish.set_node(new_start + old_nodes_num - 1);
	}

	template<class T, class alloc>
	void deque<T, alloc>::pop_back() {
		if (finish.cur != finish.first) {
			--finish;
			destory(finish.cur);
		}
		else
			pop_back_aux();
	}

	template<class T, class alloc>
	void deque<T, alloc>::pop_back_aux() {
		deallocate_node(finish.first);
		--finish;
		destory(finish.cur);
	}

	template<class T, class alloc>
	void deque<T, alloc>::pop_front() {
		if (start.cur != start.last - 1) {
			destory(start.cur);
			++start;
		}
		else
			pop_front_aux();
	}

	template<class T, class alloc>
	void deque<T, alloc>::pop_front_aux() {
		destory(start.cur);
		deallocate_node(start.first);
		++start;
	}

	template<class T, class alloc>
	void deque<T, alloc>::clear() {
		for (auto node_to_clear = start.node + 1; node_to_clear < finish.node; ++node_to_clear) {
			destory(*node_to_clear,*node_to_clear + buffer_size());
			deallocate_node(*node_to_clear);
		}
		if (start.node == finish.node)
			destory(start.cur,finish.cur);
		else {
			destory(start.cur, start.last);
			destory(finish.first, finish.cur);
			deallocate_node(finish.first);
		}
		finish = start;
	}

	template<class T, class alloc>
	typename deque<T,alloc>::iterator deque<T,alloc>::erase(iterator pos) {
		iterator next = pos;
		++next;
		difference_type dis = pos - start;
		if (dis < (size() >> 1)) {
			std::copy_backward(start,pos,next);
			pop_front();
		}
		else {
			std::copy(next, finish, pos);
			pop_back();
		}
		return start + dis; //这里注意，要返回指向pos元素后边的那个迭代器
	}

	template<class T, class alloc>
	typename deque<T,alloc>::iterator deque<T, alloc>::erase(iterator first, iterator last) {
		if (first == start && last == finish) {
			clear();
			return finish;
		}
		difference_type n = last - first;
		difference_type elems_before = first - start + 1;
		if (elems_before < (size() - n) / 2) {
			std::copy_backward(start, first, last);
			auto new_start = start + n;
			destory(start,new_start);
			for (auto node = start.node; node < new_start.node; ++node)
				deallocate_node(*node);
			start = new_start;
		}
		else {
			std::copy(last, finish, first);
			iterator new_finish = finish - n;
			for (auto node = finish.node; node > new_finish; --node)
				deallocate_node(*node);
			finish = new_finish;
		}
		return start + elems_before;
	}
}// end of namespace tinySTL

#endif