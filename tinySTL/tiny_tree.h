#ifndef TINY_TREE_H_
#define TINY_TREE_H_

#include "tiny_iterator.h"
#include "tiny_alloc.h"
#include "tiny_pair.h"

namespace tinySTL {
	typedef bool __rb_tree_color_type;
	const __rb_tree_color_type __rb_tree_red = false;
	const __rb_tree_color_type __rb_tree_black = true;

	

	struct __rb_tree_node_base {
		typedef __rb_tree_color_type color_type;
		typedef __rb_tree_node_base* base_ptr;

		color_type color;
		base_ptr left;
		base_ptr right;
		base_ptr parent;

		static base_ptr minimum(base_ptr x) {
			while (x->left) x = x->left;
			return x;
		}

		static base_ptr maximum(base_ptr x) {
			while (x->right) x = x->right;
			return x;
		}
	};

	template<class Value>
	struct __rb_tree_node : public __rb_tree_node_base{
		typedef __rb_tree_node<Value>* link_type;
		Value value;
	};

	struct __rb_tree_base_iterator {
		typedef __rb_tree_node_base::base_ptr base_ptr;
		typedef bidirectional_iterator_tag iterator_category;
		typedef ptrdiff_t difference_type;

		base_ptr node;

		//下面这两个函数都需要对node在边界，也就是在header附近时进行特殊考虑
		void increment() {
			if (node->right) {
				node = node->right;
				while (node->left)
					node = node->left;
			}
			else {
				base_ptr tmp = node->parent;
				while (node == tmp->right) {
					node = tmp;
					tmp = tmp->parent;
				}
				if (node->right != tmp)		//这里是为了解决根节点没有右节点的特殊情况,这时就返回header
					node = tmp;
			}
		}

		void descrement() {
			if (node->color == __rb_tree_red && node->parent->parent == node)	//如果node是header，也就是end(),那么直接将node指向最大值
				node = node->right;
			else if (node->left) {
				node = node->left;
				while (node->right)
					node = node->right;
			}
			else {
				base_ptr tmp = node->parent;
				if (node == tmp->left) {
					node = tmp;
					tmp = tmp->parent;
				}
				node = tmp;
			}
		}
	};

	template<class Value, class Ref, class Ptr>
	struct __rb_tree_iterator : public __rb_tree_base_iterator {
		typedef Value value_type;
		typedef Ref reference;
		typedef Ptr pointer;
		typedef __rb_tree_iterator<Value, Value&, Value*> iterator;
		typedef __rb_tree_iterator<Value, const Value&, const Value*> const iterator;
		typedef __rb_tree_iterator<Value, Ref, Ptr> self;
		typedef __rb_tree_node<Value>* link_type;

		__rb_tree_iterator() = default;
		__rb_tree_iterator(link_type x) { node = x; }
		__rb_tree_iterator(const iterator& it) { node = it.node; }

		reference operator*() const { return link_type(node)->value; }
		pointer operator->() const { return &(opeartor*()); }
		self& operator++() { increment(); return *this; }
		self operator++(int) {
			auto tmp = *this;
			increment();
			return tmp;
		}

		self& operator--() { decrement(); return *this; }
		self operator--(int) {
			auto tmp = *this;
			decrement();
			return tmp;
		}
	};

		template<class Key, class Value, class KeyOfValue, class Compare, class alloc = Alloc>
		class rb_tree{
		protected:
			typedef void* void_pointer;
			typedef __rb_tree_node_base* base_ptr;
			typedef __rb_tree_node<Value> rb_tree_node;
			typedef __rb_tree_color_type color_type;
			typedef allocator<rb_tree_node, Alloc> rb_tree_node_allocator;
		public:
			typedef Key key_type;
			typedef Value value_type;
			typedef value_type* pointer;
			typedef const value_type* const_pointer;
			typedef value_type& reference;
			typedef const value_type& const_reference;
			typedef size_t size_type;
			typedef ptrdiff_t defference_type;
			typedef rb_tree_node* link_type;

		protected:
			link_type get_node() {
				return rb_tree_node_allocator.allocate();
			}
			
			void put_node(link_type* p) { rb_tree_node_allocator.deallocate(p); }

			link_type create_node(const value_type& x) {
				link_type node = get_node();
				construct(node->value, x);
				return node;
			}

			link_type clone_node(link_type x) {
				link_type tmp = create_node(x->value;);
				tmp->color = x->color;
				tmp->left = 0;
				tmp->right = 0;
				return tmp;
			}

			void destory_node(link_type p) {
				destory(&p->value);
				put_node(p);
			}

			size_type node_count;
			link_type header;
			Compare key_compare;

			link_type& root() const { return (link_type&)(header->parent); }
			link_type& leftmost() const { return (link_type&)(header->left); }
			link_type& rightmost() const { retunr(link_type&)(header->right); }
			
			static link_type& left(link_type x) { return (link_type&)(x->left); }
			static link_type& right(link_type x) { return (link_type&)(x->right); }
			static link_type& parent(link_type x) { return (link_type&)(x->parent); }
			static reference value(link_type x) { return x->value; }
			static const key_type& key(link_type x) { return KeyOfValue()(value(x)); }
			static color_type& color(link_type x) { return (color_type&)x->color; }

			static link_type& left(base_ptr x) { return (link_type&)(x->left); }
			static link_type& right(base_ptr x) { return (link_type&)(x->right); }
			static link_type& parent(base_ptr x) { return (link_type&)(x->parent); }
			static reference value(base_ptr x) { return (link_type(x))->value; }
			static const key_type& key(base_ptr x) { return KeyOfValue()(value(x)); }
			static color_type& color(base_ptr x) { return (color_type&)x->color; }

			static link_type minimum(link_type x) {
				return (link_type)__rb_tree_node_base::minimum(x);
			}

			static link_type maximum(link_type x) {
				return (link_type)__rb_tree_node_base::maximum(x);
			}

		public:
			typedef __rb_tree_iterator<value_type, reference, pointer> iterator;

		private:
			void init() {
				header = get_node();
				color(header) = __rb_tree_red;
				root() = 0;
				leftmost() = header;
				rightmost() = header;
			}
			iterator __insert(base_ptr x, base_ptr y, const value_type& v);
			link_type __copy(link_type x, link_type p);
			void __erase(link_type x);
			
		public:
			rb_tree(const Compare& comp = Compare()) :node_count(0), key_compare(comp) { init(); }

			~rb_tree() {
			}

			rb_tree<Key, Value, KeyOfValue, Compare, Alloc>&
				operator=(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x);

		public:
			Compare key_comp() const { return key_compare; }
			iterator begin() { return leftmost(); }
			iterator end() { return header; }
			bool empty() { return node_count == 0; }
			size_type size() const { return node_count; }
			size_type max_size() const { return size_type(-1); }

			pair<iterator, bool> insert_unique(const value_type& v);
			iterator insert_equal(const value_type& v);

			void erase(iterator __position);
			size_type erase(const key_type& __x);
			void erase(iterator __first, iterator __last);
			void erase(const key_type* __first, const key_type* __last);

			iterator find(const Key& k);
		};

		template<class Key, class Value, class KeyOfValue, class Compare, class alloc = Alloc>
		typename rb_tree<Key, Value, KeyOfValue, Compare, alloc>::iterator
			rb_tree<Key, Value, KeyOfValue, Compare, alloc>::insert_equal(const value_type& v) {
			link_type y = header;
			link_type x = root();
			while (x) {
				y = x;
				auto comp = key_compare(KeyOfValue()(v), key(x));
				x = comp ? left(x) : right(x);
			}
			return __insert(x, y, v);
		}

		template<class Key, class Value, class KeyOfValue, class Compare, class alloc = Alloc>
		pair<typename rb_tree<Key, Value, KeyOfValue, Compare, alloc>::iterator,bool>
		rb_tree<Key, Value, KeyOfValue, Compare, alloc>::insert_unique(const value_type& v) {
			link_type y = header;
			link_type x = root();
			while (x) {
				y = x;
				auto comp = key_compare(KeyOfValue()(v), key(x));
				x = comp ? left(x) : right(x);
			}

			iterator j = iterator(y);
			if (comp) {
				if (j == begin())
					return pair<iterator, bool>(__insert(x, y, v), true);
				else
					--j;	//这个--j是迭代器的自减，不要理解成父节点。。
			}

			if (key_compare(key(j.node), KeyOfValue()(v)))
				return pair<iterator, bool>(__insert(x, y, v), true);
			return pair<iterator, bool>(j, false);
		}

		template<class Key, class Value, class KeyOfValue, class Compare, class alloc = Alloc>
		typename rb_tree<Key, Value, KeyOfValue, Compare, alloc>::iterator
			rb_tree<Key, Value, KeyOfValue, Compare, alloc>::__insert(base_ptr x_, base_ptr y_, const value_type& v) {
			link_type x = (link_type)x_;
			link_type y = (link_type)y_;
			link_type z;
			if (y == header || x != 0 || key_compare(KeyOfValue()(v), key(y))) {
				z = create_node(v);
				left(y) = z;
				if (y == header) {
					root() = z;
					rightmost() = z;
				}
				else if (y == leftmost())
					leftmost() = z;
			}
			else {
				z = create_node(v);
				right(y) = z;
				if (y == rightmost()) {
					rightmost() = z;
				}
			}
			parent(z) = y;
			left(z) = 0;
			right(z) = 0;

			__rb_tree_rebalance(z, root());
			++node_count;
			return iterator(z);
		}

		template<class Key, class Value, class KeyOfValue, class Compare, class alloc = Alloc>
		typename rb_tree<Key, Value, KeyOfValue, Compare, alloc>::iterator
			rb_tree<Key, Value, KeyOfValue, Compare, alloc>::find(const Key& k) {
			link_type y = header;
			link_type x = root();
			while (x != 0) {
				if (!key_compare(key(x), k)) {
					y = x;
					x = left(x);
				}
				else
					x = right(x);	//注意这里没有y = x
			}

			itrator j = iterator(y);
			return (j == end() || key_compare(k, key(j.node))) ? end() : j;

		}




		//RB-tree调整函数
		void __rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
			x->color = __rb_tree_red;
			while (x != root && x->parent->color == __rb_tree_red) {
				if (x->parent == x->parent->parent->left) {
					__rb_tree_node_base* y = x->parent->parent->right;
					if (y && y->color == __rb_tree_red) {
						x->parent->color = __rb_tree_black;
						y->color = __rb_tree_black;
						x->parent->parent->color = __rb_tree_red;
						x = x->parent->parent;
					}
					else {
						if (x == x->parent->right) {
							x = x->parent;
							__rb_tree_rotate_left(x, root);
						}
						x->parent->color = __rb_tree_black;
						x->parent->parent->color = __rb_tree_red;
						__rb_tree_rotate_right(x->parent->parent, root);
					}
				}
				else {
					__rb_tree_node_base* y = x->parent->parent->left;
					if (y && y->color == __rb_tree_red) {
						x->parent->color = __rb_tree_black;
						y->color = __rb_tree_black;
						x->parent->parent->color = __rb_tree_red;
						x = x->parent->parent;
					}
					else {
						if (x == x->parent->left) {
							x = x->parent;
							__rb_tree_rotate_right(x, root);
						}
						x->parent->color = __rb_tree_black;
						x->parent->parent->color = __rb_tree_red;
						__rb_tree_rotate_left(x->parent->parent, root);
					}
				}
			}
			root->color = __rb_tree_black;
		}

		void __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
			auto y = x->right;
			x->right = y->left;
			if (y->left != 0)
				y->left->parent = x;
			y->parent = x->parent;
			if (x == root)
				root = y;
			else if (x == x->parent->left)
				x->parent->left = y;
			else
				x->parent->right = y;
			y->left = x;
			x->parent = y;
		}

		void __rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
			auto y = x->left;
			x->left = y->right;
			if (y->right != 0)
				y->right->parent = x;
			y->parent = x->parent;
			if (x == root)
				root = y;
			else if (x == x->parent->left)
				x->parent->left = y;
			else
				x->parent->right = y;
			y->right = x;
			x->parent = y;
		}

} //end of namespace tinySTL




#endif