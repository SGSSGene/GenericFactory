#include "test.h"
#include <iostream>

int main(int, char**) {
	{
		genericFactory::Item<A> a_ptr;
		a_ptr->bar();
	}
	{
		genericFactory::Item<A> a_ptr("B1");
		a_ptr->bar();
	}
	{
		genericFactory::Item<A> a_ptr("B2");
		a_ptr->bar();
	}
	{
		genericFactory::Item<A> a1_ptr("B1");
		genericFactory::Item<A> a2_ptr("B2");
		a1_ptr = a2_ptr;
		a1_ptr->bar();
	}
	{
		genericFactory::Item<A> a1_ptr("B1");
		genericFactory::Item<A> a2_ptr(a1_ptr);
		a2_ptr->bar();
	}
	{
		for (auto const& c : genericFactory::Item<A>::getClassList()) {
			std::cout << c << std::endl;
		}
	}

	return EXIT_SUCCESS;
}
