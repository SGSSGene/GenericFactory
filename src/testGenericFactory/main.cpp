#include "test.h"
#include <iostream>

int main(int, char**) {
/*	{
		auto a_ptr = genericFactory::GenericFactory::getSharedInstance<Base1>("Base1_C1");
		a_ptr->bar();
	}
	{
		auto a_ptr = genericFactory::GenericFactory::getSharedInstance<Base1>("Base1_C2");
		a_ptr->bar();
	}*/
	{
		for (auto const& c : genericFactory::GenericFactory::getValidNames<Base1>()) {
			std::cout << c << std::endl;
		}
	}

	return EXIT_SUCCESS;
}
