/*
 * @Copyright (c) 2014-2020 SenseTime :  All rights reserved.
 * @Description:
 * @Author: 周波
 * @Date: 2021-03-20 08:58:42
 * @LastEditors: 周波
 * @LastEditTime: 2021-04-05 22:43:29
 * @FilePath: \lemon\examples\share_mem\main.cc
 */
#include <lemon_share_mem_mgr.hh>
#include <cstdint>

using namespace lemon;

int32_t main(int32_t argc, char *arg[])
{
    ShareMemMgr *share_mem_mgr = Singleton<ShareMemMgr>::GetInstance();
    Print("share_mem_mgr addr is ", share_mem_mgr);

    {
        auto &sp_share_mem = share_mem_mgr->AllocShareMemNode<uint32_t>("aaa");
        auto &sp_share_mem1 = share_mem_mgr->AllocShareMemNode<uint32_t>("bbb");
        //Print("share addr is ", sp_share_mem->first, "value is ", *sp_share_mem->first);
        // getchar();
        *sp_share_mem1 = 100;
        while (1) {
            *sp_share_mem = *sp_share_mem + *sp_share_mem1;
            Print("share addr is ", sp_share_mem->first, "value is ", *sp_share_mem->first);
        }

        getchar();
        Print("share addr is ", sp_share_mem->first, "value is ", *sp_share_mem->first);
    }

    Singleton<ShareMemMgr>::DestroyInstance();

    Print("-------------------------------");

    // ShareMemMgr *share_mem_mgr1 = Singleton<ShareMemMgr>::GetInstance();
    // Print("share_mem_mgr1 addr is ", share_mem_mgr1);

    // getchar();
    // Singleton<ShareMemMgr>::DestroyInstance();

    return 0;
}
