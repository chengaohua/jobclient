//
// Created by gh on 2021/6/2.
//
#include <iostream>
#include<unistd.h>
#include "config.h"
#include "dispatchProcess/taskManager.h"
#include "masterProcess/masterProcess.h"
#include "dispatchProcess/dispatchProcess.h"
#include "workProcess/workProcess.h"

int main(int argc, char **argv) {

  initDeviceId(10010);
  initGrpcServer(argv[1]);

  auto parent_pid = getpid();
  taskManager::initParentPID(parent_pid);

  run_work();

//  auto pid = fork();
//  if (pid == 0) {
//    //child
//    run_task_dispatch();
//    exit(0);
//  }
//
//  pid = fork();
//  if (pid == 0) {
//    //child
//    run_work();
//    exit(0);
//  }
//
//  initWorkProcess(pid);
//
//  run_master();
  return 0;
}

