/*
 * @Copyright (c) 2014-2020 SenseTime :  All rights reserved.
 * @Description:
 * @Author: 周波
 * @Date: 2021-03-20 08:58:42
 * @LastEditors: 周波
 * @LastEditTime: 2021-03-20 13:13:30
 * @FilePath: \lemon\examples\Derive2.cc
 */
#include <derive2.hh>
#include <cstdint>
#include <iostream>

Derive2::Derive2()
{
}

Derive2::Derive2(int32_t a)
            : a_(a)
{
}

static bool set_derived_2 = lemon::AbsFactory<Base, std::string, int32_t>::Register<Derive2>("bbb");