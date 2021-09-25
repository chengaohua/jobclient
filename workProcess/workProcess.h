//
// Created by gh on 2021/9/16.
//

#ifndef TEST_PROCESS_WORKPROCESS_H_
#define TEST_PROCESS_WORKPROCESS_H_

#include "taskQueue.h"

void run_work(int pfd[2]) ;

void pipe_thread(int pfd0, int pfd1, TaskQueue & queue);

void task_thread(TaskQueue & queue);
#endif //TEST_PROCESS_WORKPROCESS_H_
