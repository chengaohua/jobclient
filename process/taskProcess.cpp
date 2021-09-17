//
// Created by gh on 2021/9/16.
//
#include<signal.h>
#include<unistd.h>
#include <wait.h>
#include "../deviceManager/device_manager.h"

void run_task_dispatch ()  {
  std::string target_str = "0.0.0.0:50051";
  DeviceManagerClient greeter(
      grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
  greeter.dispatch();
}