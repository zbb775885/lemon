/*
 * @Copyright (c) 2014-2020 SenseTime :  All rights reserved.
 * @Description:
 * @Author: 周波
 * @Date: 2021-03-20 08:58:42
 * @LastEditors: 周波
 * @LastEditTime: 2021-03-20 13:10:30
 * @FilePath: \lemon\examples\derive2.hh
 */
#include <base.hh>
#include <cstdint>
#include <iostream>

class Derive2 : public Base
{
    public:
    Derive2();
    Derive2(int32_t a);

    public:
    int32_t a_;
};
