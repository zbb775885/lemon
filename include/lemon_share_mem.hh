/*
 * @Copyright (c) 2014-2020 SenseTime :  All rights reserved.
 * @Description:
 * @Author: 周波
 * @Date: 2021-03-21 22:42:54
 * @LastEditors: 周波
 * @LastEditTime: 2021-04-05 16:35:48
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
#include <lemon_unique_lock.hh>

namespace lemon
{
/**
*@ brief 定义空的Base类主要用于存储的统一，没有其他意义
*/
typedef class ShareMemBase
{
    public:
    ShareMemBase() = default;
    virtual ~ShareMemBase() = default;

    public:
    uint32_t shm_idx;  ///节点在mgr中的索引
} ShareMemBase;

/**
*@ brief 共享内存
*/
template <typename _MemNode>
class ShareMem : public ShareMemBase
{
    public:
    /**
     * @name: ShareMem
     * @brief: 默认构造函数
     * @param {void}
     * @return {*}
     */
    ShareMem(int32_t shm_key, int32_t sema_key, uint32_t node_size) throw()
            : ShareMemBase(),
              shm_key_(shm_key),
              share_mem_id_(-1),
              share_mem_node_(nullptr),
              share_seam_(sema_key, 1),
              node_size_(node_size)
    {
        ///相同的shm_key同时只能一个创建
        UniqueLock<ShareSemaphore> unique_lock(share_seam_);
        ///如果共享内存管理没有初始化那么先完成初始化
        share_mem_id_ = shmget(shm_key_, sizeof(_MemNode) * node_size_, 0666 | IPC_CREAT);
        MISC_CHK_CONDITION_REPORT_EXEC(share_mem_id_ >= 0, , throw "shmget fail",
                                       "shmget share mem key ", shm_key_, "failed");

        //将共享内存连接到当前进程的地址空间
        share_mem_node_ = static_cast<_MemNode *>(shmat(share_mem_id_, nullptr, 0));
        MISC_CHK_UN_NULL_REPORT_EXEC(share_mem_node_, , throw "shmat fail",
                                     "shmat share mem id ", share_mem_id_, " to virt addr failed");

        ///创建共享内存的需要清零一下
        struct shmid_ds shm_ds;
        int32_t ret = shmctl(share_mem_id_, IPC_STAT, &shm_ds);
        MISC_CHK_CONDITION_REPORT_EXEC(ret >= 0, , throw "shmctl stat fail",
                                       "shmctl stat share mem key ", shm_key_, "failed");
        ///只有一个attach说明当前是创建了节点，需要清除一下共享内存
        if (1 == shm_ds.shm_nattch) {
            MEMSET(share_mem_node_, 0, sizeof(_MemNode) * node_size_);
        }

        Print("shmget share mem key ", shm_key_, " addr is ", share_mem_node_);
    }

    /**
     * @name: ~ShareMem
     * @brief: 默认析构函数
     * @param {void}
     * @return {*}
     */
    virtual ~ShareMem(void)
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
            ///创建共享内存的需要清零一下
            struct shmid_ds shm_ds;
            int32_t ret = shmctl(share_mem_id_, IPC_STAT, &shm_ds);
            MISC_CHK_CONDITION_REPORT_EXEC(ret >= 0, , "shmctl stat share mem key ", shm_key_, "failed");
            ///attach个数为0说明当前无人引用可以删除共享内存了
            if (0 == shm_ds.shm_nattch) {
                ret = shmctl(share_mem_id_, IPC_RMID, 0);
                MISC_CHK_STATUS_REPORT(ret, , "rm share mem id ", share_mem_id_, "failed, ret ", ret);
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
    ///共享内存器的key
    const key_t shm_key_;

    ///共享内存器的id
    int32_t share_mem_id_;

    ///共享内存地址
    _MemNode *share_mem_node_;

    ///Posix 信号量
    ShareSemaphore share_seam_;

    ///_MemNode大小的个数
    uint32_t node_size_;
};

}  // namespace lemon

#endif
