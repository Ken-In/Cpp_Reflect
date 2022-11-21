#pragma once
 
#include <string>
#include <map>
#include <vector>
#include <functional>
using namespace std;

#include "Singleton.h"
using namespace congb::utility;

#include "ClassField.h"
#include "ClassMethod.h"
using namespace congb::reflect;

namespace congb {
namespace reflect {

class Object {
public:
	Object() {};
	virtual ~Object() {};

	void set_class_name(const string& className);
	const string& get_class_name() const;

	int get_field_count();//得到成员数据数量
	ClassField* get_field(int pos);//用pos得到成员数据 pos指map中vector的索引
	ClassField* get_field(const string& fieldName);//用成员名得到成员数据

	template<typename T>
	void get(const string& fieldName, T& value);//用成员名得到成员数据 赋值给value

	template<typename T>
	void set(const string& fieldName, const T& value);//用名字和value设置成员数据 

	int get_method_count();
	
	template <typename R = void, typename ...Args>
	R call(const string& methodName, Args... args);

	virtual void show() = 0;

private:
	string m_className;
};

typedef Object* (*create_object)(void);//定义create_object类型 表示一个函数指针
// 指向的函数 参数为void 返回值为void*

class ClassFactory {
	friend class Singleton<ClassFactory>;
public:
	void register_class(const string& className, create_object method);
	Object* create_class(const string& className);

	void register_class_field(const string& className, const string& fieldName, const string& fieldType, size_t offset);
	int get_field_count(const string& className);
	ClassField* get_field(const string& className, int pos);
	ClassField* get_field(const string& className, const string& fieldName);

	void register_class_method(const string& className, const string& methodName, uintptr_t method);
	int get_method_count(const string& className);
	ClassMethod* get_class_method(const string& className, int pos);
	ClassMethod* get_class_method(const string& className, const string& methodName);
private:
	ClassFactory(){}
	~ClassFactory(){}

private:
	map<string, create_object> m_classMap;
	map<string, vector<ClassField *>> m_classFields;
	map<string, vector<ClassMethod *>> m_classMethods;
};

template<typename T>
void Object::get(const string& fieldName, T& value) {
	ClassField* field = Singleton<ClassFactory>::instance()->get_field(m_className, fieldName);//用类名和成员名取得field
	size_t offset = field->offset();
	value = *(T*)((unsigned char*)this + offset);//offset偏移找到成员数据地址
}

template<typename T>
void Object::set(const string& fieldName, const T& value) {
	ClassField* field = Singleton<ClassFactory>::instance()->get_field(m_className, fieldName);
	size_t offset = field->offset();
	*(T*)((unsigned char*)this + offset) = value;
}

template <typename R, typename ...Args>
R Object::call(const string& methodName, Args... args)
{
	ClassMethod* method = Singleton<ClassFactory>::instance()->get_class_method(m_className, methodName);
	auto func = method->method();
	typedef std::function<R(decltype(this), Args...)> class_method;
	return (*((class_method*)func))(this, args...);
}

}
}