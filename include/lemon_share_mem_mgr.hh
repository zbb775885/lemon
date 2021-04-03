/*
 * @Copyright (c) 2014-2020 SenseTime :  All rights reserved.
 * @Description:
 * @Author: 周波
 * @Date: 2021-03-21 22:42:54
 * @LastEditors: 周波
 * @LastEditTime: 2021-04-03 21:41:57
 * @FilePath: \lemon\include\lemon_share_mem_mgr.hh
 */
#ifndef __LEMON_SHARED_MEM_MGR_HH__
#define __LEMON_SHARED_MEM_MGR_HH__

#include <lemon_singleton.hh>
#include <lemon_share_mem.hh>

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
*@ brief 共享内存管理器的信号量默认key
*/
#ifndef SHARED_MEM_SEAM_KEY
#define SHARED_MEM_SEAM_KEY (0x1234)
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
    uint32_t ref_cnt;                   ///当前共享内存的引用计数
} SharedMemNode;

/**
*@ brief 共享内存管理器
*/
class ShareMemMgr
{
    public:
    ///从共享内存管理器中根据名字获取节点，引用计数加1
    SharedMemNode *GetShareMemNode(char *shm_name);

    ///释放节点引用计数减1
    int32_t PutShareMemNode(SharedMemNode *mem_node);

    ///从共享内存管理器中申请一个共享内存id
    SharedMemNode *AllocShareMemNode(char *shm_name, uint32_t size);

    ///从共享内存管理器中申请一个共享内存id
    int32_t FreeShareMemNode(SharedMemNode *mem_node);

    private:
    /**
     * @name: ShareMemMgr
     * @brief: 默认构造函数
     * @param {void}
     * @return {*}
     */
    ShareMemMgr(void);

    /**
     * @name: ~ShareMemMgr
     * @brief: 默认析构函数
     * @param {void}
     * @return {*}
     */
    ~ShareMemMgr(void);

    ///单例友元类
    friend Singleton<ShareMemMgr>;

    private:
    ///mgr的共享节点
    ShareMem<SharedMemNode, SHARED_MEM_MAX_NODE> share_mem_mgr_;
};

}  // namespace lemon

#endif
