//
// Created by gh on 2021/9/16.
//

#include <iostream>
#include "taskManager.h"


#define INVALIDTASKID  -1

namespace taskManager {
static int current_task = INVALIDTASKID;
static pid_t parent_pid = 0;

void initParentPID(pid_t pid) {
  parent_pid = pid;
}

void createTask(int taskid) {
  if(taskid == current_task) {
    std::cout<<" current_task == taskid"<<std::endl;
    return ;
  }

  std::cout<<"----------------------->"<<taskid<<std::endl;
  current_task = taskid;

  int signum;
  union sigval mysigval;
  signum= SIGUSR1;
  mysigval.sival_int=taskid;
  //不代表具体含义，只用于说明问题

  if(sigqueue(parent_pid,signum,mysigval)==-1)
    printf("send error＼n");
}

void cancelTask(int taskid) {
  if(current_task != taskid || current_task == INVALIDTASKID) {
    return ;
  }

  int signum;
  union sigval mysigval;
  signum= SIGUSR2;
  mysigval.sival_int=taskid;
  //不代表具体含义，只用于说明问题

  if(sigqueue(parent_pid,signum,mysigval)==-1)
    printf("send error＼n");

  current_task = INVALIDTASKID;
}

}