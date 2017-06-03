// tinySTL.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "tiny_vector.h"
#include "tiny_list.h"
#include <iostream>

void listTest() {
	tinySTL::list<int> l;
	l.push_back(5);
	l.push_back(2);
	l.push_back(1);
	l.push_back(9);
	l.push_back(8);
	for (auto i : l)
		std::cout << i << std::endl;
	l.reverse();
	std::cout << "reverse" << std::endl;
	for (auto i : l)
		std::cout << i << std::endl;
	tinySTL::list<int> l2;
	l2.push_back(40);
	l2.push_back(30);
	l2.push_back(10);
	l2.push_back(20);
	l.merge(l2);
	for (auto i : l2)
		std::cout << i << std::endl;
	std::cout << l2.size() << std::endl;
	for (auto i : l2)
		std::cout << i << std::endl;
}
int main()
{
	listTest();
}

