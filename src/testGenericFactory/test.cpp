/** \example ./src/testGenericFactory/test.cpp
 */

#include <iostream>
#include <gtest/gtest.h>
#include <genericFactory/genericFactory.h>

class Base1 {
public:
	virtual std::string bar() = 0;
};

class Deriv1_of_Base1 : public Base1 {
public:
	std::string bar() override {
		return "Deriv1_of_Base1";
	}
};

class Deriv2_of_Base1 : public Base1 {
public:
	std::string bar() override {
		return "Deriv2_of_Base1";
	}
};

// Register class at factory
namespace {
	genericFactory::Register<Base1>                  base("Base1");
	genericFactory::Register<Base1, Deriv1_of_Base1> item1("Deriv1_of_Base1");
	genericFactory::Register<Base1, Deriv2_of_Base1> item2("Deriv2_of_Base1");
}

TEST(Test, Test1) {
	auto ptr1 = genericFactory::getSharedInstance<Base1>("Deriv1_of_Base1");
	std::cout<<ptr1->bar()<<std::endl;
	EXPECT_EQ(10, 10);
}





