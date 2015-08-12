#include "test.h"
#include <iostream>

class Base1_C1 : public Base1 {
public:
	virtual void bar() override {
		std::cout << "running Base1_C1" << std::endl;
	}
};

class Base1_C2 : public Base1 {
public:
	Base1_C2() {}
	Base1_C2(Base1_C2 const&) = delete;
	virtual void bar() override {
		std::cout << "running Base1_C2" << std::endl;
	}
};

// Register class at factory
namespace {
	genericFactory::Register<Base1, Base1_C1> item1("Base1_C1", true);
	genericFactory::Register<Base1, Base1_C2> item2("Base1_C2", false);
}



