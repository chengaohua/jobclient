//
// Created by gh on 2021/9/16.
//

#include "workProcess.h"
#include <stdio.h>
#include <iostream>
#include <csignal>
#include "jobTask.h"
#include <signal.h>
#include "taskQueue.h"
#include <thread>

typedef enum {
  JOB_NONE,
  JOB_RUNNING,
} JOB_STATUS;

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

  std::cout << "sig_siguser1_from_parent" << std::endl;
// new job
  g_cmd.cmd = CMD_CREATE;
  g_cmd.task_id = info->si_int;
}
void sig_siguser2_from_parent(int signum, siginfo_t *info, void *myact) {
  std::cout << "sig_siguser2_from_parent" << std::endl;
  g_cmd.cmd = CMD_CANCEL;
  g_cmd.task_id = info->si_int;
}

typedef enum {
  OPERATOR_CONTINUE,
  OPERATOR_RESET,
  OPERATRO_CANCEL,
} OPERATOR;

OPERATOR status_check() {
  std::cout << "work--------------------->" << __FILE__ << "  " << __LINE__ << std::endl;
  if (CMD_NONE == g_cmd.cmd) {
    if (JOB_NONE == g_job_status) {
//      std::cout<<"work--------------------->"<<__FILE__<<"  "<<__LINE__<<std::endl;
//      // 进程挂起
//      sigset_t empty;
//      sigemptyset(&empty);
//      sigsuspend(&empty);
      g_job_status = JOB_RUNNING;
      return OPERATOR_RESET;

    } else if (JOB_RUNNING == g_job_status) {
      std::cout << "work--------------------->" << __FILE__ << "  " << __LINE__ << std::endl;
      return OPERATOR_CONTINUE;
    }
  }

  if (CMD_CREATE == g_cmd.cmd) {
    if (JOB_NONE == g_job_status) {
      std::cout << "work--------------------->" << __FILE__ << "  " << __LINE__ << std::endl;
      // begin run job
      g_job_id = g_cmd.task_id;
      g_cmd.cmd = CMD_NONE;
      g_job_status = JOB_RUNNING;

      return OPERATOR_RESET;
    } else {
      std::cout << "work--------------------->" << __FILE__ << "  " << __LINE__ << std::endl;
      //continue run
      g_job_id = g_cmd.task_id;
      g_cmd.cmd = CMD_NONE;
      g_job_status = JOB_RUNNING;
      return OPERATOR_RESET;
    }

  }

  if (CMD_CANCEL == g_cmd.cmd) {
    if (JOB_NONE == g_job_status) {
      std::cout << "work--------------------->" << __FILE__ << "  " << __LINE__ << std::endl;
      // begin run job
      g_job_status = JOB_NONE;
      g_cmd.cmd = CMD_NONE;
      return OPERATRO_CANCEL;
    } else if (JOB_RUNNING == g_job_status) {

      std::cout << "work--------------------->" << __FILE__ << "  " << __LINE__ << std::endl;
      //continue run
      g_cmd.cmd = CMD_NONE;
      g_job_status = JOB_NONE;
      return OPERATRO_CANCEL;
    }
  }

  return OPERATOR_CONTINUE;

}


#include "taskQueue.h"

TaskQueue g_queue;


void run_work(int pfd[2]) {
  std::cout << "run_work" << std::endl;

  std::thread t(pipe_thread, pfd[0], pfd[1], std::ref(g_queue));
  std::thread t2(task_thread, std::ref(g_queue));
  t2.join();
  t.join();

  //pipe_thread(pfd[0], pfd[1], g_queue);

//  if (close(pfd[1]) == -1)
//    _exit(1);
//
//  //exit(0);
//
//  char read_buff[BUFF_SIZE];
//
//  int readn = read(pfd[0], read_buff, BUFF_SIZE);
//
//  printf("child process read: %s\n", read_buff);
//  close(pfd[0]);
//  auto registerSignal = [](int signum, decltype(sig_siguser1_from_parent) func) {
//    struct sigaction act;
//    int sig = signum;
//
//    sigemptyset(&act.sa_mask);
//    act.sa_sigaction = func;
//    act.sa_flags = SA_SIGINFO;
//
//    if (sigaction(sig, &act, NULL) < 0) {
//      printf("install sigal error＼n");
//    }
//  };
//
//  registerSignal(SIGUSR1, sig_siguser1_from_parent);
//  registerSignal(SIGUSR2, sig_siguser2_from_parent);

//  while (true) {
//    // std::cout<<"run child ! \n";
//    auto op = status_check();
//    std::unique_ptr<JobTask> job(new JobTask(g_job_id));
//    std::cout << "work--------------------->" << __FILE__ << "  " << __LINE__ << std::endl;
//
//    job->next();
//
////    if(OPERATOR_RESET == op) {
////      std::cout<<"work--------------------->"<<__FILE__<<"  "<<__LINE__<<std::endl;
////      job.reset(new JobTask(g_job_id));
////      std::cout<<"work--------------------->"<<__FILE__<<"  "<<__LINE__<<std::endl;
////    } else if( OPERATOR_CONTINUE == op)  {
////      std::cout<<"work--------------------->"<<__FILE__<<"  "<<__LINE__<<std::endl;
////      job->next();
////    } else if (OPERATRO_CANCEL == op) {
////      std::cout<<"work--------------------->"<<__FILE__<<"  "<<__LINE__<<std::endl;
////      continue;
////    }
//
//  }
//  // std::cout<<" run child end!"<<std::endl;
//  return;
}


void pipe_thread(int pfd0, int pfd1, TaskQueue & queue ) {
  if (close(pfd1) == -1)
    _exit(1);

  //exit(0);

  TASK_INFO task_info;
  //char read_buff[BUFF_SIZE];

  int readn = read(pfd0, &task_info, sizeof (TASK_INFO));

  g_queue.push_back(task_info);

  printf("child process read: %ld, readn = %d\n", task_info.task_id, readn);
  //close(pfd[0]);
}


void task_thread(TaskQueue & queue) {
  while (true) {
    auto info = queue.pop();
    if(info.cmd == 0) {
      std::unique_ptr<JobTask> job(new JobTask(g_job_id));
      std::cout << "work--------------------->" << __FILE__ << "  " << __LINE__ << std::endl;
      while (true) {
        if(!queue.isEmpty()) {
          //队列中有新的命令，退出
          break;
        }
        auto code = job->next();
        if(code != 0) {
          break;
        }
      }

    }
  }
}
