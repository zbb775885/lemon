/*
 * @Copyright (c) 2014-2020 SenseTime :  All rights reserved.
 * @Description:
 * @Author: 周波
 * @Date: 2021-03-21 22:42:54
 * @LastEditors: 周波
 * @LastEditTime: 2021-04-03 21:41:35
 * @FilePath: \lemon\include\lemon_share_mem.hh
 */
#ifndef __LEMON_SHARED_MEM_HH__
#define __LEMON_SHARED_MEM_HH__

#include <stdint.h>
#include <string.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <algorithm>
#include <lemon_log.hh>
#include <lemon_misc.hh>
#include <lemon_semaphore.hh>

namespace lemon
{
/**
*@ brief 共享内存
*/
template <typename _MemNode, uint32_t _NodeSize = 1>
class ShareMem
{
    public:
    /**
     * @name: ShareMem
     * @brief: 默认构造函数
     * @param {void}
     * @return {*}
     */
    ShareMem(int32_t shm_key, int32_t sema_key)
            : shm_key_(shm_key),
              share_mem_id_(-1),
              share_mem_node_(nullptr),
              create_(false),
              share_seam_(sema_key, 1)
    {
        ///TODO:构造的时候可能需要加个锁
        share_mem_id_ = shmget(shm_key_, sizeof(_MemNode) * _NodeSize, 0666);
        if (share_mem_id_ < 0) {
            Print("shmget share mem key", shm_key_, " didn't exist ");
            ///如果共享内存管理没有初始化那么先完成初始化
            share_mem_id_ = shmget(shm_key_, sizeof(_MemNode) * _NodeSize, 0666 | IPC_CREAT);
            MISC_CHK_CONDITION_REPORT(share_mem_id_ >= 0, , "shmget share mem key ", shm_key_, "failed");
            create_ = true;
        }

        //将共享内存连接到当前进程的地址空间
        share_mem_node_ = static_cast<_MemNode *>(shmat(share_mem_id_, nullptr, 0));
        MISC_CHK_UN_NULL_REPORT(share_mem_node_, , "shmat share mem id ", share_mem_id_, " to virt addr failed");

        Print("shmget share mem key ", shm_key_, " addr is ", share_mem_node_);
    }

    /**
     * @name: ~ShareMem
     * @brief: 默认析构函数
     * @param {void}
     * @return {*}
     */
    ~ShareMem(void)
    {
        int32_t ret = 0;

        Print("shmdel share mem id ", share_mem_id_, " addr is ", share_mem_node_);
        // 把共享内存从当前进程中分离
        if (nullptr != share_mem_node_) {
            ret = shmdt(share_mem_node_);
            MISC_CHK_STATUS_REPORT(ret, , "shmdt share mem addr ", share_mem_node_, "failed, ret ", ret);
            share_mem_node_ = nullptr;
        }

        ///删除共享内存
        if (share_mem_id_ >= 0) {
            if (true == create_) {
                ret = shmctl(share_mem_id_, IPC_RMID, 0);
                MISC_CHK_STATUS_REPORT(ret, , "rm share mem id ", share_mem_id_, "failed, ret ", ret);
                create_ = false;
            }

            share_mem_id_ = -1;
        }
    }

    ///默认构造函数禁用
    ShareMem(void) = delete;

    ///拷贝构造函数禁用
    ShareMem(const ShareMem &share_mem_mgr) = delete;

    ///赋值构造函数禁用
    ShareMem &operator=(const ShareMem &share_mem_mgr) = delete;

    ///右值拷贝构造函数禁用
    ShareMem(const ShareMem &&share_mem_mgr) = delete;

    ///右值赋值构造函数禁用
    ShareMem &operator=(const ShareMem &&share_mem_mgr) = delete;

    public:
    ///共享内存管理器的key
    const key_t shm_key_;

    ///共享内存管理器的id
    int32_t share_mem_id_;

    ///共享内存地址
    _MemNode *share_mem_node_;

    ///当前共享内存是否create出来的
    bool create_;

    ///Posix 信号量
    ShareSemaphore share_seam_;
};

}  // namespace lemon

#endif
