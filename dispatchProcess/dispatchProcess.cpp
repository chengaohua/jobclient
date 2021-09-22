//
// Created by gh on 2021/9/16.
//
#include<signal.h>
#include<unistd.h>
#include <wait.h>
#include "../grpc/device_manager.h"
#include "../config.h"

void run_task_dispatch ()  {
  std::string target_str = getGrpcServer();
  DeviceManagerClient greeter(
      grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
  greeter.dispatch();
}