/*
 * @Copyright (c) 2014-2020 SenseTime :  All rights reserved.
 * @Description:
 * @Author: 周波
 * @Date: 2021-03-20 08:58:42
 * @LastEditors: 周波
 * @LastEditTime: 2021-03-28 22:42:08
 * @FilePath: \lemon\examples\share_mem\main.cc
 */
#include <lemon_share_mem.hh>
#include <cstdint>

using namespace lemon;

int32_t main(int32_t argc, char *arg[])
{
    ShareMemMgr *share_mem_mgr = Singleton<ShareMemMgr>::GetInstance();
    Print("share_mem_mgr addr is ", share_mem_mgr);
    Singleton<ShareMemMgr>::DestroyInstance();

    ShareMemMgr *share_mem_mgr1 = Singleton<ShareMemMgr>::GetInstance();
    Print("share_mem_mgr1 addr is ", share_mem_mgr1);

    return 0;
}