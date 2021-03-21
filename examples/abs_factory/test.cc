/*
 * @Copyright (c) 2014-2020 SenseTime :  All rights reserved.
 * @Description:
 * @Author: 周波
 * @Date: 2021-03-20 08:58:42
 * @LastEditors: 周波
 * @LastEditTime: 2021-03-20 13:45:00
 * @FilePath: \lemon\examples\test.cc
 */
#include <base.hh>
#include <cstdint>
#include <iostream>
#include <derive1.hh>
#include <derive2.hh>
#include <dlfcn.h>

// class Base
// {
// };

// class Derive : public Base
// {
//     public:
//     Derive() = default;
//     Derive(int32_t a)
//             : a_(a){

//               };

//     public:
//     int32_t a_;
// };
extern "C" {
int32_t tt()
{
    // lemon::AbsFactory<Base>::Register<Derive>("aaa");
    // Base *base = lemon::AbsFactory<Base>::Create("aaa");
    // Derive *derive = static_cast<Derive *>(base);
    // std::cout << typeid(base).name() << std::endl;
    // std::cout << derive->a_ << " ddda  " << std::endl;

    // lemon::AbsFactory<Base, int32_t>::Register<Derive>(1);
    // Base *base1 = lemon::AbsFactory<Base, int32_t>::Create(1);
    // Derive *derive1 = static_cast<Derive *>(base1);
    // std::cout << typeid(base1).name() << std::endl;
    // std::cout << derive1->a_ << " aaa  " << std::endl;

    // lemon::AbsFactory<Base, uint32_t, int32_t>::Register<Derive>(1);
    // Base *base2 = lemon::AbsFactory<Base, uint32_t, int32_t>::Create(1, 100);
    // Derive *derive2 = static_cast<Derive *>(base2);
    // std::cout << typeid(base2).name() << std::endl;
    // std::cout << derive2->a_ << " bbb  " << std::endl;

    std::cout << " uuuuuuuuuuuu " << std::endl;
    Base *base = lemon::AbsFactory<Base, std::string, int32_t>::Create("bbb", 1);
    Derive1 *derive1 = static_cast<Derive1 *>(base);
    std::cout << derive1->a_ << " dddaS  " << std::endl;

    return 0;
}
}