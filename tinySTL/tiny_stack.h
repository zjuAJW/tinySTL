#ifndef TINY_STACK_H_
#define TINY_STACK_H_

#include "tiny_alloc.h"
#include "tiny_deque.h"
namespace tinySTL {
	template<class T, class Sequence = deque<T>>
	class stack {
	public:
		/*friend bool operator==(const stack&, const stack&);*/
		/*friend bool operator<(const stack&, const stack&);*/

		typedef Sequence::value_type value_type;
		typedef Sequence::size_type size_type;
		typedef Sequence::reference reference;
		typedef Sequence::const_reference const_reference;
	protected:
		Sequence c;
	public:
		bool empty() const { return c.empty(); }
		size_type size() const { return c.size(); }
		reference top() { return c.back(); }
		const_reference top() const { return c.back(); }
		void push(const value_type& value) { c.push_back(value; }
		void pop() { c.pop_back(value); }
	};

	//template<class T, class Sequence>
	//bool operator==(const stack<T,Sequence>& a, const stack<T,Sequence>& b) {
	//	return a.c == b.c;
	//}

	//template<class T, class Sequence>
	//bool operator<(const stack<T, Sequence>& a, const stack<T, Sequence>& b) {
	//	return a.c < y.c;
	//}
}
#endif 


