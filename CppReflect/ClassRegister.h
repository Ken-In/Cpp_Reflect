#pragma once

#include <string>
using namespace std;

#include "ClassFactory.h"
using namespace congb::reflect;

namespace congb {
namespace reflect {

	//����ע���� ʵ������ע�Ṧ��
	class ClassRegister {
	public:
		// ע��class
		ClassRegister(const string& className, create_object method) {
			Singleton<ClassFactory>::instance()->register_class(className, method);
		}

		// ע��class_field
		ClassRegister(const string& className, const string& fieldName, const string& fieldType, size_t offset) {
			Singleton<ClassFactory>::instance()->register_class_field(className, fieldName, fieldType, offset);
		}

		// ע��class_method
		ClassRegister(const string& className, const string& methodName, uintptr_t method) {
			Singleton<ClassFactory>::instance()->register_class_method(className, methodName, method);
		}
	};

#define OFFSET(className, fieldName)\
	(size_t)&(className.fieldName) - (size_t)&className

//������ָ��createObject##className����map��
#define REGISTER_CLASS(className)\
	Object* createObject##className(){\
		Object* obj = new className();\
		obj->set_class_name(#className);\
		return obj;\
	}\
	ClassRegister classRegister##className(#className,createObject##className)

//��class_field����map��
#define REGISTER_CLASS_FIELD(className,fieldName,fieldType)\
	className className##fieldName;\
	ClassRegister classRegister##className##fieldName( #className, #fieldName, #fieldType, OFFSET(className##fieldName, fieldName) )

//��class_method����map
#define REGISTER_CLASS_METHOD(className,methodName, returnType, ...)\
	std::function<returnType(className *,##__VA_ARGS__)> className##methodName = &className::methodName;\
	ClassRegister classRegister##className##methodName(#className, #methodName, (uintptr_t)&(className##methodName))

}
}


