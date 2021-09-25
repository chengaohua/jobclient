//
// Created by gh on 2021/9/16.
//
//#include<signal.h>
//#include<unistd.h>
//#include <wait.h>
//
//#include "../config.h"
//#include "define.h"

#include "dispatchProcess.h"
#include "../grpc/device_manager.h"
#include "../config.h"

void run_task_dispatch (int pid[2])  {

  if (close(pid[0]) == -1)
    _exit(1);

  DispatchCallback dispatch_callback(pid[1]);

  std::string target_str = getGrpcServer();
  DeviceManagerClient greeter(
      grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
  while (true) {
    greeter.dispatch(dispatch_callback);
  }

}

DispatchCallback::DispatchCallback(int pfd0) : pfd_(pfd0) {

}

DispatchCallback::~DispatchCallback() {

}

void DispatchCallback::callback(TASK_INFO info) {
  std::cout<<"work--------------------->"<<__FILE__<<"  "<<__LINE__<<std::endl;
  int writen = write(pfd_, &info, sizeof(TASK_INFO));
}

