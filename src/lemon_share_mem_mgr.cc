/*
 * @Copyright (c) 2014-2020 SenseTime :  All rights reserved.
 * @Description:
 * @Author: 周波
 * @Date: 2021-03-21 22:42:54
 * @LastEditors: 周波
 * @LastEditTime: 2021-04-04 12:10:51
 * @FilePath: \lemon\src\lemon_share_mem_mgr.cc
 */
#include <lemon_share_mem_mgr.hh>

namespace lemon
{
/**
 * @name: ShareMemMgr
 * @brief: 默认构造函数
 * @param {void}
 * @return {*}
 */
ShareMemMgr::ShareMemMgr(void)
        : share_mem_mgr_(SHARED_MEM_DEFAULT_KEY, SHARED_MEM_SEAM_KEY, SHARED_MEM_MAX_NODE)
{
    Print("shmget share mem mgr key ", share_mem_mgr_.shm_key_,
          " addr is ", share_mem_mgr_.share_mem_node_);
}

/**
 * @name: ~ShareMemMgr
 * @brief: 默认析构函数
 * @param {void}
 * @return {*}
 */
ShareMemMgr::~ShareMemMgr(void)
{
    Print("shmdel share mem mgr id ", share_mem_mgr_.share_mem_id_,
          " addr is ", share_mem_mgr_.share_mem_node_);
}

}  // namespace lemon
