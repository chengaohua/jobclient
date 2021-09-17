//
// Created by gh on 2021/9/16.
//

#include "workProcess.h"
#include <stdio.h>
#include <iostream>
#include "../hander/handerManager.h"

void sig_siguser1_from_parent(int signum, siginfo_t *info, void *myact) {

}
void sig_siguser2_from_parent(int signum, siginfo_t *info, void *myact) {

}

void run_work() {
  std::cout<<"run_work"<<std::endl;
  auto registerSignal = [](int signum, decltype(sig_siguser1_from_parent) func) {
    struct sigaction act;
    int sig = signum;

    sigemptyset(&act.sa_mask);
    act.sa_sigaction = func;
    act.sa_flags = SA_SIGINFO;

    if (sigaction(sig, &act, NULL) < 0) {
      printf("install sigal error＼n");
    }
  };

  registerSignal(SIGUSR1, sig_siguser1_from_parent);
  registerSignal(SIGUSR2, sig_siguser2_from_parent);

  while (true) {
    // std::cout<<"run child ! \n";
    sleep(20);

  }
  // std::cout<<" run child end!"<<std::endl;
  return;
}