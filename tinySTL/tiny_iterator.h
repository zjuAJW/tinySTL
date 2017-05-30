#ifndef TINY_ITERATOR_H_
#define TINY_ITERATOR_H_

#include <cstddef>
namespace tinySTL {

	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag :public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};

	template<class Category, class T, class Distance = ptrdiff_t, 
			 class Pointer = T*, class Reference = T&>
	struct iterator{
		typedef Category	iterator_category;
		typedef T			value_type;
		typedef Distance	difference_type;
		typedef Pointer		pointer;
		typedef Reference	reference;
	};

	template <class Iterator>
	struct iterator_traits {
		typedef typename Iterator::iterator_category	iterator_category;
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::difference_type		difference_type;
		typedef typename Iterator::pointer				pinter;
		typedef typename Iterator::reference			reference;
	};

	template <class T>
	struct iterator_traits<T*> {
		typedef typename random_access_iterator_tag		iterator_category;
		typedef typename T								value_type;
		typedef typename ptrdiff_t						difference_type;
		typedef typename T*								pinter;
		typedef typename T&								reference;
	};

	template <class T>
	struct iterator_traits<const T*> {
		typedef typename random_access_iterator_tag		iterator_category;
		typedef typename T								value_type;
		typedef typename ptrdiff_t						difference_type;
		typedef typename T*								pinter;
		typedef typename T&								reference;
	};

	template<class Iterator>
	inline typename iterator_traits<Iterator>::iterator_caterogy
		iterator_category(const Iterator&) {
		//TODO: 待定
		return typename iterator_traits<Iterator>::iterator_category();
	}

	//注意返回值是指针
	template<class Iterator>
	inline typename iterator_traits<Iterator>::value_type*
	value_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}

	//注意返回值是指针
	template<class Iterator>
	inline typename iterator_traits<Iterator>::distance_type*
	distance_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::distance_type*>(0);
	}





}//end of namespace tinySTL


#endif
