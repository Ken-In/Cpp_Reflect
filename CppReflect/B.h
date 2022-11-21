#pragma once

#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include "ClassRegister.h"
using namespace congb::reflect;

class B : public Object {
public:
	B() {}
	~B() {}

	void show() {
		cout << "B" << endl;
	}

	void f1() {
		cout << "B f1" << endl;
	}

	int f2(int num) {
		cout << "B f2" << "," << num << endl;
		return num;
	}
public:
	string m_name;
	int m_age;
};

//ע���
REGISTER_CLASS(B);//ע����

REGISTER_CLASS_FIELD(B, m_name, string);//ע��field
REGISTER_CLASS_FIELD(B, m_age, int);

REGISTER_CLASS_METHOD(B, f1, void);//ע��method
REGISTER_CLASS_METHOD(B, f2, int, int);

