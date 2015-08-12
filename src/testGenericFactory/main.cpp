#include "test.h"
#include <iostream>

int main(int, char**) {
	{
		genericFactory::Item<Base1> a_ptr;
		a_ptr->bar();
	}
	{
		genericFactory::Item<Base1> a_ptr("Base1_C1");
		a_ptr->bar();
	}
	{
		genericFactory::Item<Base1> a_ptr("Base1_C2");
		a_ptr->bar();
	}
	{
		genericFactory::Item<Base1> a1_ptr;
		genericFactory::Item<Base1> a2_ptr;
		a1_ptr = a2_ptr;
		a1_ptr->bar();
	}
	{
		genericFactory::Item<Base1> a1_ptr("Base1_C1");
		genericFactory::Item<Base1> a2_ptr(a1_ptr);
		a2_ptr->bar();
	}
/*	{
		genericFactory::Item<Base1> a1_ptr("Base1_C2");
		genericFactory::Item<Base1> a2_ptr(a1_ptr);
		a2_ptr->bar();
	}*/

	{
		for (auto const& c : genericFactory::Item<Base1>::getClassList()) {
			std::cout << c << std::endl;
		}
	}

	return EXIT_SUCCESS;
}
