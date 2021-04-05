/*
 * @Copyright (c) 2014-2020 SenseTime :  All rights reserved.
 * @Description:
 * @Author: 周波
 * @Date: 2021-03-21 22:42:54
 * @LastEditors: 周波
 * @LastEditTime: 2021-04-05 16:13:19
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
        std::unique_lock<std::mutex> unique_mutex(GetMutex());
        if (nullptr == GetSingleInstance().instance_) {
            GetSingleInstance().instance_ = new T;
            Print("new instance_ ", GetSingleInstance().instance_);
        }

        return GetSingleInstance().instance_;
    };

    ///获取单例的实例对象状态，是否存在
    static bool GetInstanceStatus(void)
    {
        std::unique_lock<std::mutex> unique_mutex(GetMutex());
        return (nullptr != GetSingleInstance().instance_) ? true : false;
    };

    ///销毁单例的实际对象
    static void DestroyInstance(void)
    {
        std::unique_lock<std::mutex> unique_mutex(GetMutex());
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

    ///获取实例
    static std::mutex &GetMutex(void)
    {
        static std::mutex mutex;
        return mutex;
    };

    private:
    ///实例T
    T *instance_;
};

}  // namespace lemon

#endif
