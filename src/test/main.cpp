#include "test.h"
#include <iostream>

int main(int, char**) {
	{
		auto a_ptr  = genericFactory::GenericFactory<A>::createDefaultClass();
		a_ptr->bar();
	}
	{
		auto a_ptr = genericFactory::GenericFactory<A>::createClass("B1");
		a_ptr->bar();
	}
	{
		auto a_ptr = genericFactory::GenericFactory<A>::createClass("B2");
		a_ptr->bar();
	}
	{
		for (auto c : genericFactory::GenericFactory<A>::getClassList()) {
			std::cout << c.first << std::endl;
		}
	}

	return EXIT_SUCCESS;
}
