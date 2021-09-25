//
// Created by gh on 2021/6/2.
//
#include <iostream>
#include<unistd.h>
#include <fcntl.h>
#include "config.h"
#include "dispatchProcess/taskManager.h"
#include "masterProcess/masterProcess.h"
#include "dispatchProcess/dispatchProcess.h"
#include "workProcess/workProcess.h"
#include "define.h"
const int BUFF_SIZE = 11;

int main(int argc, char **argv) {

  initDeviceId(10010);
  initGrpcServer(argv[1]);

  auto parent_pid = getpid();
  taskManager::initParentPID(parent_pid);

  int pfd[2];

  if (pipe(pfd) == -1)
    printf("failed to create a pipe");

  switch (fork())
  {
    case -1:
      printf("failed to create a process");


    case 0:run_work(pfd);
//      if (close(pfd[1]) == -1)
//        _exit(1);
//
//      char read_buff[BUFF_SIZE];
//
//      int readn = read(pfd[0], read_buff, BUFF_SIZE);
//
//      printf("child process read: %s\n", read_buff);

      _exit(0);
  }

  if (close(pfd[0]) == -1)
    exit(1);

  char write_buff[] = "hello world";

  std::cout<<"--------------->"<<std::endl;

  sleep(10);

//  int flag=fcntl(pfd[1], F_GETFL);
//  fcntl(pfd[1], F_SETFL, flag|O_NONBLOCK);

TASK_INFO info;
info.task_id = 10010;
info.cmd = 0;

  int writen = write(pfd[1], &info, sizeof(TASK_INFO));

  close(pfd[1]);

  std::cout<<"--------------->"<<writen<<std::endl;

  exit(0);



 // run_work();

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

