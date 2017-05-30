#ifndef TINY_LIST_H_
#define TINY_LIST_H_

#include "tiny_iterator.h"

namespace tinySTL {
	template<class T>
	struct list_node{
		T data;
		list_node<T> * prev;
		list_node<T> * next;
		list_node<T> * data;
	};

	template<class T>
	class list_iterator :public iterator<bidirectional_iterator_tag, T>{
	public:
		typedef list_iterator<T> self;
		typedef list_node <T>* nodePtr;

		nodePtr p;

		//Constructors
		list_iterator(nodePtr x) :p(x) {}
		list_iterator() = default;
		list_iterator(const self& x):p(x.p){}

		bool operator==(const self& x) { return p == x.p; }
		bool operator!=(const self& x) { return !(operator==(x)); }

		reference operator*() { return p->data; }
		pointer operator->() { return &(operator*()); } //TODO:这个操作符重载，，，查下资料

		self& oprator++() {
			p = p->next;
			return *this;
		}

		self operator(int) {
			auto tmp = *this;
			++*this;
			return tmp;
		}
		
		self & operator--() {
			p = p->prev;
			return *this;
		}

		self operator--(int) {
			auto tmp = *this;
			--*this;
			return tmp;
		}
	};

	template<class T>
	class list {
	public:
		typedef list_node<T> list_node;
		typedef list_iterator<T> iterator;
		typedef T value_type;
		typedef size_t size_type;
		typedef list_node<T> * nodePtr;
		typedef T& reference;
		typedef T* pointer;

	protected:
		iterator node;
		iterator begin() { return node->next; }
		iterator end() { return node; }
		bool empty() { return node->next == node };
		size_type size();

		reference front();
		reference back();

	};
	



}//End of namespace tinySTL
#endif
