#ifndef TINY_PAIR_H_
#define TINY_PAIR_H_
namespace tinySTL {
	template<class T1, class T2>
	struct pair {
		typedef T1 first_type;
		typedef T2 second_type;
		T1 first;
		T2 second;
		pair() :first(T1()), second(T2()) {}
		pair(const T1& a, const T2& b) :frist(a), second(b) {}

		template<class U1, class U2>
		pair(const pair<U1, U2>& p) : first(p.first), second(p.second) {}
	};

	template <class _T1, class _T2>
	inline bool operator==(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
	{
		return __x.first == __y.first && __x.second == __y.second;
	}

	template <class _T1, class _T2>
	inline bool operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
	{
		return __x.first < __y.first ||
			(!(__y.first < __x.first) && __x.second < __y.second);
	}

	template <class _T1, class _T2>
	inline bool operator!=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y) {
		return !(__x == __y);
	}

	template <class _T1, class _T2>
	inline bool operator>(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y) {
		return __y < __x;
	}

	template <class _T1, class _T2>
	inline bool operator<=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y) {
		return !(__y < __x);
	}

	template <class _T1, class _T2>
	inline bool operator>=(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y) {
		return !(__x < __y);
	}

	template <class _T1, class _T2>
	inline pair<_T1, _T2> make_pair(const _T1& __x, const _T2& __y)
	{
		return pair<_T1, _T2>(__x, __y);
	}
}

#endif
