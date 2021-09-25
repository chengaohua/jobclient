//
// Created by gh on 2021/9/16.
//

#ifndef TEST_PROCESS_TASKPROCESS_H_
#define TEST_PROCESS_TASKPROCESS_H_
#include "define.h"

void run_task_dispatch (int pid[2]);

class DispatchCallback {
 public:
  DispatchCallback(int pfd0) ;
  DispatchCallback(const DispatchCallback &) = delete;
  DispatchCallback & operator=(const DispatchCallback &) = delete;
  ~DispatchCallback();

  void callback(TASK_INFO );
 private:
  int pfd_;

};

#endif //TEST_PROCESS_TASKPROCESS_H_

