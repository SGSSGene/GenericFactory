# GenericFactory

This library provides a generic implementation of the factory pattern.

## Example
### Register classes at the factory
```
struct A {
	virtual void bar() = 0;
};
struct B1 : public A {
	void bar() override {
		std::cout << "calling B1" << std::endl;
	}
};
struct B2 : public A {
	void bar() override {
		std::cout << "calling B2" << std::endl;
	}
}

// Register class at factory with B2 as default
namespace {
	genericFactory::GenericFactoryItem<A, B1> item1("B1");
	genericFactory::GenericFactoryItem<A, B2> item2("B2", true);
}
```
### Using the Factory
```c
int main(int, char**) {
	auto a_ptr  = genericFactory::GenericFactory<A>::createDefaultClass();
	a_ptr->bar();
	auto b_ptr = genericFactory::GenericFactory<A>::createClass("B1");
	b_ptr->bar();

	for (auto c : genericFactory::GenericFactory<A>::getClassList()) {
		std::cout << c.first << std::endl;
	}
	return 0;
}
```

