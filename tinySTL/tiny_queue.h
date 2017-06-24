#ifndef TINY_QUEUE_H_
#define TINY_QUEUE_H_

#include "tiny_functional.h"
#include "tiny_vector.h"
#include "tiny_heap.h"
#include "tiny_deque.h"

namespace tinySTL {

	template<class T, class Container = deque<T>>
	class queue {
	public:
		typedef typename Container::value_type value_type;
		typedef typename Container::reference reference;
		typedef typename Container::const_reference const_reference;
		typedef typename Container::size_type size_type;
	protected:
		Container c;
	public:
		size_type size() { return c.size(); }
		reference front() { return c.front(); }
		const_reference front() const{ return c.front(); }
		reference back() { return c.back(); }
		const_reference back() const { return c.back(); }
		void push(const value_type& value) { c.push_back(value); }
		void pop() { c.pop_front(); }
	};

	template<class T, class Container = vector<T>, class Compare = less<typename Container::value_type>>
	class priority_queue {
	public:
		typedef typename Container::value_type value_type;
		typedef typename Container::size_type size_type;
		typedef typename Container::reference reference;
		typedef typename Container::const_reference const_reference;
	protected:
		Container c;
		Compare comp;
	public:
		priority_queue() :c() {}
		priority_queue(const Compare& _comp) :c(), comp(x) {}

		template<class InputIterator>
		priority_queue(InputIterator first, InputIterator last) : c(first, last) {
			tinySTL::make_heap(c.begin(),c.end(), comp);
		}

		template<class InputIterator>
		priority_queue(InputIterator first, InputIterator last, const Compare& _comp) : c(first, last),comp(_comp) {
			tinySTL::make_heap(c.begin(), c.end(), comp);
		}

		bool empty() const { return c.empty(); }
		size_type size() const { return c.size(); }
		const_reference top() const { return c.front(); }
		void push(const value_type& value) {
			c.push_back(value);
			tinySTL::push_heap(v.begin(),c.end(),comp);
		}

		void pop() {
			tinySTL::pop_heap(c.begin(),c.end(),comp);
			c.pop_back();
		}
	};

}


#endif
