#ifndef TINY_LIST_H_
#define TINY_LIST_H_

#include "tiny_iterator.h"
#include "tiny_allocator.h"
#include "tiny_uninitialized.h"


namespace tinySTL {
	template<class T>
	struct list_node{
		T data;
		list_node<T> * prev;
		list_node<T> * next;
	};

	template<class T>
	class list_iterator :public iterator<bidirectional_iterator_tag, T>{
	public:
		typedef list_iterator<T> self;
		typedef list_node <T>* nodePtr;

		nodePtr node;

		//Constructors
		list_iterator(nodePtr x) :node(x) {}
		list_iterator() = default;
		list_iterator(const self& x):node(x.node){}

		bool operator==(const self& x) { return node == x.node; }
		bool operator!=(const self& x) { return !(operator==(x)); }

		reference operator*() { return node->data; }
		pointer operator->() { return &(operator*()); }

		self& operator++() {
			node = node->next;
			return *this;
		}

		self operator++(int) {
			auto tmp = *this;
			++*this;
			return tmp;
		}
		
		self& operator--() {
			node = node->prev;
			return *this;
		}

		self operator--(int) {
			auto tmp = *this;
			--*this;
			return tmp;
		}
	};

	template<class T, class alloc = Alloc>
	class list {
	public:
		typedef list_node<T> list_node;
		typedef list_iterator<T> iterator;
		typedef T value_type;
		typedef size_t size_type;
		typedef list_node* nodePtr;
		typedef T& reference;
		typedef T* pointer;

		//构造函数
		list() {
			node = get_node();
			node->next = node;
			node->prev = node;
		}

	protected:
		nodePtr node;

		typedef Allocator<list_node, alloc> list_node_allocator;

		//节点操作相关函数
		nodePtr get_node() {
			nodePtr p = list_node_allocator::allocate();
			return p;
		}

		void put_node(nodePtr p) {
			list_node_allocaotr::deallocate(p);
		}

		nodePtr create_node(const T& value) {
			nodePtr p = get_node();
			construct(&p->data,value);
			return p;
		}

		void destory_node(nodePtr p) {
			destory(&p->data);
			put_node(p);
		}

		void transfer(iterator position, iterator first, iterator last);
		
	public:
		//迭代器 容量相关函数
		iterator begin() { return node->next; }
		iterator end() { return node; }
		bool empty() { return node->next == node };
		size_type size();
		
		//元素操作相关函数
		reference front() {return *begin()};
		reference back() {return *(--end())};
		iterator insert(iterator position, const T& value);
		void push_back(const T& value) { insert(end(), value); }
		void push_front(const T& value) { insert(begin(), value); }
		iterator erase(iterator position);
		void pop_back() { erase(--end()); }
		void pop_front() { erase(begin()); }
		void clear();
		void remove(const T& value);
		void unique();

		void splice(iterator position, list& x);
		void splice(iterator position, list&, iterator i);
		void splice(iterator position, list&, iterator first, iterator last);
		void merge(list<T,alloc>& x);
		void reverse();
		void sort();
		


	};
	
	template<class T, class alloc>
	typename list<T, alloc>::size_type list<T, alloc>::size() {
		return distance(begin(),end());
	}

	template<class T, class alloc>
	typename list<T, alloc>::iterator list<T, alloc>::insert(iterator position, const T& value) {
		nodePtr tmp = create_node(value);
		tmp->next = position.node;
		tmp->prev = position.node->prev;
		position.node->prev->next = tmp;
		position.node->prev = tmp;
		return tmp;
	}

	template<class T, class alloc>
	typename list<T, alloc>::iterator list<T, alloc>::erase(iterator position) {
		auto next_node = position.node->next;
		auto prev_node = position.node->prev;
		pre_node->next = next_node;
		next_node->prev = prev_node;
		destory(position.node);
		return iterator(next_node);
	}

	template<class T, class alloc>
	void list<T, alloc>::clear() {
		nodePtr cur = begin();
		nodePtr last = end();
		while (cur != last) {
			auto tmp = cur;
			cur = cur->next;
			destory_node(tmp);
		}
		node->next = node;
		node->prev = node;
	}

	template<class T, class alloc>
	void list<T, alloc>::remove(const T& value) {
		iterator cur = begin();
		iterator last = end();
		while (cur != last) {
			iterator next = cur;
			++next;
			if (*cur == value)
				erase(cur);
			cur = tmp;
		}
	}

	template<class T, class alloc>
	void list<T, alloc>::unique() {
		iterator cur = begin();
		iterator last = end();
		if (cur == last) reurn;
		auto next = cur;
		while (++next != last) {
			if (*cur == *next) {
				erase(next);
				next = cur;
			}
			else
				cur = next;
		}
	}

	template<class T, class alloc>
	void list<T, alloc>::transfer(iterator position, iterator first, iterator last) {
		last.node->prev->next = position.node;
		first.node->prev->next = last.node;
		position.node->prev->next = first.node;
		auto tmp = position.node->prev;
		position.node->prev = last.node->prev;
		last.node->prev = first.node->prev;
		first.node->prev = tmp;
	}

	template<class T, class alloc>
	void list<T, alloc>::splice(iterator position, list& x) {
		if (!x.empty())
			transfer(position, x.begin(), x.end());
	}

	template<class T, class alloc>
	void list<T, alloc>::splice(iterator position, list& x, iterator i) {
		iterator j = i;
		++j;
		if (position == i || position == j) return;
		transfer(position, i, j);
	}
	
	template<class T, class alloc>
	void list<T,alloc>::splice(iterator position, list&, iterator first, iterator last) {
		if (first != last)
			transfer(position,first,last);
	}

	template<class T, class alloc>
	void list<T, alloc>::merge(list<T, alloc>& x) {
		auto first = begin();
		auto last = end();
		auto first2 = x.begin();
		auto last2 = x.end();
		while (first != last && first2 != last2) {
			if (*first > *first2) {
				auto next = first2;
				transfer(first, first2, ++next);
				first2 = next;
			}
			else
				++first;
		}
		if (first2 != last2)
			transfer(last, first2, last2);
	}

	template<class T, class alloc>
	void list<T, alloc>::reverse() {
		if (node->next == node || node->next->next == node) return;
		auto first = begin();
		auto last = end();
		++first;
		while (first != last) {
			auto tmp = first;
			++first;
			transfer(begin(), tmp, first);
		}
	}

	template<class T, class alloc>
	void list<T, alloc>::sort() {
		
	}



}//End of namespace tinySTL
#endif
