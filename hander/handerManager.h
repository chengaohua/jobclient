//
// Created by gh on 2021/9/16.
//

#ifndef TEST_HANDER_HANDERMANAGER_H_
#define TEST_HANDER_HANDERMANAGER_H_

#include<signal.h>
#include<unistd.h>
#include <wait.h>

namespace handerManager {

void initWorkProcess(pid_t pid);



//子进程推出信号
void sig_child(int signo);

void sig_sigusr1_from_child(int signum,siginfo_t *info,void *myact);

void sig_siguser2_from_child(int signum,siginfo_t *info,void *myact);



}

#endif //TEST_HANDER_HANDERMANAGER_H_
