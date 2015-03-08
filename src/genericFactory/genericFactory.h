#ifndef GENERICFACTORY_GENERICFACTORY_H
#define GENERICFACTORY_GENERICFACTORY_H

#include <functional>
#include <map>
#include <memory>
#include <string>

namespace genericFactory {
	template<typename Base>
	struct Class {
		std::function<std::shared_ptr<Base>()> creationFunc;
		std::function<bool(Base*)>             testFunc;
	};

	template<typename Base>
	class GenericFactory {
	private:
		static std::map<std::string, Class<Base>> classList;
		static std::string& getDefaultName() {
			static std::string defaultName;
			return defaultName;
		}
	public:
		GenericFactory(std::string const& _name, Class<Base> _class, bool _default) {
			if (classList.size() == 0 || _default) {
				getDefaultName() = _name;
			}
			classList[_name] = _class;
		}
		static std::shared_ptr<Base> createClass(std::string const& _name) {
			if (classList.find(_name) != classList.end()) {
				return classList.at(_name).creationFunc();
			}
			return nullptr;
		}
		static std::shared_ptr<Base> createDefaultClass() {
			return createClass(getDefaultName());
		}
		static void advance(std::shared_ptr<Base>& base, int step) {
			auto iter = classList.cbegin();
			for (; iter != classList.cend(); ++iter) {
				if (iter->second.testFunc(base.get())) {
					break;
				}
			}

			if (step > 0) {
				++iter;
				if (iter == classList.cend()) {
					iter = classList.cbegin();
				}
			} else if (step < 0) {
				if (iter == classList.cbegin()) {
					iter = classList.cend();
				}
				--iter;
			}
			base = iter->second.creationFunc();
		}
		static std::map<std::string, Class<Base>> const& getClassList() {
			return classList;
		}
	};
	template<typename Base>
	std::map<std::string, Class<Base>> GenericFactory<Base>::classList;


	template<typename Base, typename T>
	class GenericFactoryItem : public GenericFactory<Base> {
	public:
		GenericFactoryItem(std::string const& _name, bool _default = false)
			: GenericFactory<Base>(_name,
			                       {[]() { return std::make_shared<T>(); },
			                       [](Base* b) { return dynamic_cast<T*>(b) != nullptr; }},
			                       _default)
		{}
	};
}
#endif
