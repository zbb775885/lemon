/*
 * @Copyright (c) 2014-2020 SenseTime :  All rights reserved.
 * @Description:
 * @Author: 周波
 * @Date: 2021-03-21 22:42:54
 * @LastEditors: 周波
 * @LastEditTime: 2021-04-03 21:21:37
 * @FilePath: \lemon\src\lemon_share_mem_mgr.cc
 */
#include <lemon_share_mem_mgr.hh>

namespace lemon
{

///从共享内存管理器中根据名字获取节点，引用计数加1
SharedMemNode *ShareMemMgr::GetShareMemNode(char *shm_name)
{
    MISC_CHK_UN_NULL_REPORT(shm_name, nullptr, "shm_name is null");

    ///查询共享内存节点是否在mgr中
    // UniqueLock<ShareSemaphore> unique_lock(share_seam_);
    // for (uint32_t i = 0; i < SHARED_MEM_MAX_NODE; i++) {
    //     if ((share_mem_node_[i].ref_cnt > 0) &&
    //         (0 == strcmp(share_mem_node_[i].shm_name, shm_name))) {
    //         share_mem_node_[i].ref_cnt++;
    //         return &share_mem_node_[i];
    //     }
    // }

    // Print("can't find shm name ", shm_name);
    return nullptr;
}

int32_t ShareMemMgr::PutShareMemNode(SharedMemNode *mem_node)
{
    MISC_CHK_UN_NULL_REPORT(mem_node, -1, "mem_node is null");

    ///查询共享内存节点是否在mgr中
    // UniqueLock<ShareSemaphore> unique_lock(share_seam_);
    // mem_node->ref_cnt--;
    return 0;
}

///从共享内存管理器中申请一个共享内存id
SharedMemNode *ShareMemMgr::AllocShareMemNode(char *shm_name, uint32_t size)
{
    MISC_CHK_UN_NULL_REPORT(shm_name, nullptr, "shm_name is null");

    // UniqueLock<ShareSemaphore> unique_lock(share_seam_);
    // SharedMemNode *empty_node = nullptr;
    // for (uint32_t i = 0; i < SHARED_MEM_MAX_NODE; i++) {
    //     if (share_mem_node_[i].ref_cnt > 0) {
    //         if (0 == strcmp(share_mem_node_[i].shm_name, shm_name)) {
    //             Print(shm_name, "has alloced in mgr");
    //             return nullptr;
    //         }
    //     } else {
    //         empty_node = &share_mem_node_[i];
    //     }
    // }

    return nullptr;
}

///从共享内存管理器中申请一个共享内存id
int32_t ShareMemMgr::FreeShareMemNode(SharedMemNode *mem_node)
{
    MISC_CHK_UN_NULL_REPORT(mem_node, -1, "mem_node is null");

    // UniqueLock<ShareSemaphore> unique_lock(share_seam_);
    // mem_node->ref_cnt = false;

    return 0;
}

/**
 * @name: ShareMemMgr
 * @brief: 默认构造函数
 * @param {void}
 * @return {*}
 */
ShareMemMgr::ShareMemMgr(void)
        : share_mem_mgr_(SHARED_MEM_DEFAULT_KEY, SHARED_MEM_SEAM_KEY)
{
    //Print("shmget share mem mgr key ", share_mem_mgr_.shm_key_, " addr is ", share_mem_mgr_.share_mem_node_);
}

/**
 * @name: ~ShareMemMgr
 * @brief: 默认析构函数
 * @param {void}
 * @return {*}
 */
ShareMemMgr::~ShareMemMgr(void)
{
    //Print("shmdel share mem mgr id ", share_mem_id_, " addr is ", share_mem_node_);
}

}  // namespace lemon
