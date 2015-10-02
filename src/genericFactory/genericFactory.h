#pragma once
#include <map>
#include <memory>
#include <set>
#include <string>
#include <typeinfo>
#include <type_traits>

namespace genericFactory {

class GenericFactory {
private:
	std::map<std::size_t, std::string>            classList;
	std::map<std::string, std::function<void*()>> constructorList;

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
		classList[typeid(T).hash_code()] = _name;
		std::cout<<"is abstrict"<<std::endl;
	}

	template<typename T, typename std::enable_if<not std::is_abstract<T>::value>::type* = nullptr>
	void registerClass(std::string const& _name) {
		classList[typeid(T).hash_code()] = _name;
		constructorList[_name] = [] {
			return (void*)(new T());
		};
	}

	template<typename Base, typename T>
	void registerClass(std::string const& _name) {
		//auto realName = getType<Base>() + "_" + _name;
		auto realName = _name;
		classList[typeid(T).hash_code()] = realName;
		constructorList[realName] = [] { return (void*)new T(); };
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
	T* getItem(std::string const& _name) const {
		return (T*)(constructorList.at(_name)());
	}
	template<typename T>
	T* getItem(T* t) const {
		auto typeName  = getType(t);
		auto _ctorFunc = constructorList.at(typeName);

		return (T*)(_ctorFunc());
	}
	template<typename T>
	std::shared_ptr<T> getSharedItem(std::string const _name) const {
		std::shared_ptr<T> ptr;
		ptr.reset((T*)(constructorList.at(_name)()));
		return ptr;
	}

	template<typename T>
	std::shared_ptr<T> getSharedItem(std::shared_ptr<T> const& _ptr) const {
		std::shared_ptr<T> ptr;
		ptr.reset((T*)(constructorList.at(getType(_ptr.get()))()));
		return ptr;
	}
};

template<typename ...T>
class Register;

template<typename T>
class Register<T> {
public:
	Register(std::string const& _name) {
		static_assert(std::is_polymorphic<T>::value, "must be polymorphic type");
		GenericFactory::getInstance().registerClass<T>(_name);
	}
};
template<typename Base, typename T>
class Register<Base, T> {
public:
	Register(std::string const& _name) {
		static_assert(std::is_polymorphic<Base>::value, "must be polymorphic type");
		static_assert(std::is_polymorphic<T>::value, "must be polymorphic type");
		static_assert(std::is_base_of<Base, T>::value, "Base must be base of T");
		GenericFactory::getInstance().registerClass<Base, T>(_name);
		std::cout<<genericFactory::GenericFactory::getInstance().hasType("Base1")<<std::endl;
	}
};


template<typename T>
inline std::shared_ptr<T> getSharedInstance(std::string const& _name) {
	return GenericFactory::getInstance().getSharedItem<T>(_name);
}
template<typename T>
inline std::unique_ptr<T> getUniqueInstance(std::string const& _name) {
	return GenericFactory::getInstance().getItem<T>(_name);
}

}

