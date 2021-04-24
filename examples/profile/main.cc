/*
 * @Copyright (c) 2014-2020 SenseTime :  All rights reserved.
 * @Description:
 * @Author: 周波
 * @Date: 2021-03-20 08:58:42
 * @LastEditors: 周波
 * @LastEditTime: 2021-03-28 22:38:38
 * @FilePath: \lemon\examples\log\main.cc
 */
#include <lemon_log.hh>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
void prompt_info(int signo)
{
    printf("can i be called!!!\n");
    //system("./test");
}
void init_sigaction(void)
{
    struct sigaction act;
    act.sa_handler = prompt_info;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGPROF, &act, NULL);
}
void init_time()
{
    struct itimerval value;
    value.it_value.tv_sec = 0;
    value.it_value.tv_usec = 100;
    value.it_interval = value.it_value;
    setitimer(ITIMER_PROF, &value, NULL);
}
int main()
{
    init_sigaction();
    init_time();
    while (1) {
    }
    exit(0);
}

// while (1) {
//         usleep(1000000);
//         printf("dddddddd\n");
//     }