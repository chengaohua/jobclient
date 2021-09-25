//
// Created by gh on 2021/9/13.
//

#ifndef TEST_DEVICEMANAGER_DEVICE_MANAGER_H_
#define TEST_DEVICEMANAGER_DEVICE_MANAGER_H_

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include "taskmanagerback.pb.h"
#include "taskmanagerback.grpc.pb.h"
#include "../dispatchProcess/dispatchProcess.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using taskmanager::ReportDeviceRequest;
using taskmanager::ReportDeviceReply;
using taskmanager::ReportTaskRequest;
using taskmanager::ReportTaskReply;
using taskmanager::CpuInfo;
using taskmanager::MemoryInfo;
using taskmanager::GpuInfo;
using taskmanager::DispatchTaskIDRequest;
using taskmanager::DispatchTaskIDStream;
using taskmanager::EnumTaskType;
using taskmanager::EnumTaskStatus;
using taskmanager::GetTaskAimodelRequest;
using taskmanager::GetTaskAimodelReply;
using taskmanager::GetTaskDatasetRequest;
using taskmanager::GetTaskAimodelReply;

class DeviceManagerClient {
 public:
  DeviceManagerClient(std::shared_ptr<grpc::Channel> channel) ;
  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  std::string report(int32_t device_id, const std::string &device_name) ;
  void dispatch(DispatchCallback & callback) ;


 private:
  ///<  分发任务task
 // void dispatchTask(DispatchTaskIDStream & task);
  std::unique_ptr<taskmanager::TaskManagerBack::Stub> stub_;
};

#endif //TEST_DEVICEMANAGER_DEVICE_MANAGER_H_
