//
// Created by gh on 2021/9/25.
//

#include <unistd.h>
#include "signal.h"
#include <cstdio>
#include "daemonProcess.h"
#include "workProcess.h"
#include <sys/wait.h>

//void
//handler(int signum)
//{
//  if(signum == SIGCHLD)
//    printf("SIGIO   signal: %d\n", signum);
//  else if(signum == SIGUSR1)
//    printf("SIGUSR1   signal: %d\n", signum);
//  else
//    printf("error\n");
//}


void run_daemon_work(int pfd[2]) {
  while (true) {
    auto pid = fork();
    switch (pid) {
      case -1: {
        printf("failed to create a process");
        break;
      }
      case 0 : {
        run_work(pfd);
        _exit(0);
      }

    }

    int status = 0;

    waitpid(pid, &status, 0 );
    printf("waitpid ===        %d\n", status);
  }

  //signal(SIGCHLD, handler);
//
//  close(pfd[0]);
//  close(pfd[1]);
}