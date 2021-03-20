/*
 * @Copyright (c) 2014-2020 SenseTime :  All rights reserved.
 * @Description:
 * @Author: 周波
 * @Date: 2021-03-20 08:58:42
 * @LastEditors: 周波
 * @LastEditTime: 2021-03-20 11:46:03
 * @FilePath: \lemon\examples\main.cc
 */
#include <lemon_abs_factory.hh>
#include <cstdint>
#include <iostream>

class Base
{
};

class Derive : public Base
{
    public:
    Derive() = default;
    Derive(int32_t a)
            : a_(a){

              };

    public:
    int32_t a_;
};

int32_t main()
{
    lemon::AbsFactory<Base>::Register<Derive>("aaa");
    Base *base = lemon::AbsFactory<Base>::Create("aaa");
    Derive *derive = static_cast<Derive *>(base);
    std::cout << typeid(base).name() << std::endl;
    std::cout << derive->a_ << " ddd  " << std::endl;

    lemon::AbsFactory<Base, int32_t>::Register<Derive>(1);
    Base *base1 = lemon::AbsFactory<Base, int32_t>::Create(1);
    Derive *derive1 = static_cast<Derive *>(base1);
    std::cout << typeid(base1).name() << std::endl;
    std::cout << derive1->a_ << " aaa  " << std::endl;

    lemon::AbsFactory<Base, uint32_t, int32_t>::Register<Derive>(1);
    Base *base2 = lemon::AbsFactory<Base, uint32_t, int32_t>::Create(1, 3);
    Derive *derive2 = static_cast<Derive *>(base2);
    std::cout << typeid(base2).name() << std::endl;
    std::cout << derive2->a_ << " bbb  " << std::endl;

    return 0;
}