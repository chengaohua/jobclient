//
// Created by gh on 2021/9/16.
//

#include "workProcess.h"
#include <stdio.h>
#include <iostream>
#include <csignal>
#include "jobTask.h"
#include <signal.h>


typedef enum {
  JOB_NONE,
  JOB_RUNNING,
}JOB_STATUS;

typedef enum {
  CMD_NONE,
  CMD_CREATE,
  CMD_CANCEL,
} CMD_TYPE;

typedef struct {
  CMD_TYPE cmd = CMD_NONE;
  int task_id = 0;
} CMD;

CMD g_cmd;

static int g_job_id = 0;
static JOB_STATUS g_job_status = JOB_NONE;
void sig_siguser1_from_parent(int signum, siginfo_t *info, void *myact) {
// new job
  g_cmd.cmd =CMD_CREATE;
  g_cmd.task_id = info->si_int;
}
void sig_siguser2_from_parent(int signum, siginfo_t *info, void *myact) {
  g_cmd.cmd =CMD_CANCEL;
  g_cmd.task_id = info->si_int;
}

typedef enum {
  OPERATOR_CONTINUE,
  OPERATOR_RESET,
  OPERATRO_CANCEL,
} OPERATOR;

OPERATOR status_check() {
  if(CMD_NONE == g_cmd.cmd ) {
    if(JOB_NONE == g_job_status) {

      // 进程挂起
      sigset_t empty;
      sigemptyset(&empty);
      sigsuspend(&empty);
      return OPERATOR_CONTINUE;
    } else if(JOB_RUNNING == g_job_status) {
      return OPERATOR_CONTINUE;
    }
  }

  if(CMD_CREATE == g_cmd.cmd) {
    if(JOB_NONE == g_job_status) {
      // begin run job
      g_job_id = g_cmd.task_id;
      g_cmd.cmd = CMD_NONE;
      g_job_status = JOB_RUNNING;

      return OPERATOR_RESET;
    } else {
      //continue run
      g_job_id = g_cmd.task_id;
      g_cmd.cmd = CMD_NONE;
      g_job_status = JOB_RUNNING;
      return OPERATOR_RESET;
    }

  }

  if(CMD_CANCEL == g_cmd.cmd) {
    if(JOB_NONE == g_job_status) {
      // begin run job
      g_job_status = JOB_NONE;
      g_cmd.cmd = CMD_NONE;
      return OPERATRO_CANCEL;
    } else if(JOB_RUNNING == g_job_status) {
      //continue run
      g_cmd.cmd = CMD_NONE;
      g_job_status = JOB_NONE;
      return OPERATRO_CANCEL;
    }
  }

  return OPERATOR_CONTINUE;

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
    auto op = status_check();
    std::unique_ptr<JobTask> job;

    if(OPERATOR_RESET == op) {
      job.reset(new JobTask(g_job_id));
    } else if( OPERATOR_CONTINUE == op)  {
      job->next();
    } else if (OPERATRO_CANCEL == op) {
      continue;
    }

  }
  // std::cout<<" run child end!"<<std::endl;
  return;
}