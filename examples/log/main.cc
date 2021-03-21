/*
 * @Copyright (c) 2014-2020 SenseTime :  All rights reserved.
 * @Description:
 * @Author: 周波
 * @Date: 2021-03-20 08:58:42
 * @LastEditors: 周波
 * @LastEditTime: 2021-03-21 23:09:18
 * @FilePath: \lemon\examples\log\main.cc
 */
#include <lemon_log.hh>
#include <cstdint>

int32_t main()
{
    double value = 0.9;

    lemon::Print(value);

    lemon::Print("he's name is ", value);

    std::vector<std::string> vector_str = { "123", "456" };

    lemon::Print(vector_str);

    std::vector<int32_t> vector_int = { 1, 3 };

    lemon::Print(vector_int);

    return 0;
}