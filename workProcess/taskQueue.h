//
// Created by gh on 2021/9/25.
//

#ifndef TEST_WORKPROCESS_TASKQUEUE_H_
#define TEST_WORKPROCESS_TASKQUEUE_H_

#include <mutex>
#include <condition_variable>
#include <queue>
#include "define.h"


class TaskQueue  final{
 public:
  TaskQueue() = default;
  TaskQueue(const TaskQueue &) = delete;
  TaskQueue & operator=(const TaskQueue &) = delete;
  ~TaskQueue() = default;

 public:
  bool isEmpty();

  int push_back(TASK_INFO );

  TASK_INFO pop();


 private:
  std::mutex mutex_;
  std::condition_variable oneFinished_;
  std::queue<TASK_INFO> queue_;


};

#endif //TEST_WORKPROCESS_TASKQUEUE_H_
