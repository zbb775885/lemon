/*
 * @Copyright (c) 2014-2020 SenseTime :  All rights reserved.
 * @Description:
 * @Author: 周波
 * @Date: 2021-03-21 22:42:54
 * @LastEditors: 周波
 * @LastEditTime: 2021-04-02 14:16:16
 * @FilePath: /lemon/include/lemon_share_mem.hh
 */
#ifndef __LEMON_SHARED_MEM_HH__
#define __LEMON_SHARED_MEM_HH__

#include <stdint.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <lemon_singleton.hh>
#include <lemon_log.hh>
#include <lemon_misc.hh>

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
#define SHARED_MEM_DEFAULT_KEY (0x1234)
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
              share_mem_mgr_id_(-1),
              shm_virt_addr_(nullptr),
              create_(false)
    {
        ///TODO:构造的时候可能需要加个锁
        share_mem_mgr_id_ = shmget(shm_mgr_key_, sizeof(SharedMemNode) * SHARED_MEM_MAX_NODE, 0666);
        if (share_mem_mgr_id_ < 0) {
            ///如果共享内存管理没有初始化那么先完成初始化
            share_mem_mgr_id_ = shmget(shm_mgr_key_, sizeof(SharedMemNode) * SHARED_MEM_MAX_NODE, 0666 | IPC_CREAT);
            MISC_CHK_CONDITION_REPORT(share_mem_mgr_id_ > 0, , "shmget share mem mgr key ", shm_mgr_key_, "failed");
            create_ = true;
        }

        //将共享内存连接到当前进程的地址空间
        shm_virt_addr_ = shmat(share_mem_mgr_id_, nullptr, 0);
        MISC_CHK_UN_NULL_REPORT(shm_virt_addr_, , "shmat share mem id ", share_mem_mgr_id_, " to virt addr failed");

        Print("shmget share mem mgr key ", shm_mgr_key_, " addr is ", shm_virt_addr_);
    }

    ///默认析构函数
    ~ShareMemMgr(void)
    {
        int32_t ret = 0;

        Print("shmdel share mem mgr id ", share_mem_mgr_id_, " addr is ", shm_virt_addr_);

        // 把共享内存从当前进程中分离
        if (nullptr != shm_virt_addr_) {
            ret = shmdt(shm_virt_addr_);
            MISC_CHK_STATUS_REPORT(ret, , "shmdt share mem mgr addr ", shm_virt_addr_, "failed, ret ", ret);
            shm_virt_addr_ = nullptr;
        }

        ///删除共享内存
        if (share_mem_mgr_id_ > 0) {
            if (true == create_) {
                ret = shmctl(share_mem_mgr_id_, IPC_RMID, 0);
                MISC_CHK_STATUS_REPORT(ret, , "rm share mem mgr id ", share_mem_mgr_id_, "failed, ret ", ret);
                create_ = false;
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

    ///共享内存地址
    void *shm_virt_addr_;

    ///当前共享内存是否create出来的
    bool create_;
};

}  // namespace lemon

#endif
