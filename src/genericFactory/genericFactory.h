#ifndef GENERICFACTORY_GENERICFACTORY_H
#define GENERICFACTORY_GENERICFACTORY_H

#include <cassert>
#include <functional>
#include <map>
#include <memory>
#include <set>
#include <string>

namespace genericFactory {
	template<typename Base>
	struct Class {
		std::function<Base*()>            creationFunc;
		std::function<Base*(Base const*)> copyFunc;
		std::function<bool(Base const*)>  testFunc;
	};

	template<typename T>
	class Item;

	template<typename Base, typename T>
	class Register;

	template<typename Base>
	class GenericFactory {
	private:
		static std::map<std::string, Class<Base>> classList;

		friend class Item<Base>;

		static std::string& getDefaultName() {
			static std::string defaultName;
			return defaultName;
		}
	public:
		static void registerClass(std::string const& _name, Class<Base> _class, bool _default) {
			static bool defaultNameWasSet { false };
			if (classList.size() == 0 || _default) {
				if (_default) {
					assert(not defaultNameWasSet && "genericFactory: default Object for this type defined twice");
					defaultNameWasSet = true;
				}
				getDefaultName() = _name;
			}
			classList[_name] = _class;
		}
	private:

		static Base* createClass(std::string const& _name) {
			if (classList.find(_name) != classList.end()) {
				return classList.at(_name).creationFunc();
			}
			assert(false && "tried to create invalid class");
			return nullptr;
		}

		static Base* copyClass(Base const* base) {
			auto iter = classList.cbegin();
			for (; iter != classList.cend(); ++iter) {
				if (iter->second.testFunc(base)) {
					return iter->second.copyFunc(base);
				}
			}
			assert(false && "tried to copy invalid class");
			return nullptr;
		}

		static std::set<std::string> getClassList() {
			std::set<std::string> classNameSet;
			for (auto const& e : classList) {
				classNameSet.insert(e.first);
			}
			return classNameSet;
		}
	};
	template<typename Base>
	std::map<std::string, Class<Base>> GenericFactory<Base>::classList;

	template<typename Base, typename T>
	std::function<Base*(Base const*)> __getCopyFunc() {
		std::function<Base*(Base const*)> f = [](Base const* b) {
			T const* t = dynamic_cast<T const*>(b);
			return new T(*t);
		};
		return f;
	}


	template<typename Base, typename T>
	auto _getCopyFunc(int) -> decltype(T((T const&)T()), std::function<Base*(Base const*)>()) {
		return __getCopyFunc<Base, T>();
	}
	template<typename Base, typename T>
	auto _getCopyFunc(long) -> decltype(std::function<Base*(Base const*)>()) {
		return [](Base const*) {
			assert(false && "Item doesn't have copy constructor implemented");
			return nullptr;
		};
	}

	template<typename Base, typename T>
	std::function<Base*(Base const*)> getCopyFunc() {
		return _getCopyFunc<Base, T>(int(0));
	}

	template<typename Base, typename T>
	class Register {
	public:
		Register(std::string const& _name, bool _default = false) {
			GenericFactory<Base>::registerClass(_name,
			                       Class<Base> {[]() { return new T; },
			                                    genericFactory::getCopyFunc<Base, T>(),
			                                    [](Base const* b) { return dynamic_cast<T const*>(b) != nullptr; }
			                                   },
			                       _default);


		}
	};
	template<typename Base>
	class Item {
		std::string type;
		std::unique_ptr<Base> base;
	public:
		Item()
			: type(GenericFactory<Base>::getDefaultName())
			, base(GenericFactory<Base>::createClass(type)) {
		}

		Item(Item const& _item) {
			*this = _item;
		}
		Item(Item&& _item) {
			*this = std::move(_item);
		}

		Item(std::string const _type)
			: type(_type)
			, base(GenericFactory<Base>::createClass(type)) {
		}

		Base& operator*() const {
			return *(base.get());
		}
		Base& operator*() {
			return *(base.get());
		}
		Base* operator->() const {
			return base.get();
		}
		Base* operator->() {
			return base.get();
		}

		Item& operator=(Item const& _item) {
			type = _item.type;
			base.reset(GenericFactory<Base>::copyClass(_item.base.get()));
			return *this;
		}
		Item& operator=(Item&& _item) {
			type = _item.type;
			base = std::move(_item.base);
			return *this;
		}
		std::string const& getType() const {
			return type;
		}
		static std::set<std::string> getClassList() {
			return GenericFactory<Base>::getClassList();
		}

	};
}
#endif
