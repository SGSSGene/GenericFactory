#pragma once
#include <map>
#include <memory>
#include <set>
#include <string>
#include <typeinfo>
#include <type_traits>

namespace genericFactory {

class Base {
public:
	virtual ~Base() {}
};

template<typename B>
class BaseT : public Base {
private:
	virtual std::shared_ptr<B> createSharedBase() const = 0;
	virtual B*                 createUniqueBase() const = 0;
public:
	template<typename T2>
	std::shared_ptr<T2> createShared() const {
		auto ptr1 = createSharedBase();
		auto ptr2 = std::dynamic_pointer_cast<T2>(ptr1);
		if (ptr2.get() == nullptr) {
			throw std::runtime_error("can't create shared pointer (genericFactory)");
		}
		return ptr2;
	}
	template<typename T2>
	std::unique_ptr<T2> createUnique() const {
		auto ptr1 = createUniqueBase();
		auto ptr2 = dynamic_cast<T2*>(ptr1);
		if (ptr2 == nullptr) {
			delete ptr1;
			throw std::runtime_error("can't create unique pointer (genericFactory)");
		}
		return std::unique_ptr<T2> { ptr2 };
	}
};

template<typename B, typename T>
class BaseTT final : public BaseT<B> {
public:
	std::shared_ptr<B> createSharedBase() const override {
		return std::make_shared<T>();
	}
	B* createUniqueBase() const override {
		return new T();
	}
};

class GenericFactory {
private:
	std::map<std::size_t, std::string>                        classList;
	std::map<std::string, std::vector<std::unique_ptr<Base>>> constructorList;

public:
	static GenericFactory& getInstance() {
		static GenericFactory* instance = new GenericFactory();
		return *instance;
	}

	template<typename T>
	std::set<std::string> getValidNames() const {
		std::set<std::string> validNames;
		auto typeName = getType<T>() + "_";
		for (auto const& e : classList) {
			if (0 == e.second.find(typeName)) {
				validNames.insert(e.second);
			}
		}
		return validNames;
	}


	template<typename T, typename std::enable_if<std::is_abstract<T>::value>::type* = nullptr>
	void registerClass(std::string const& _name) {
		static_assert(std::is_polymorphic<T>::value, "must be polymorphic type");
		classList[typeid(T).hash_code()] = _name;
	}

	template<typename T, typename std::enable_if<not std::is_abstract<T>::value>::type* = nullptr>
	void registerClass(std::string const& _name) {
		static_assert(std::is_polymorphic<T>::value, "must be polymorphic type");
		classList[typeid(T).hash_code()] = _name;
		constructorList[_name].emplace_back(new BaseTT<T, T>());
	}

	template<typename T, typename Base, typename ...Bases>
	void registerClass(std::string const& _name) {
		registerClass<T, Bases...>(_name);
		classList[typeid(T).hash_code()] = _name;
		constructorList[_name].emplace_back(new BaseTT<Base, T>());
	}
	template<typename T>
	std::string const& getType() const {
		return classList.at(typeid(T).hash_code());
	}

	template<typename T>
	std::string const& getType(T* t) {
		return classList.at(typeid(*t).hash_code());
	}

	template<typename T>
	bool hasType() const {
		return classList.find(typeid(T).hash_code()) != classList.end();
	}
	template<typename T>
	bool hasType(T* t) const {
		return classList.find(typeid(*t).hash_code()) != classList.end();
	}


	template<typename T>
	std::unique_ptr<T> getUniqueItem(std::string const& _name) const {
		for (auto const& base : constructorList.at(_name)) {
			auto ptr = dynamic_cast<BaseT<T> const*>(base.get());
			if (ptr != nullptr) {
				return std::unique_ptr<T> { ptr->template createUnique<T>() };
			}
		}
		throw std::runtime_error("couldn't create unique item");
	}
	template<typename T>
	std::shared_ptr<T> getSharedItem(std::string const _name) const {
		for (auto const& base : constructorList.at(_name)) {
			auto ptr = dynamic_cast<BaseT<T> const*>(base.get());
			if (ptr != nullptr) {
				return ptr->template createShared<T>();
			}
		}
		throw std::runtime_error("couldn't create shared item");
	}
};


template<typename T, typename ...Bases>
class Register {
public:
	Register(std::string const& _name) {
		staticAssertClass<T, Bases...>();
		GenericFactory::getInstance().registerClass<T, Bases...>(_name);
	}
private:
	template<typename T2>
	void staticAssertClass() const {
		static_assert(std::is_polymorphic<T2>::value, "must be polymorphic type");
		static_assert(std::has_virtual_destructor<T2>::value, "must have a virtual destructor");
	}
	template<typename T2, typename Base, typename ...Bases2>
	void staticAssertClass() const {
		staticAssertClass<Base, Bases2...>();
		static_assert(std::is_polymorphic<Base>::value, "must be polymorphic type");
		static_assert(std::is_polymorphic<T2>::value, "must be polymorphic type");
		static_assert(std::is_base_of<Base, T>::value, "Base must be base of T");
		static_assert(std::has_virtual_destructor<T2>::value, "must have a virtual destructor");
	}


};


template<typename T, typename std::enable_if<std::is_polymorphic<T>::value>::type* = nullptr>
inline std::shared_ptr<T> make_shared(std::string const& _name) {
	return GenericFactory::getInstance().getSharedItem<T>(_name);
}
template<typename T, typename std::enable_if<not std::is_polymorphic<T>::value>::type* = nullptr>
inline std::shared_ptr<T> make_shared(std::string const& _name) {
	throw std::runtime_error("this should not happen (genericFactory");
}

template<typename T, typename std::enable_if<std::is_polymorphic<T>::value>::type* = nullptr>
inline std::unique_ptr<T> make_unique(std::string const& _name) {
	return GenericFactory::getInstance().getUniqueItem<T>(_name);
}
template<typename T, typename std::enable_if<not std::is_polymorphic<T>::value>::type* = nullptr>
inline std::unique_ptr<T> make_unique(std::string const& _name) {
	throw std::runtime_error("this should not happen (genericFactory");
}

template<typename T>
inline std::shared_ptr<T> make_shared(T* _t) {
	auto type = GenericFactory::getInstance().getType(_t);
	return make_shared<T>(type);
}
template<typename T>
inline std::unique_ptr<T> make_unique(T* _t) {
	auto type = GenericFactory::getInstance().getType(_t);
	return make_unique<T>(type);
}


}

