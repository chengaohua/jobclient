//
// Created by gh on 2021/6/2.
//

#include <sys/signalfd.h>
#include<unistd.h>
#include <signal.h>
#include <iostream>
#include "config.h"
#include "workProcess/daemonProcess.h"
#include "reportProcess/reportProcess.h"
#include "dispatchProcess/dispatchProcess.h"
#include "define.h"


int main(int argc, char **argv) {

  initDeviceId(10010);
  initGrpcServer(argv[1]);

  switch (fork()) {
    case -1:printf("failed to create a process");
      break;

    case 0: {
      run_report();
      _exit(0);
    };

  }

  // work process
  int pfd[2];
  if (pipe(pfd) == -1)
    printf("failed to create a pipe");

  switch (fork()) {
    case -1:printf("failed to create a process");
      break;

    case 0:run_daemon_work(pfd);
      _exit(0);
  }

  if (close(pfd[0]) == -1)
    exit(1);


  // dispatch process
  int dispatch_fd[2];
  if (pipe(dispatch_fd) == -1)
    printf("failed to create a pipe");

  switch (fork()) {
    case -1:printf("failed to create a process");

    case 0:run_task_dispatch(dispatch_fd);
      _exit(0);
  }

  if (close(dispatch_fd[1]) == -1)
    exit(1);

//  sigset_t mask;
//  struct signalfd_siginfo fdsi;
//
//  sigemptyset(&mask);
//  sigaddset(&mask, SIGCHLD);
////  if(sigprocmask(SIG_BLOCK, &mask, NULL) == -1) {
////    printf("Fail to sigprocmask!\n");
////  }
//
//  int sfd = signalfd(-1, &mask, 0);

  int max_fd = dispatch_fd[0];

  fd_set read_fds;  //读文件操作符
  //fd_set exception_fds; //异常文件操作符
  FD_ZERO(&read_fds);
  //FD_ZERO(&exception_fds);


  while (true) {
    FD_SET(dispatch_fd[0],&read_fds);
   // FD_SET(sfd, &read_fds);
    auto ret = select(max_fd+1,&read_fds,nullptr,nullptr,nullptr);
    if(ret < 0)
    {
      printf("Fail to select!\n");
      return -1;
    }


    if(FD_ISSET(dispatch_fd[0], &read_fds))
    {
      TASK_INFO  info;
      int readn = read(dispatch_fd[0],&info,sizeof(TASK_INFO));
      if(readn <= 0)
      {
        continue;
      }

      printf("get %d bytes of normal data: \n", readn);

      int writen = write(pfd[1], &info, sizeof (TASK_INFO));

      printf("write %d bytes of normal data: \n", writen);

    }

//    if(FD_ISSET(sfd, &read_fds))
//    {
//      std::cout<<"------------------------------>SIGCHLD"<<std::endl;
//      int readn = read(sfd, &fdsi,sizeof(struct signalfd_siginfo));
//      if(readn <= 0)
//      {
//        continue;
//      }
//
//      if (fdsi.ssi_signo == SIGCHLD) {
//        std::cout<<"------------------------------>SIGCHLD"<<std::endl;
//      }
//
//    }

  }


  return 0;
}

