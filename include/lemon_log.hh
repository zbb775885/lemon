/*
 * @Copyright (c) 2014-2020 SenseTime :  All rights reserved.
 * @Description:
 * @Author: 周波
 * @Date: 2021-03-21 22:42:54
 * @LastEditors: 周波
 * @LastEditTime: 2021-04-02 12:47:27
 * @FilePath: /lemon/include/lemon_misc.hh
 */
#ifndef __LEMON_LOG_HH__
#define __LEMON_LOG_HH__

#include <iostream>
#include <vector>

namespace lemon
{
template <typename T>
void Print(const T &t)
{
    std::cout << t << " " << std::endl;
}

template <typename T>
void Print(const std::vector<T> &vector)
{
    Print("vector begin:");
    for (auto &iter : vector) {
        Print(iter);
    }
    Print("vector end");
}

template <typename T, typename... Args>
void Print(const T &t, Args... args)
{
    std::cout << t << " ";
    Print(args...);
}

}  // namespace lemon

#endif
