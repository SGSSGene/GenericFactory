#include "test.h"
#include <iostream>

class B1 : public A {
public:
	virtual void bar() override {
		std::cout << "running B1" << std::endl;
	}
};

class B2 : public A {
public:
	virtual void bar() override {
		std::cout << "running B2" << std::endl;
	}
};

// Register class at factory
namespace {
	genericFactory::Register<A, B1> item1("B1", true);
	genericFactory::Register<A, B2> item2("B2");
}



