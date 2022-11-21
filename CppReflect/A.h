#pragma once

#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include "ClassRegister.h"
using namespace congb::reflect;

class A : public Object{
public:
	A() {}
	~A() {}

	void show() {
		cout << "A" << endl;
	}

	void f1() {
		cout << "A f1" << endl;
	}

	int f2(int num) {
		cout << "A f2" << "," << num << endl;
		return num;
	}
public:
	string m_name;
	int m_age;
};

//×¢²áºê
REGISTER_CLASS(A);//×¢²áÀà

REGISTER_CLASS_FIELD(A, m_name, string);//×¢²áfield
REGISTER_CLASS_FIELD(A, m_age, int); 

REGISTER_CLASS_METHOD(A, f1, void);//×¢²ámethod
REGISTER_CLASS_METHOD(A, f2, int, int);

