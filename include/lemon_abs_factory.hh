#ifndef __LEMON_ABS_FACTORY_HH__
#define __LEMON_ABS_FACTORY_HH__

#include <unordered_map>
#include <functional>
#include <iostream>

namespace lemon
{
/**
 * @name:AbsFactory
 * @brief:Abstract factory, used to generate factories, _Base is the base class,
 * @brief:Args is the derive class constructor param
 * @brief:and can produce subclasses derived from _Base,
 * @brief:and the derived subclasses must be public inheritance
 * @brief:The factory’s key defaults to string type, if you need to customize other types,
 * @brief:you can customize the settings when instantiating
 * @brief:When using, first to complete the registration,
 * @brief:and then use Create function to create the Derive class
 * @param {_Base} base class
 * @param {_Key} factory map's key
 * @param {_Base} base class
 * @return {Args...} constructor param of derived class
 */
template <typename _Base, typename _Key = std::string, typename... Args>
class AbsFactory
{
    private:
    ///Return pointer to _Base
    using Type = _Base *;

    ///Generate functions for derived classes
    using CreateFunc = std::function<Type(Args...)>;

    ///Factory pair
    using PairFactory = std::pair<_Key, CreateFunc>;

    ///factory map
    using FactoryMap = std::unordered_map<_Key, CreateFunc>;

    public:
    /**
     * @name:Register
     * @brief:Register according to the key and variable parameter template parameter Args
     * @param {const_Key} &key
     * @return {bool}
     */
    template <typename _Derive>
    static bool Register(const _Key &key)
    {
        auto &factory_map = GetFactoryMap();
        auto &&func = [](const Args &...args) -> Type { return new _Derive(args...); };
        auto &&pair = PairFactory(key, func);
        auto ret = factory_map.insert(pair);
        // std::cout << "register " << key << std::endl;
        // std::cout << "addr is " << std::__addressof(GetFactoryMap()) << std::endl;
        return ret.second;
    }

    /**
     * @name:Create
     * @brief:Create an object based on the key value, if there is a parameter, then pass in the parameter
     * @param {const _Key} &key
     * @param {const Args} &...args
     * @return {Type}
     */
    static Type Create(const _Key &key, const Args &...args)
    {
        // std::cout << "size is " << GetFactoryMap().size() << std::endl;
        // std::cout << "addr is " << std::__addressof(GetFactoryMap()) << std::endl;
        auto &&iter = GetFactoryMap().find(key);
        if (iter != GetFactoryMap().end()) {
            return iter->second(args...);
        }

        return nullptr;
    };

    private:
    /**
     * @name:GetFactoryMap
     * @brief:Get factory table, the factory type is generated according to _Key, _Base, Args...
     * @param {void}
     * @return {FactoryMap &}
     */
    static FactoryMap &GetFactoryMap(void)
    {
        static FactoryMap factory_map;
        return factory_map;
    }
};

// template <typename _Base, typename _Key = std::string>
// class AbsFactory
// {
//     private:
//     ///Return pointer to _Base
//     using Type = _Base *;

//     ///Generate functions for derived classes
//     template <typename... Args>
//     using CreateFunc = std::function<Type(Args...)>;

//     ///factory map
//     template <typename... Args>
//     using FactoryMap = std::unordered_map<_Key, CreateFunc<Args...>>;

//     public:
//     /**
//      * @name:Register
//      * @brief:根据key之和可变参数args注册
//      * @param {const_Key} &key
//      * @param {constArgs} &
//      * @return {*}
//      */
//     template <typename _Derive, typename... Args>
//     static bool Register(const _Key &key)
//     {
//         auto &factory_map = GetFactoryMap<Args...>();
//         auto &&func = [](const Args &...args) -> Type { return new _Derive(args...); };
//         auto &&pair = std::pair<_Key, CreateFunc<Args...>>(key, func);
//         auto ret = factory_map.insert(pair);

//         return ret.second;
//     }

//     template <typename... Args>
//     static Type Create(const _Key &key, const Args &...args)
//     {
//         auto &&iter = GetFactoryMap<Args...>().find(key);
//         if (iter != GetFactoryMap<Args...>().end()) {
//             return iter->second(args...);
//         }

//         return nullptr;
//     };

//     private:
//     template <typename... Args>
//     static FactoryMap<Args...> &GetFactoryMap(void)
//     {
//         static FactoryMap<Args...> factory_map;
//         return factory_map;
//     }
// };
}  // namespace lemon

#endif
