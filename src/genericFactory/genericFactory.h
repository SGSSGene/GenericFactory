#include <map>
#include <memory>
#include <set>
#include <string>
#include <typeinfo>
#include <type_traits>

namespace genericFactory {

class GenericFactory {
private:
	static std::map<std::size_t, std::string>& getClassList() {
		static std::map<std::size_t, std::string> map;
		return map;
	}
	static std::map<std::string, std::function<void*()>> getConstructorList() {
		static std::map<std::string, std::function<void*()>> map;
		return map;
	}

public:
	template<typename T>
	static std::set<std::string> getValidNames() {
		std::set<std::string> validNames;
		auto typeName = getType<T>() + "_";
		for (auto const& e : getClassList()) {
			if (0 == e.second.find(typeName)) {
				validNames.insert(e.second);
			}
		}
		return validNames;
	}


	template<typename T, typename std::enable_if<std::is_abstract<T>::value>::type* = nullptr>
	static void registerClass(std::string const& _name) {
		getClassList()[typeid(T).hash_code()] = _name;
	}

	template<typename T, typename std::enable_if<not std::is_abstract<T>::value>::type* = nullptr>
	static void registerClass(std::string const& _name) {
		getClassList()[typeid(T).hash_code()] = _name;
		getConstructorList()[_name] = []() -> void* { return new T(); };
	}

	template<typename Base, typename T>
	static void registerClass(std::string const& _name) {
		auto realName = getType<Base>() + "_" + _name;
		getClassList()[typeid(T).hash_code()] = realName;
		getConstructorList()[_name] = []() -> void* { return new T(); };
	}

	template<typename T>
	static std::string getType() {
		return getClassList()[typeid(T).hash_code()];
	}

	template<typename T>
	static std::string getType(T* t) {
		return getClassList()[typeid(*t).hash_code()];
	}


	template<typename T>
	static T* getInstance(std::string const& _name) {
		return (T*)getConstructorList()[_name]();
	}
	template<typename T>
	static T* getInstance(T const* t) {
		return (T*)getConstructorList()[getType(t)]();
	}
	template<typename T>
	static std::shared_ptr<T> getSharedInstance(std::string const _name) {
		std::shared_ptr<T> ptr;
		ptr.reset((T*)(getConstructorList()[_name]()));
		return ptr;
	}

	template<typename T>
	static std::shared_ptr<T> getSharedInstance(std::shared_ptr<T> const& _ptr) {
		std::shared_ptr<T> ptr;
		ptr.reset((T*)getConstructorList()[getType(_ptr.get())]());
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
		GenericFactory::registerClass<T>(_name);
	}
};
template<typename Base, typename T>
class Register<Base, T> {
public:
	Register(std::string const& _name) {
		static_assert(std::is_polymorphic<Base>::value, "must be polymorphic type");
		static_assert(std::is_polymorphic<T>::value, "must be polymorphic type");
		static_assert(std::is_base_of<Base, T>::value, "Base must be base of T");
		GenericFactory::registerClass<Base, T>(_name);
	}
};


}

