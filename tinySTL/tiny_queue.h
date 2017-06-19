#ifndef TINY_QUEUE_H_
#define TINY_QUEUE_H_

namespace tinySTL {

	template<class T, class Sequence = deque<T>>
	class queue {
	public:
		typedef typename Sequence::value_type value_type;
		typedef typename Sequence::reference reference;
		typedef typename Sequence::const_reference const_reference;
		typedef typename Sequence::size_type size_type;
	protected:
		Sequence c;
	public:
		size_type size() { return c.size(); }
		reference front() { return c.front(); }
		const_reference front() const{ return c.front(); }
		reference back() { return c.back(); }
		const_reference back() const { return c.back(); }
		void push(const value_type& value) { c.push_back(value); }
		void pop() { c.pop_front(); }
	};
}


#endif
