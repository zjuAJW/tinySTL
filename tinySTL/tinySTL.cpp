// tinySTL.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "tiny_vector.h"
#include <iostream>

int main()
{
	tinySTL::vector<int> v;
	v.push_back(1);
	std::cout << v.capacity() << std::endl;
	v.push_back(5);
	std::cout << v.capacity() << std::endl;
	v.push_back(4);
	std::cout << v.capacity() << std::endl;
	v.push_back(3);
	std::cout << v.capacity() << std::endl;
	v.push_back(2);
	std::cout << v.capacity() << std::endl;
	for (auto a : v) {
		std::cout << a;
	}
	v.erase(v.begin()+2);
	std::cout << std::endl;
	for (auto a : v) {
		std::cout << a;
	}
    return 0;
}

