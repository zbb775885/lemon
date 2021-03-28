/*
 * @Copyright (c) 2014-2020 SenseTime :  All rights reserved.
 * @Description:
 * @Author: 周波
 * @Date: 2021-03-21 22:42:54
 * @LastEditors: 周波
 * @LastEditTime: 2021-03-28 22:37:35
 * @FilePath: \lemon\include\lemon_share_mem.hh
 */
#ifndef __LEMON_SHARED_MEM_HH__
#define __LEMON_SHARED_MEM_HH__

#include <stdint.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <lemon_singleton.hh>
#include <lemon_log.hh>

namespace lemon
{
/**
*@ brief 共享内存管理的最大共享内存块数
*/
#ifndef SHARED_MEM_MAX_NODE
#define SHARED_MEM_MAX_NODE (100)
#endif

/**
*@ brief 共享内存管理器的默认key
*/
#ifndef SHARED_MEM_DEFAULT_KEY
#define SHARED_MEM_DEFAULT_KEY (0X1234)
#endif

/**
*@ brief 共享内存块的名字长度
*/
#define SHARE_MEM_NAME_LEN (16)

/**
*@ brief 共享内存元素
*/
typedef struct SharedMemNode {
    key_t shm_key;                      ///当前共享内存的key
    char shm_name[SHARE_MEM_NAME_LEN];  ///共享内存名
    uint32_t shm_size;                  ///共享内存的大小
    bool in_use;                        ///当前共享内存是否在使用
} SharedMemNode;

/**
*@ brief 共享内存
*/
class ShareMem
{
    public:
    ///不支持默认构造共享内存
    ShareMem(void) = delete;

    ///不支持默认构造共享内存
    ShareMem(char *shm_name, uint32_t size){

    };

    private:
};

/**
*@ brief 共享内存管理器
*/
class ShareMemMgr
{
    public:
    ///从共享内存管理器中申请一个共享内存id
    // SharedMemNode *GetShareMemNode(char *shm_name, uint32_t size)
    // {
    //     void *shm = nullptr;

    //     shm = shmat(share_mem_mgr_id_, 0, 0);

    //     return shm;
    // }

    private:
    ///默认构造函数
    ShareMemMgr(void)
            : shm_mgr_key_(SHARED_MEM_DEFAULT_KEY),
              share_mem_mgr_id_(-1)
    {
        ///如果共享内存管理没有初始化那么先完成初始化
        share_mem_mgr_id_ = shmget(shm_mgr_key_, sizeof(SharedMemNode) * SHARED_MEM_MAX_NODE, 0666 | IPC_CREAT);
    }

    ///默认析构函数
    ~ShareMemMgr(void)
    {
        int32_t ret = 0;
        if (share_mem_mgr_id_ > 0) {
            ret = shmctl(share_mem_mgr_id_, IPC_RMID, 0);
            if (ret < 0) {
                Print("rm share mem mgr id ", share_mem_mgr_id_, "failed, ret ", ret);
            }

            share_mem_mgr_id_ = -1;
        }
    }

    ///拷贝构造函数禁用
    ShareMemMgr(const ShareMemMgr &singleton) = delete;

    ///赋值构造函数禁用
    ShareMemMgr &operator=(const ShareMemMgr &singleton) = delete;

    ///右值拷贝构造函数禁用
    ShareMemMgr(const ShareMemMgr &&singleton) = delete;

    ///右值赋值构造函数禁用
    ShareMemMgr &operator=(const ShareMemMgr &&singleton) = delete;

    ///单例友元类
    friend Singleton<ShareMemMgr>;

    private:
    ///共享内存管理器的key
    const key_t shm_mgr_key_;

    ///共享内存管理器的id
    int32_t share_mem_mgr_id_;
};

}  // namespace lemon

#endif
