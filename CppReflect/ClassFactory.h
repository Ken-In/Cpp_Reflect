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

	int get_field_count();//�õ���Ա��������
	ClassField* get_field(int pos);//��pos�õ���Ա���� posָmap��vector������
	ClassField* get_field(const string& fieldName);//�ó�Ա���õ���Ա����

	template<typename T>
	void get(const string& fieldName, T& value);//�ó�Ա���õ���Ա���� ��ֵ��value

	template<typename T>
	void set(const string& fieldName, const T& value);//�����ֺ�value���ó�Ա���� 

	int get_method_count();
	
	template <typename R = void, typename ...Args>
	R call(const string& methodName, Args... args);

	virtual void show() = 0;

private:
	string m_className;
};

typedef Object* (*create_object)(void);//����create_object���� ��ʾһ������ָ��
// ָ��ĺ��� ����Ϊvoid ����ֵΪvoid*

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
	ClassField* field = Singleton<ClassFactory>::instance()->get_field(m_className, fieldName);//�������ͳ�Ա��ȡ��field
	size_t offset = field->offset();
	value = *(T*)((unsigned char*)this + offset);//offsetƫ���ҵ���Ա���ݵ�ַ
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