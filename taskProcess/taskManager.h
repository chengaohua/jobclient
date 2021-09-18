//
// Created by gh on 2021/9/16.
//

#ifndef TEST_TASK_TASKMANAGER_H_
#define TEST_TASK_TASKMANAGER_H_

#include<signal.h>
#include<unistd.h>
#include <wait.h>

namespace taskManager {

void initParentPID(pid_t pid);

void createTask(int taskid) ;

void cancelTask(int taskid);
}

#endif //TEST_TASK_TASKMANAGER_H_
