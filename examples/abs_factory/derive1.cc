/*
 * @Copyright (c) 2014-2020 SenseTime :  All rights reserved.
 * @Description:
 * @Author: 周波
 * @Date: 2021-03-20 08:58:42
 * @LastEditors: 周波
 * @LastEditTime: 2021-03-20 13:11:44
 * @FilePath: \lemon\examples\Derive1.cc
 */
#include <derive1.hh>
#include <cstdint>
#include <iostream>

Derive1::Derive1()
{
}

Derive1::Derive1(int32_t a)
            : a_(a)
{
}


static bool set_derived_1 = lemon::AbsFactory<Base, std::string, int32_t>::Register<Derive1>("aaa");
