//
// Created by gh on 2021/9/16.
//

#include "../grpc/device_manager.h"
#include "workProcess.h"
#include "masterProcess.h"
#include <stdio.h>
#include<signal.h>
#include<unistd.h>
#include <wait.h>
#include <atomic>

#define INVALID_PROCESS 0

typedef enum {
  WORK_PROCESS_NONE,
  WORK_PROCESS_RUNNING,
  WORK_PROCESS_EXITED
} WORK_PROCESS_STATUS;

typedef enum {
  TASK_NONE,
  TASK_CRETATE,
  TASK_CANCEL,
} TASK_STATUS;

typedef struct {
  WORK_PROCESS_STATUS process_status_ = WORK_PROCESS_NONE;
  pid_t pid_ = INVALID_PROCESS;
} WORK_PROCESS_INFO;

typedef struct {
  TASK_STATUS  task_status_ = TASK_NONE;
  int task_id_ = 0;
} TASK_INFO;

WORK_PROCESS_INFO work_process_info;
TASK_INFO task_info;

//子进程推出信号
void sig_child(int signum,siginfo_t *info,void *myact) {
  pid_t  pid;

  //处理僵尸进程, -1 代表等待任意一个子进程, WNOHANG代表不阻塞
  while ((pid = waitpid(-1, NULL, WNOHANG)) > 0)
  {
    if(pid == work_process_info.pid_) {
      work_process_info.pid_ = INVALID_PROCESS;
      work_process_info.process_status_ = WORK_PROCESS_EXITED;
    }
  }
}

void sig_sigusr1_from_child(int signum,siginfo_t *info,void *myact) {
  //dispatch task to work process
  int task_id = info->si_int;
  task_info.task_id_ = task_id;
  task_info.task_status_ = TASK_CRETATE;
  std::cout<<"---------------------------------->"<<"sig_sigusr1_from_child"<<std::endl;
}

void sig_sigusr2_from_child(int signum,siginfo_t *info,void *myact) {
  // dispatch task to work process
  //dispatch task to work process
  int task_id = info->si_int;
  task_info.task_id_ = task_id;
  task_info.task_status_ = TASK_CANCEL;
}

void checkWorkProcess() {
  std::cout<<__FILE__<<" "<<__LINE__<<" "<<work_process_info.process_status_<<std::endl;

  if(work_process_info.process_status_ != WORK_PROCESS_RUNNING) {
    std::cout<<__FILE__<<" "<<__LINE__<<std::endl;
    auto pid = fork();
    if(pid == 0 ) {
      //child
      run_work();
      exit(0);
    }else if (pid > 0 ) {
      //parent
      work_process_info.process_status_ = WORK_PROCESS_RUNNING;
      work_process_info.pid_ = pid;
    }
  }
}

void checkTask() {
  if(task_info.task_status_ == TASK_CRETATE) {
    union sigval mysigval;
    mysigval.sival_int=task_info.task_id_;
    //create job
    if(sigqueue(work_process_info.pid_,SIGUSR1,mysigval)==-1)
      printf("send error＼n");
  } else if (task_info.task_status_ == TASK_CANCEL) {
    union sigval mysigval;
    mysigval.sival_int=task_info.task_id_;
    //create job
    if(sigqueue(work_process_info.pid_,SIGUSR2,mysigval)==-1)
      printf("send error＼n");
  }
}

void run_master() {
  auto registerSignal = [](int signum, decltype(sig_sigusr1_from_child) func) {
    struct sigaction act;
    int sig = signum;

    sigemptyset(&act.sa_mask);
    act.sa_sigaction = func;
    act.sa_flags = SA_SIGINFO;

    if (sigaction(sig, &act, NULL) < 0) {
      printf("install sigal error＼n");
    }
  };

  registerSignal(SIGUSR1, sig_sigusr1_from_child);
  registerSignal(SIGUSR2, sig_sigusr2_from_child);
  registerSignal(SIGCHLD, sig_child);

  std::string target_str = "0.0.0.0:50051";
  DeviceManagerClient greeter(
      grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));

  while (true) {
    checkWorkProcess();
    checkTask();
    std::string user;
    std::string reply = greeter.report(10024, user);
    std::cout << "Greeter received: " << reply << std::endl;
    sleep(10);
  }
}