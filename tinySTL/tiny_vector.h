#ifndef VECTOR_H_
#define VECTOR_H_

#include "tiny_allocator.h"
#include "tiny_uninitialized.h"

#include <algorithm>

namespace tinySTL {
	template<class T, class alloc = Alloc> 
	class vector{
	public:
		typedef T			value_type;
		typedef value_type*	iterator;
		typedef value_type*	pointer;
		typedef value_type& reference;
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;

	protected:

		typedef Allocator<value_type, alloc> data_allocator;

		iterator start;
		iterator finish;
		iterator end_of_storage;

		iterator allocate_and_fill(size_type n, const T& value) {
			iterator result = data_allocator::allocate(n);
			tinySTL::uninitialized_fill_n(result, n, value);
			return result;
		}
		
		void fill_initialize(size_type n, const T& value) {
			start = allocate_and_fill(n, value);
			finish = start + n;
			end_of_storage = finish;
		}

		void insert_aux(iterator position, const T& x);

	public:
		iterator begin() const { return start; }
		iterator end() const { return finish; }
		size_type size() const { return (size_type)(end() - begin()); }
		size_type capacity() const { return static_cast<size_type>(end_of_storage - begin()); }
		bool empty() const { return begin() == end(); }

		reference operator[](size_type n) { return *(begin() + n); }
		reference front() { return *begin() };
		reference back() { return *(end() - 1); }

		void push_back(const T& x);
		void pop_back();
		iterator erase(iterator first, iterator last);
		iterator erase(iterator postition);
		void insert(iterator position, size_type n, const T& value);
		void clear() { erase(begin(), end()); }
	};

	template<class T,class alloc>
	void vector<T,alloc>::insert_aux(iterator position, const T& x) {
		if (finish != end_of_storage) {
			construct(finish, *(end()-1));
			++finish;
			std::copy_backward(position, end() - 2, end() - 1);//这里暂时用了现有的，可以自己写一下！
			*position = x;
		}
		else {
			size_type old_size = size();
			size_type len = old_size == 0 ? 1 : 2 * old_size;
			iterator new_start = data_allocator::allocate(len);
			iterator new_finish = new_start;
			try {
				new_finish = uninitalized_copy(start, position, new_start);
				construct(new_finish, x);
				++new_finish;
				new_finish = uninitalized_copy(position, finish, new_finish);
			}
			catch (...) {
				destory(new_start, new_finish);
				data_allocator::deallocate(new_finish, len);
				throw;
			}

			destory(begin(), end());
			data_allocator::deallocate(begin(), capacity());
			start = new_start;
			finish = new_finish;
			end_of_storage = start + len;
		}
	}

	template<class T, class alloc>
	void vector<T, alloc>::push_back(const T& x) {
		if (end() != end_of_storage) {
			construct(finish, x);
			++finish;
		}
		else {
			insert_aux(end(), x);
		}
	}

	template<class T, class alloc>
	void vector<T, alloc>::pop_back() {
		--finish;
		destory(finish);
	}

	template<class T, class alloc>
	typename vector<T, alloc>::iterator vector<T, alloc>::erase(iterator first, iterator last) {
		iterator new_finish = std::copy(last, finish, first);
		destory(new_finish,finish);
		finish = new_finish;
		return first;
	}
	template<class T, class alloc>
	typename vector<T, alloc>::iterator vector<T, alloc>::erase(iterator position) {
		if (position + 1 != end())
			std::copy(position + 1, finish, position);
		--finish;
		destory(finish);
		return position;
	}

	template<class T, class alloc>
	void vector<T, alloc>::insert(iterator position, size_type n, const T& value) {
		if (n > 0) {
			if (size_type(end_of_storage - end()) >= n) {
				size_type element_after = finish - position;
				iteratro old_finish = finish;
				if (element_after > n) {
					uninitialized_copy(finish - n, finish, finish);
					finish += n;
					std::copy_backward(position,old_finish - n,old_finish);
					std::fill(position, position + n, value);
				}
				else {
					uninitialized_fill_n(finish, n - element_after, value);
					finish += (n - element_after);
					uninitialized_copy(position, old_finish, finish);
					finish += element_after;
					std::fill(position, old_finish, value);
				}
			}
			else {

			}
		}
	}
}//End of namespace tinySTL

#endif

