/*
 * @Copyright (c) 2014-2020 SenseTime :  All rights reserved.
 * @Description:
 * @Author: 周波
 * @Date: 2021-03-21 22:42:54
 * @LastEditors: 周波
 * @LastEditTime: 2021-04-05 21:27:58
 * @FilePath: \lemon\src\lemon_semaphore.cc
 */
#include <lemon_semaphore.hh>

namespace lemon
{
/**
 * @name: ShareSemaphore
 * @brief: 默认构造函数，根据输入的seam key创建信号量，并设置初始资源为init_value
 * @param {int32_t} sema_key
 * @param {int32_t} init_value
 * @return {}
 */
ShareSemaphore::ShareSemaphore(int32_t sema_key, int32_t init_value) throw()
        : sema_key_(sema_key),
          sema_id_(-1),
          create_(false)
{
    ///TODO:构造的时候可能需要加个锁
    sema_id_ = semget(sema_key_, 1, 0666);
    if (sema_id_ < 0) {
        Print("semget semaphore key", sema_key_, " didn't exist ");
        ///如果共享内存管理没有初始化那么先完成初始化
        sema_id_ = semget(sema_key_, 1, 0666 | IPC_CREAT);
        MISC_CHK_CONDITION_REPORT_EXEC(sema_id_ >= 0, , throw "semget fail", "semget semaphore key ", sema_key_, "failed ", sema_key_);
        create_ = true;
    }

    int32_t ret = 0;
    ///TODO:后续切换为IPC_STAT来获取当前的状态
    if (true == create_) {
        ret = semctl(sema_id_, 0, SETVAL, init_value);  //0代表对1个信号来量初始化，即有1个资源
        MISC_CHK_CONDITION_REPORT_EXEC(ret >= 0, , throw "semctl fail",
                                       "semctl set semaphore ", sema_id_,
                                       " init ", init_value, "failed ", ret);
    }

    Print("semget semaphore key ", sema_key_, " id is ", sema_id_);
}

/**
 * @name: ~ShareSemaphore
 * @brief: 默认析构函数
 * @param {void}
 * @return {}
 */
ShareSemaphore::~ShareSemaphore(void)
{
    int32_t ret = 0;

    //Print("semdel semaphore id ", sema_id_);

    ///删除共享内存
    if (sema_id_ >= 0) {
        ///如果信号量需要删除使用ipcrm,因为可能有其他的进程在使用导致整体异常退出。
        // if (true == create_) {
        //     ret = semctl(sema_id_, 0, IPC_RMID, 0);
        //     MISC_CHK_STATUS_REPORT(ret, , "rm semaphore id ", sema_id_, "failed, ret ", ret);
        //     create_ = false;
        // }

        sema_id_ = -1;
    }
}

/**
 * @name:Lock
 * @brief:P操作
 * @param {void}
 * @return {int32_t}
 */
int32_t ShareSemaphore::Lock(void)
{
    ///使用SEM_UNDO保证当前进程挂了之后内核自动释放pend的信号
    struct sembuf sema_ops = { 0, -1, SEM_UNDO };
    return semop(sema_id_, &sema_ops, 1);
}

/**
 * @name:TryLock
 * @brief:P操作
 * @param {void}
 * @return {int32_t}
 */
int32_t ShareSemaphore::TryLock(void)
{
    struct sembuf sema_ops = { 0, -1, IPC_NOWAIT };
    return semop(sema_id_, &sema_ops, 1);
}

/**
 * @name:Unlock
 * @brief:V操作
 * @param {void}
 * @return {int32_t}
 */
int32_t ShareSemaphore::Unlock(void)
{
    struct sembuf sema_ops = { 0, 1, SEM_UNDO };
    return semop(sema_id_, &sema_ops, 1);
}

}  // namespace lemon
