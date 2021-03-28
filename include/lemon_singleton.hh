/*
 * @Copyright (c) 2014-2020 SenseTime :  All rights reserved.
 * @Description:
 * @Author: 周波
 * @Date: 2021-03-21 22:42:54
 * @LastEditors: 周波
 * @LastEditTime: 2021-03-28 22:41:42
 * @FilePath: \lemon\include\lemon_singleton.hh
 */
#ifndef __LEMON_SINGLETON_HH__
#define __LEMON_SINGLETON_HH__

namespace lemon
{
/**
*@ brief 单例模式通用模板
*/
template <typename T>
class Singleton
{
    public:
    ///获取单例的实例对象
    static T *GetInstance(void)
    {
        if (nullptr == GetSingleInstance().instance_) {
            GetSingleInstance().instance_ = new T;
            Print("new instance_ ", GetSingleInstance().instance_);
        }

        return GetSingleInstance().instance_;
    };

    ///销毁单例的实际对象
    static void DestroyInstance(void)
    {
        if (nullptr != GetSingleInstance().instance_) {
            Print("delete instance_ ", GetSingleInstance().instance_);
            delete GetSingleInstance().instance_;
            GetSingleInstance().instance_ = nullptr;
        }
    };

    private:
    ///默认构造函数
    Singleton(void)
            : instance_(nullptr){};

    ///默认析构函数
    ~Singleton(void)
    {
    }

    ///拷贝构造函数禁用
    Singleton(const Singleton &singleton) = delete;

    ///赋值构造函数禁用
    Singleton &operator=(const Singleton &singleton) = delete;

    ///右值拷贝构造函数禁用
    Singleton(const Singleton &&singleton) = delete;

    ///右值赋值构造函数禁用
    Singleton &operator=(const Singleton &&singleton) = delete;

    ///获取实例
    static Singleton &GetSingleInstance(void)
    {
        static Singleton single_instance;
        return single_instance;
    };

    private:
    ///实例T
    T *instance_;
};

}  // namespace lemon

#endif
