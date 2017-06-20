// tinySTL.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "tiny_vector.h"
#include "tiny_list.h"
#include "tiny_deque.h"
#include <iostream>

using std::cout;
using std::endl;
int test = 0;
int test_pass = 0;
int main_ret = 0;

#define expect_equal(actual, expect) \
    do {\
        test++;\
        if (expect == actual)\
            test_pass++;\
        else {\
            cout << __FILE__ << " " << __LINE__ << ": expect: " << expect << " actual: " << actual << endl;\
            main_ret = 1;\
        }\
    } while(0)

void listTest() {
	tinySTL::list<int> l;
	l.push_back(5);
	l.push_back(2);
	l.push_back(1);
	l.push_back(9);
	l.push_back(8);
	expect_equal(*l.begin(), 5);
	expect_equal(*(--l.end()), 8);
	expect_equal(l.size(), 5);
	l.reverse();
	expect_equal(*l.begin(), 8);
	expect_equal(*(--l.end()), 5);
	tinySTL::list<int> l2;
	l2.push_back(40);
	l2.push_back(30);
	l2.push_back(10);
	l2.push_back(20); 
	l.merge(l2);
	expect_equal(*(--l.end()), 20);
	expect_equal(l.size(), 9);

}

void dequeTest() {
	tinySTL::deque<double> db;
	expect_equal(db.empty(), 0);
	tinySTL::deque<int> d(20,9);	//20个9
	expect_equal(d[0],9);
	expect_equal(d.size(), 20);

	d.push_back(10); 
	d.push_back(11);
	d.push_back(12);
	d.push_back(13);
	d.push_back(14); 
	//20个9，10，11，12，13，14
	expect_equal(*(d.end() - 1), 14);
	expect_equal(d.size(), 25);
	d.push_front(8);
	d.push_front(7);
	//7，8，20个9，10，11，12，13，14
	expect_equal(*d.begin(),7);
	expect_equal(d.size(), 27);
	d.pop_back();
	d.pop_back();
	d.pop_back();
	d.pop_back();
	d.pop_back();
	//7，8，20个9
	expect_equal(*(d.end() - 1), 9);
	expect_equal(d.size(), 22);
	d.pop_front();
	d.pop_front();
	//20个9
	expect_equal(*d.begin(), 9);
	expect_equal(d.size(), 20);
	d.clear();
	//空
	expect_equal(d.size(),0);
	expect_equal((d.begin() == d.end()),true);

	d.push_back(1);
	d.push_back(2);
	d.push_back(3);
	//1，2，3
	d.erase(d.begin());
	//2，3
	expect_equal(*d.begin(), 2);
	expect_equal(d.size(), 2);
	d.insert(d.begin() + 1, 10);
	expect_equal(*(d.begin() + 1), 10);
	expect_equal(d.size(), 3);
}

int main()
{
	listTest();
	//dequeTest();

	cout << test_pass << "/" << test << " : " << test / test_pass * 100.0 << "%";
	return main_ret;
}

