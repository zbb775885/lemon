/*
 * @Copyright (c) 2014-2020 SenseTime :  All rights reserved.
 * @Description:
 * @Author: 周波
 * @Date: 2021-03-21 22:42:54
 * @LastEditors: 周波
 * @LastEditTime: 2021-04-03 15:42:26
 * @FilePath: \lemon\include\lemon_semaphore.hh
 */
#ifndef __LEMON_SEMAPHORE_HH__
#define __LEMON_SEMAPHORE_HH__

#include <stdint.h>
#include <sys/sem.h>
#include <vector>
#include <lemon_log.hh>
#include <lemon_misc.hh>

namespace lemon
{
/**
*@ brief 进程间通信信号量
*/
class ShareSemaphore
{
    public:
    /**
     * @name: ShareSemaphore
     * @brief: 默认构造函数，根据输入的seam key创建信号量，并设置初始资源为init_value,初始化失败会throw int32_t类型失败
     * @param {int32_t} sema_key
     * @param {int32_t} init_value
     * @return {}
     */
    ShareSemaphore(int32_t sema_key, int32_t init_value) throw();

    /**
     * @name: ~ShareSemaphore
     * @brief: 默认析构函数
     * @param {void}
     * @return {}
     */
    ~ShareSemaphore(void);

    /**
     * @name:Lock
     * @brief:P操作
     * @param {void}
     * @return {int32_t}
     */
    int32_t Lock(void);

    /**
     * @name:TryLock
     * @brief:P操作
     * @param {void}
     * @return {int32_t}
     */
    int32_t TryLock(void);

    /**
     * @name:Unlock
     * @brief:V操作
     * @param {void}
     * @return {int32_t}
     */
    int32_t Unlock(void);

    private:
    ///默认构造函数禁用
    ShareSemaphore(void) = delete;

    ///拷贝构造函数禁用
    ShareSemaphore(const ShareSemaphore &share_semaphone) = delete;

    ///赋值构造函数禁用
    ShareSemaphore &operator=(const ShareSemaphore &share_semaphone) = delete;

    ///右值拷贝构造函数禁用
    ShareSemaphore(const ShareSemaphore &&share_semaphone) = delete;

    ///右值赋值构造函数禁用
    ShareSemaphore &operator=(const ShareSemaphore &&share_semaphone) = delete;

    private:
    ///信号量的key
    const key_t sema_key_;

    ///信号量的id
    int32_t sema_id_;

    ///当前信号量是否create出来的
    bool create_;
};

}  // namespace lemon

#endif
