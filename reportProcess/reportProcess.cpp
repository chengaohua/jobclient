//
// Created by gh on 2021/9/16.
//

#include "../grpc/device_manager.h"
#include "../workProcess/workProcess.h"
#include "../grpc/job_manager.h"
#include "reportProcess.h"
#include <stdio.h>
#include<signal.h>
#include<unistd.h>
#include <wait.h>
#include <atomic>
#include "../config.h"

void run_report() {
  std::string target_str = getGrpcServer();
  DeviceManagerClient greeter(
      grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));

  while (true) {
    std::string user;
    std::string reply = greeter.report(getDeviceId(), user);
    std::cout << "reportProcess: " << reply << std::endl;
    sleep(10);
  }
}

