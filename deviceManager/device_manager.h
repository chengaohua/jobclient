//
// Created by gh on 2021/9/13.
//

#ifndef TEST_DEVICEMANAGER_DEVICE_MANAGER_H_
#define TEST_DEVICEMANAGER_DEVICE_MANAGER_H_

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include "jobmanager.pb.h"
#include "jobmanager.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using JobManager::ReportDeviceRequest;
using JobManager::ReportDeviceReply;
using JobManager::ReportJobRequest;
using JobManager::ReportJobReply;
using JobManager::CpuInfo;
using JobManager::MemoryInfo;
using JobManager::GpuInfo;
using JobManager::DispatchJobIDRequest;
using JobManager::DispatchJobIDStream;
using JobManager::EnumJobType;
using JobManager::EnumJobStatus;
using JobManager::GetJobAimodelRequest;
using JobManager::GetJobAimodelReply;
using JobManager::GetJobDatasetRequest;
using JobManager::GetJobAimodelReply;

class DeviceManagerClient {
 public:
  DeviceManagerClient(std::shared_ptr<grpc::Channel> channel) ;
  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  std::string report(int32_t device_id, const std::string &device_name) ;
  void dispatch() ;
 private:
  ///<  分发任务task
  void dispatchTask(DispatchJobIDStream & task);
  std::unique_ptr<JobManager::JobManager::Stub> stub_;
};

#endif //TEST_DEVICEMANAGER_DEVICE_MANAGER_H_
