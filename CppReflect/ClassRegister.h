#pragma once

#include <string>
using namespace std;

#include "ClassFactory.h"
using namespace congb::reflect;

namespace congb {
namespace reflect {

	//辅助注册类 实现所有注册功能
	class ClassRegister {
	public:
		// 注册class
		ClassRegister(const string& className, create_object method) {
			Singleton<ClassFactory>::instance()->register_class(className, method);
		}

		// 注册class_field
		ClassRegister(const string& className, const string& fieldName, const string& fieldType, size_t offset) {
			Singleton<ClassFactory>::instance()->register_class_field(className, fieldName, fieldType, offset);
		}

		// 注册class_method
		ClassRegister(const string& className, const string& methodName, uintptr_t method) {
			Singleton<ClassFactory>::instance()->register_class_method(className, methodName, method);
		}
	};

#define OFFSET(className, fieldName)\
	(size_t)&(className.fieldName) - (size_t)&className

//将函数指针createObject##className加入map中
#define REGISTER_CLASS(className)\
	Object* createObject##className(){\
		Object* obj = new className();\
		obj->set_class_name(#className);\
		return obj;\
	}\
	ClassRegister classRegister##className(#className,createObject##className)

//将class_field加入map中
#define REGISTER_CLASS_FIELD(className,fieldName,fieldType)\
	className className##fieldName;\
	ClassRegister classRegister##className##fieldName( #className, #fieldName, #fieldType, OFFSET(className##fieldName, fieldName) )

//将class_method加入map
#define REGISTER_CLASS_METHOD(className,methodName, returnType, ...)\
	std::function<returnType(className *,##__VA_ARGS__)> className##methodName = &className::methodName;\
	ClassRegister classRegister##className##methodName(#className, #methodName, (uintptr_t)&(className##methodName))

}
}


