// tinySTL.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "tiny_vector.h"
#include "tiny_list.h"
#include "tiny_deque.h"
#include <iostream>
#include "tiny_heap.h"
#include "tiny_queue.h"

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
void vectorTest() {
	int ia[10] = { 0,1,2,3,4,5,6,7,8,9 };
	tinySTL::vector<int> ivec(ia,ia + 10);
	expect_equal(ivec[0], 0);
	expect_equal(ivec[9], 9);
}
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

void heapTest() {
	{
		int ia[9] = { 0,1,2,3,4,8,9,3,5 };
		tinySTL::vector<int> ivec(ia, ia + 9);
		tinySTL::make_heap(ivec.begin(), ivec.end());
		expect_equal(ivec[0], 9);
		expect_equal(ivec[8], 1);

		ivec.push_back(7);
		tinySTL::push_heap(ivec.begin(), ivec.end());
		expect_equal(ivec[1], 7);
		expect_equal(ivec[9], 4);
		tinySTL::pop_heap(ivec.begin(), ivec.end());
		expect_equal(*(ivec.end() - 1), 9);
		ivec.pop_back();
		expect_equal(ivec[8], 1);
		expect_equal(ivec[0], 8);
		expect_equal(ivec[6], 2);
		tinySTL::sort_heap(ivec.begin(), ivec.end());
		expect_equal(ivec[0], 0);
		expect_equal(ivec[2], 2);
		expect_equal(ivec[8], 8);
	}
	{
		int ia[9] = { 0,1,2,3,4,8,9,3,5 };
		tinySTL::make_heap(ia, ia + 9);
		tinySTL::sort_heap(ia, ia + 9);
		expect_equal(ia[0], 0);
		expect_equal(ia[2], 2);
		expect_equal(ia[8], 9);
	}
}

void queueTest() {
	{
		int ia[9] = { 0,1,2,3,4,8,9,3,5 };
		tinySTL::priority_queue<int> ipq(ia, ia + 9);
		expect_equal(ipq.top(),9);
		ipq.pop();
		expect_equal(ipq.top(),8);
	}
}
int main()
{	
	//vectorTest();
	//listTest();
	//dequeTest();
	//heapTest();
	queueTest();

	cout << test_pass << "/" << test << " : " << test / test_pass * 100.0 << "%";
	return main_ret;
}

