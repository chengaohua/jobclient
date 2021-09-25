//
// Created by gh on 2021/9/25.
//

#ifndef TEST_INCLUDE__DEFINE_H_
#define TEST_INCLUDE__DEFINE_H_

#define  TASK_CREATE  0
#define TASK_CANCEL  1

typedef struct {
  int cmd;  // 0 :create  1: cancel
  long task_id;
} TASK_INFO;


#endif //TEST_INCLUDE__DEFINE_H_
