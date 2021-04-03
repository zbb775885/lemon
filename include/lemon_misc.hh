/*
 * @Copyright (c) 2014-2020 SenseTime :  All rights reserved.
 * @Description:
 * @Author: 周波
 * @Date: 2021-03-21 22:42:54
 * @LastEditors: 周波
 * @LastEditTime: 2021-04-02 12:48:39
 * @FilePath: /lemon/include/lemon_misc.hh
 */
#ifndef __LEMON_MISC_HH__
#define __LEMON_MISC_HH__

#include <iostream>
#include <vector>

namespace lemon
{
/**
 *@ brief 检查condition，如果不为真则执行需必要的代码及打印，执行完后返回ret，必要代码可在ret后加
 */
#define MISC_CHK(condition, ...) \
    if (!(condition)) {          \
        return __VA_ARGS__;      \
    }

/**
 *@ brief 检查condition，如果不为真则执行需必要的代码及打印，执行完后返回ret，必要代码可在ret后加
 */
#define MISC_CHK_CONDITION(condition, ...) \
    MISC_CHK(condition, ##__VA_ARGS__)

/**
 *@ brief
 *检查ptr是否为空，如果不为真则执行需必要的代码及打印，执行完后返回ret，必要代码可在ret后加
 */
#define MISC_CHK_UN_NULL(ptr, ...) \
    MISC_CHK((NULL != (ptr)), ##__VA_ARGS__)

/**
 *@ brief
 *检查status是否为0，如果不为真则执行需必要的代码及打印，执行完后返回ret，必要代码可在ret后加
 */
#define MISC_CHK_STATUS(val, ...) \
    MISC_CHK((0 == (val)), ##__VA_ARGS__)

/**
 *@ brief 检查condition，如果不为真则执行需必要的代码及打印，执行完后返回ret，必要代码可在ret后加
 */
#define MISC_CHK_EXEC(condition, action, ...) \
    if (!(condition)) {                       \
        action;                               \
        return __VA_ARGS__;                   \
    }

/**
 *@ brief 检查condition，如果不为真则执行需必要的代码及打印，执行完后返回ret，必要代码可在ret后加
 */
#define MISC_CHK_CONDITION_EXEC(condition, action, ...) \
    MISC_CHK_EXEC(condition, action, ##__VA_ARGS__)

/**
 *@ brief
 *检查ptr是否为空，如果不为真则执行需必要的代码及打印，执行完后返回ret，必要代码可在ret后加
 */
#define MISC_CHK_UN_NULL_EXEC(ptr, action, ...) \
    MISC_CHK_EXEC((NULL != (ptr)), action, ##__VA_ARGS__)

/**
 *@ brief
 *检查status是否为0，如果不为真则执行需必要的代码及打印，执行完后返回ret，必要代码可在ret后加
 */
#define MISC_CHK_STATUS_EXEC(val, action, ...) \
    MISC_CHK_EXEC((0 == (val)), action, ##__VA_ARGS__)

/**
 *@ brief 检查condition，如果不为真则执行需必要的代码及打印，执行完后返回ret，必要代码可在ret后加
 */
#define MISC_CHK_REPORT_EXEC(condition, ret, action, ...) \
    if (!(condition)) {                                   \
        Print(__VA_ARGS__);                               \
        action;                                           \
        return ret;                                       \
    }

/**
 *@ brief 检查condition，如果不为真则执行需必要的代码及打印，执行完后返回ret，必要代码可在ret后加
 */
#define MISC_CHK_CONDITION_REPORT_EXEC(condition, ret, action, ...) \
    MISC_CHK_REPORT_EXEC(condition, ret, action, ##__VA_ARGS__)

/**
 *@ brief
 *检查ptr是否为空，如果不为真则执行需必要的代码及打印，执行完后返回ret，必要代码可在ret后加
 */
#define MISC_CHK_UN_NULL_REPORT_EXEC(ptr, ret, action, ...) \
    MISC_CHK_REPORT_EXEC((NULL != (ptr)), ret, action, ##__VA_ARGS__)

/**
 *@ brief
 *检查status是否为0，如果不为真则执行需必要的代码及打印，执行完后返回ret，必要代码可在ret后加
 */
#define MISC_CHK_STATUS_REPORT_EXEC(val, ret, action, ...) \
    MISC_CHK_REPORT_EXEC((0 == (val)), ret, action, ##__VA_ARGS__)

/**
 *@ brief 检查condition，如果不为真则先打印对应的日志信息再返回ret，日志信息可在ret后加
 */
#define MISC_CHK_CONDITION_REPORT(condition, ret, ...) \
    MISC_CHK_REPORT_EXEC(condition, ret, , ##__VA_ARGS__)

/**
 *@ brief 检查ptr是否为空，如果为空则先打印对应的日志信息再返回ret，日志信息可在ret后加
 */
#define MISC_CHK_UN_NULL_REPORT(ptr, ret, ...) \
    MISC_CHK_REPORT_EXEC((NULL != (ptr)), ret, , ##__VA_ARGS__)

/**
 *@ brief 检查status是否为0，如果不是0则先打印对应的日志信息再返回ret，日志信息可在ret后加
 */
#define MISC_CHK_STATUS_REPORT(val, ret, ...) \
    MISC_CHK_REPORT_EXEC((0 == (val)), ret, , ##__VA_ARGS__)

/**
 *@ brief 检查condition，如果不为真则执行需必要的代码及打印
 */
#define MISC_CHK_REPORT_EXEC_NO_RET(condition, action, ...) \
    if (!(condition)) {                                     \
        Print(__VA_ARGS__);                                 \
        action;                                             \
    }

/**
 *@ brief 检查condition，如果不为真则执行需必要的代码及打印，必要代码可在判断条件后加
 */
#define MISC_CHK_CONDITION_REPORT_EXEC_NO_RET(condition, action, ...) \
    MISC_CHK_REPORT_EXEC_NO_RET(condition, action, ##__VA_ARGS__)

/**
 *@ brief 检查ptr是否为空，如果不为真则执行需必要的代码及打印，必要代码可在判断条件后加
 */
#define MISC_CHK_UN_NULL_REPORT_EXEC_NO_RET(ptr, action, ...) \
    MISC_CHK_REPORT_EXEC_NO_RET((NULL != (ptr)), action, ##__VA_ARGS__)

/**
 *@ brief 检查status是否为0，如果不为真则执行需必要的代码及打印，必要代码可在判断条件后加
 */
#define MISC_CHK_STATUS_REPORT_EXEC_NO_RET(val, action, ...) \
    MISC_CHK_REPORT_EXEC_NO_RET((0 == (val)), action, ##__VA_ARGS__)

/**
 *@ brief 检查condition，如果不为真则先打印对应的日志信息再返回ret，日志信息可在判断条件后加
 */
#define MISC_CHK_CONDITION_REPORT_NO_RET(condition, ...) \
    MISC_CHK_REPORT_EXEC_NO_RET(condition, , ##__VA_ARGS__)

/**
 *@ brief 检查ptr是否为空，如果为空则先打印对应的日志信息再返回ret，日志信息可在判断条件后加
 */
#define MISC_CHK_UN_NULL_REPORT_NO_RET(ptr, ...) \
    MISC_CHK_REPORT_EXEC_NO_RET((NULL != (ptr)), , ##__VA_ARGS__)

/**
 *@ brief 检查status是否为0，如果不是0则先打印对应的日志信息再返回ret，日志信息可在判断条件后加
 */
#define MISC_CHK_STATUS_REPORT_NO_RET(val, ...) \
    MISC_CHK_REPORT_EXEC_NO_RET((0 == (val)), , ##__VA_ARGS__)

}  // namespace lemon

#endif
