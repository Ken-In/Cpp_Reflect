#include <iostream>
#include <stddef.h>
#include <functional>
using namespace std;

#include "ClassFactory.h"
using namespace congb::reflect;


#include "A.h"
#include "B.h"

int main() {

	ClassFactory* factory = Singleton<ClassFactory>::instance();

	// 反射创建类
	Object* a = factory->create_class("A");
	Object* b =  factory->create_class("B");
	
	// 反射set get 数据成员
	a->set("m_age", 20);
	a->set("m_name", (string)"kitty");
	b->set("m_age", 18);
	b->set("m_name", (string)"kevin");

	string nameA;
	int ageA;
	a->get("m_name", nameA);
	a->get("m_age", ageA);

	string nameB;
	int ageB;
	b->get("m_name", nameB);
	b->get("m_age", ageB);

	cout << "name: " << nameA << "\tage:" << ageA << endl;
	cout << "name: " << nameB << "\tage:" << ageB << endl;

	// 反射调用方法
	a->call("f1");
	b->call("f1");
	int numA = a->call<int>("f2", 100);
	int numB = b->call<int>("f2", 200);
	cout << "numA:" << numA << endl;
	cout << "numB:" << numB << endl;

	return 0;
}
