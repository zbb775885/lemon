/*
 * @Copyright (c) 2014-2020 SenseTime :  All rights reserved.
 * @Description:
 * @Author: 周波
 * @Date: 2021-03-21 22:42:54
 * @LastEditors: 周波
 * @LastEditTime: 2021-04-03 16:01:13
 * @FilePath: \lemon\include\lemon_unique_lock.hh
 */
#ifndef __LEMON_UNIQUE_LOCK_HH__
#define __LEMON_UNIQUE_LOCK_HH__

#include <stdint.h>

namespace lemon
{
///简易的智能锁模板，不支持默认构造，左右值拷贝构造，左右值复制构造
///支持模式如下:UniqueLock<ShareSemaphore> unique_lock(seam);
template <typename _Mutex>
class UniqueLock
{
    public:
    ///锁类型
    using MutexType = _Mutex;

    /**
     * @name:UniqueLock
     * @brief: 构造智能锁
     * @param {MutexType} &mutex, 引用的锁
     * @return {}
     */
    explicit UniqueLock(MutexType &mutex)
            : mutex_(std::__addressof(mutex))
    {
        Lock();
    }

    /**
     * @name:~UniqueLock
     * @brief: 析构构造智能锁
     * @param {void}
     * @return {}
     */
    ~UniqueLock(void)
    {
        if (nullptr != mutex_) {
            mutex_->Unlock();
        }
    }

    /**
     * @name:Lock
     * @brief: 上锁
     * @param {void}
     * @return {}
     */
    void Lock(void)
    {
        if (nullptr != mutex_) {
            mutex_->Lock();
        }
    }

    /**
     * @name:Unlock
     * @brief: 释放锁
     * @param {void}
     * @return {}
     */
    void Unlock(void)
    {
        if (nullptr != mutex_) {
            mutex_->Unlock();
        }
    }

    ///默认构造函数禁用
    UniqueLock(void) = delete;

    ///拷贝构造函数禁用
    UniqueLock(const UniqueLock &unique_lock) = delete;

    ///赋值构造函数禁用
    UniqueLock &operator=(const UniqueLock &unique_lock) = delete;

    ///右值拷贝构造函数禁用
    UniqueLock(const UniqueLock &&unique_lock) = delete;

    ///右值赋值构造函数禁用
    UniqueLock &operator=(const UniqueLock &&unique_lock) = delete;

    private:
    ///锁对象
    MutexType *mutex_;
};
}  // namespace lemon

#endif
