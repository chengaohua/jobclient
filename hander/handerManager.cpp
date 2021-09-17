//
// Created by gh on 2021/9/16.
//

#include "handerManager.h"
#include <stdio.h>

namespace handerManager {

static pid_t work_process_pid = 0;

void initWorkProcess(pid_t pid) {
  work_process_pid = pid;
}




}