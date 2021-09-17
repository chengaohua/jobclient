//
// Created by gh on 2021/6/2.
//
#include <iostream>
#include<unistd.h>
#include "task/taskManager.h"
#include "process/masterProcess.h"
#include "process/taskProcess.h"

int main(int argc, char **argv) {

  auto parent_pid = getpid();
  taskManager::initParentPID(parent_pid);

  auto pid = fork();
  if(pid == 0 ){
    //child
    run_task_dispatch();
    exit(0);
  }

  run_master();
  return 0;
}

