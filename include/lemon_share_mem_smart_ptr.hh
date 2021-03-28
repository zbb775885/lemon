/*
 * @Copyright (c) 2014-2020 SenseTime :  All rights reserved.
 * @Description:
 * @Author: 周波
 * @Date: 2021-03-21 22:42:54
 * @LastEditors: 周波
 * @LastEditTime: 2021-03-27 21:31:07
 * @FilePath: \lemon\include\lemon_share_mem_smart_ptr.hh
 */
#ifndef __LEMON_SHARED_MEM_SMART_PTR_HH__
#define __LEMON_SHARED_MEM_SMART_PTR_HH__

#include <stdint.h>
#include <semaphore.h>

namespace lemon
{
template <typename T>
class ShareMemSmartPtr
{
    public:
    ShareMemSmartPtr(T *ptr)
    {
        share_mem_id_ = 0;
    };

    private:
    ///获取管理共享内存的共享内存id
    static int32_t GetShareMemMgr(void)
    {
        static int32_t share_mem_mgr_id = -1;
        ///如果共享内存管理
        if (-1 == share_mem_mgr_id) {
        }
    }

    static int32_t private:
};

}  // namespace lemon

#endif
