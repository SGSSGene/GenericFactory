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
	genericFactory::Register<A, B1> item1("B1");
	genericFactory::Register<A, B2> item2("B2", true);
}
```
### Using the Factory
```c
int main(int, char**) {
	genericFactory::Item<A> a_default();
	genericFactory::Item<A> a_b1("B1");
	genericFactory::Item<A> a_b2("B2");
	a_default->bar();
	a_b1->bar();
	a_b2->bar();

	std::set<std::string classList = genericFactory::Item<A>::getClassList();
	for (auto c : classList) {
		std::cout << c << std::endl;
	}
	return 0;
}
```

