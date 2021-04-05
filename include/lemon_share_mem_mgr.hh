/*
 * @Copyright (c) 2014-2020 SenseTime :  All rights reserved.
 * @Description:
 * @Author: 周波
 * @Date: 2021-03-21 22:42:54
 * @LastEditors: 周波
 * @LastEditTime: 2021-04-05 16:13:39
 * @FilePath: \lemon\include\lemon_share_mem_mgr.hh
 */
#ifndef __LEMON_SHARED_MEM_MGR_HH__
#define __LEMON_SHARED_MEM_MGR_HH__

#include <unordered_map>
#include <mutex>
#include <memory>
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
*@ brief 根据输入的索引共享内存块的信号量key生成
*/
#define ALLOC_SHARE_MEM_KEY(idx) (SHARED_MEM_DEFAULT_KEY + 1 + idx)

/**
*@ brief 根据输入的索引共享内存块的shm生成
*/
#define ALLOC_SHARE_MEM_SEAM_KEY(idx) (SHARED_MEM_SEAM_KEY + 1 + idx)

/**
*@ brief 共享内存的pair模板
*/
template <typename _Type>
using ShareMemPair = std::pair<_Type *, ShareSemaphore *>;

/**
*@ brief 共享内存的pair智能指针模板
*/
template <typename _Type>
using SpShareMemPair = std::shared_ptr<ShareMemPair<_Type>>;

/**
*@ brief 共享内存管理器
*/
class ShareMemMgr
{
    public:
    /**
    *@ brief 共享内存记录的每个节点的必要信息
    */
    typedef struct SharedMemNode {
        int32_t shm_key;                    ///当前共享内存的key
        int32_t sema_key;                   ///当前共享内存的同步信号量
        char shm_name[SHARE_MEM_NAME_LEN];  ///共享内存名
        uint32_t shm_size;                  ///共享内存的大小
        uint32_t ref_cnt;                   ///当前共享内存节点的引用计数
    } SharedMemNode;

    public:
    ///从共享内存管理器中申请一个共享内存id
    template <typename _Type>
    const SpShareMemPair<_Type> AllocShareMemNode(const char *shm_name, uint32_t type_cnt = 1)
    {
        MISC_CHK_UN_NULL_REPORT(shm_name, nullptr, "shm_name is null");

        ///TODO:锁的范围再考虑一下
        SharedMemNode *shm_node = nullptr;
        _Type *type_addr = nullptr;
        ShareSemaphore *share_sema = nullptr;
        ///先查询一下当前节点名字是由在本地的map中，有的话可以直接获取到idx
        {
            std::unique_lock<std::mutex> map_lock(node_map_mutex_);
            auto &&iter = node_name_map_.find(shm_name);
            if (node_name_map_.end() != iter) {
                auto &sp_mem_base = iter->second;  ///mem_info类型为std::shared_ptr<ShareMemBase>
                type_addr = (std::static_pointer_cast<ShareMem<_Type>>(sp_mem_base))->share_mem_node_;
                share_sema = &(std::static_pointer_cast<ShareMem<_Type>>(sp_mem_base))->share_seam_;
                shm_node = &share_mem_mgr_.share_mem_node_[sp_mem_base->shm_idx];
                Print("shm idx is ", sp_mem_base->shm_idx, "type_addr is ", type_addr);
            } else {
                MISC_CHK_CONDITION_REPORT(node_name_map_.size() < SHARED_MEM_MAX_NODE, nullptr,
                                          "no empty shm node to alloc");
            }
        }
        ///map中找不到的话那么就需要遍历一下所有的节点观察是否已经申请过该节点
        UniqueLock<ShareSemaphore> unique_lock(share_mem_mgr_.share_seam_);
        if (nullptr == shm_node) {
            SharedMemNode *empty_node = nullptr;
            uint32_t shm_idx = 0;
            for (uint32_t i = 0; i < SHARED_MEM_MAX_NODE; i++) {
                //Print("idx is", i, "ref cnt is ", share_mem_mgr_.share_mem_node_[i].ref_cnt);
                if (share_mem_mgr_.share_mem_node_[i].ref_cnt > 0) {
                    if (0 == strcmp(share_mem_mgr_.share_mem_node_[i].shm_name, shm_name)) {
                        shm_node = &share_mem_mgr_.share_mem_node_[i];
                        shm_idx = i;
                        break;
                    }
                } else {
                    empty_node = &share_mem_mgr_.share_mem_node_[i];
                    shm_idx = i;
                }
            }

            Print("shm_node IS ", shm_node, "empty_node is ", empty_node);
            ///如果shm_node已经找到了那么由外部做引用计数增加调整，否则看下是否有空的节点，有的话申请节点
            {

                ///根据索引idx生成shm和sema的key,再new一个对象
                ShareMemBase *mem_base = nullptr;
                try {
                    mem_base = new ShareMem<_Type>(ALLOC_SHARE_MEM_KEY(shm_idx),
                                                   ALLOC_SHARE_MEM_SEAM_KEY(shm_idx),
                                                   type_cnt);
                } catch (const char *error) {
                    Print("error is ", error);
                }

                MISC_CHK_UN_NULL_REPORT(mem_base, nullptr, "alloc is null");
                {
                    ///存入map表
                    std::shared_ptr<ShareMemBase> sp_mem_base = std::shared_ptr<ShareMemBase>(mem_base);
                    type_addr = (static_cast<ShareMem<_Type> *>(mem_base))->share_mem_node_;
                    share_sema = &(static_cast<ShareMem<_Type> *>(mem_base))->share_seam_;
                    sp_mem_base->shm_idx = shm_idx;
                    std::unique_lock<std::mutex> map_lock(node_map_mutex_);
                    node_name_map_[shm_name] = sp_mem_base;
                    node_addr_map_[type_addr] = sp_mem_base;
                }

                if ((nullptr == shm_node) && (nullptr != empty_node)) {
                    ///填写节点信息
                    empty_node->shm_key = ALLOC_SHARE_MEM_KEY(shm_idx);
                    empty_node->sema_key = ALLOC_SHARE_MEM_SEAM_KEY(shm_idx);
                    snprintf(empty_node->shm_name, SHARE_MEM_NAME_LEN, "%s", shm_name);
                    empty_node->shm_size = sizeof(_Type) * type_cnt;
                    empty_node->ref_cnt = 0;  ///引用计数后续会增加，这里就不加了

                    ///可用节点附上，便于后面操作
                    shm_node = empty_node;
                }
            }
        }

        SpShareMemPair<_Type> sp_pair_data;
        if (nullptr != shm_node) {
            shm_node->ref_cnt++;
            sp_pair_data = SpShareMemPair<_Type>(new ShareMemPair<_Type>(type_addr, share_sema),
                                                 [&](ShareMemPair<_Type> *share_addr_pair) {
                                                     auto share_addr = share_addr_pair->first;
                                                     delete share_addr_pair;
                                                     return FreeShareMemNode(share_addr);
                                                 });
        }

        return sp_pair_data;
    }

    private:
    ///释放一个节点回到共享内存管理器
    int32_t FreeShareMemNode(void *share_addr)
    {
        MISC_CHK_UN_NULL_REPORT(share_addr, -1, "delete share addr is null");
        MISC_CHK_CONDITION_REPORT(true == Singleton<ShareMemMgr>::GetInstanceStatus(), -1,
                                  "share mem mgr is not exist");
        SharedMemNode *shm_node = nullptr;
        {
            ///查找一下整个地址是否在map中，不在的话返回失败
            std::unique_lock<std::mutex> map_lock(node_map_mutex_);
            auto &&iter = node_addr_map_.find(share_addr);
            if (node_addr_map_.end() != iter) {
                auto &sp_mem_base = iter->second;  ///mem_info类型为std::shared_ptr<ShareMemBase>
                shm_node = &share_mem_mgr_.share_mem_node_[sp_mem_base->shm_idx];
            } else {
                Print("share addr ", share_addr, "is not exist");
                return -1;
            }
        }

        {
            UniqueLock<ShareSemaphore> unique_lock(share_mem_mgr_.share_seam_);
            shm_node->ref_cnt--;
            Print("shm_node->ref_cnt ", shm_node->ref_cnt);
            ///引用计数为0说明需要销毁当前的共享内存，否则不做任何处理
            if (0 == shm_node->ref_cnt) {
                {
                    ///删除map中的信息
                    std::unique_lock<std::mutex> map_lock(node_map_mutex_);
                    node_addr_map_.erase(share_addr);
                    node_name_map_.erase(shm_node->shm_name);
                    Print("delete node addr ", share_addr);
                }

                ///删除节点信息
                MEMSET(shm_node, 0, sizeof(*shm_node));
            }
        }

        return 0;
    }

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

    private:
    ///单例友元类
    friend Singleton<ShareMemMgr>;

    ///mgr的共享节点
    ShareMem<SharedMemNode> share_mem_mgr_;

    ///mgr共享节点映射表,根据节点名字映射到共享内存信息，主要用于alloc的时候
    std::unordered_map<std::string, std::shared_ptr<ShareMemBase>> node_name_map_;

    ///mgr共享节点映射表,根据节点地址映射到共享内存信息，主要用于delete的时候
    std::unordered_map<void *, std::shared_ptr<ShareMemBase>> node_addr_map_;

    ///节点名字映射表的互斥锁
    std::mutex node_map_mutex_;
};

}  // namespace lemon

#endif
