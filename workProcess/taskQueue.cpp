//
// Created by gh on 2021/9/25.
//

#include "taskQueue.h"

bool TaskQueue::isEmpty() {
  std::lock_guard<std::mutex> lck(mutex_);
  return queue_.empty();
}

int TaskQueue::push_back(TASK_INFO info) {
  std::unique_lock<std::mutex> lock(mutex_);
  queue_.push(info);
  oneFinished_.notify_all();
  return 0;
}

TASK_INFO TaskQueue::pop() {
  std::unique_lock<std::mutex> lock(mutex_);
  oneFinished_.wait(lock, [this]() {
    return ! queue_.empty();
  });

  auto info = queue_.front();
  queue_.pop();
  return info;
}