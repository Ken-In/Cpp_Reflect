#include "ClassFactory.h"
using namespace congb::reflect;

#include<functional>
using namespace std;

//------------------------------------------------------------------------------------------
// Object部分

// class
void Object::set_class_name(const string& className) {
	m_className = className;
}

const string& Object::get_class_name() const {
	return m_className;
}

// field
int Object::get_field_count() {
	ClassFactory* factory = Singleton<ClassFactory>::instance();
	return factory->get_field_count(m_className);
}

ClassField* Object::get_field(int pos) {
	ClassFactory* factory = Singleton<ClassFactory>::instance();
	return factory->get_field(m_className, pos);
}

ClassField* Object::get_field(const string& fieldName) {
	ClassFactory* factory = Singleton<ClassFactory>::instance();
	return factory->get_field(m_className, fieldName);
}

// method
int Object::get_method_count() {
	ClassFactory* factory = Singleton<ClassFactory>::instance();
	return factory->get_method_count(m_className);
}


//------------------------------------------------------------------------------------------
// ClassFactory 部分

//反射类
void ClassFactory::register_class(const string& className, create_object method) {
	m_classMap[className] = method;
}

Object* ClassFactory::create_class(const string& className) {
	auto it = m_classMap.find(className);
	if (it == m_classMap.end()) {
		return nullptr;
	}
	return it->second();//这里是second() 也就是执行second函数返回值
}

//反射类数据成员
void ClassFactory::register_class_field(const string& className, const string& fieldName, const string& fieldType, size_t offset){
	m_classFields[className].push_back(new ClassField(fieldName, fieldType, offset));
}

int ClassFactory::get_field_count(const string& className){
	return m_classFields[className].size();
}

ClassField* ClassFactory::get_field(const string& className, int pos){
	return m_classFields[className][pos];
}

ClassField* ClassFactory::get_field(const string& className, const string& fieldName){
	auto it = m_classFields.find(className);
	if (it == m_classFields.end()) return nullptr;
	auto vec = m_classFields[className];
	for (auto c : vec) {
		if (c->name() == fieldName)
			return c;
	}
	return nullptr;
}

//反射类方法
void ClassFactory::register_class_method(const string& className, const string& methodName, uintptr_t method) {
	m_classMethods[className].push_back(new ClassMethod(methodName, method));
}

int ClassFactory::get_method_count(const string& className) {
	return m_classMethods[className].size();
}

ClassMethod* ClassFactory::get_class_method(const string& className, int pos){
	return m_classMethods[className][pos];
}

ClassMethod* ClassFactory::get_class_method(const string& className, const string& methodName) {
	auto vec = m_classMethods[className];
	for (auto c : vec) {
		if (c->name() == methodName)
			return c;
	}
	return nullptr;
}