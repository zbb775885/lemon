/*
 * @Copyright (c) 2014-2020 SenseTime :  All rights reserved.
 * @Description:
 * @Author: 周波
 * @Date: 2021-03-20 12:34:43
 * @LastEditors: 周波
 * @LastEditTime: 2021-03-20 13:10:23
 * @FilePath: \lemon\examples\derive1.hh
 */
#include <base.hh>
#include <cstdint>
#include <iostream>

class Derive1 : public Base
{
    public:
    Derive1();
    Derive1(int32_t a);

    public:
    int32_t a_;
};
