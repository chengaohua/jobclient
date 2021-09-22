//
// Created by gh on 2021/9/17.
//

#ifndef TEST_GRPC_JOB_MANAGER_H_
#define TEST_GRPC_JOB_MANAGER_H_

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include "taskmanagerback.pb.h"
#include "taskmanagerback.grpc.pb.h"

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
using taskmanager::GetTaskDatasetReply;

class JobManagerClient {
 public:
  JobManagerClient(std::shared_ptr<grpc::Channel> channel) ;
  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  int  GetTaskAimodel(int job_id, std::tuple<std::string, std::string, std::string> &model) ;
  int GetTaskDataset(int job_id, std::string & dataset_path) ;

  void ReportTask(int job_id, EnumTaskStatus status, int job_progress, std::string json_result );


 private:
  std::unique_ptr<taskmanager::TaskManagerBack::Stub> stub_;
};

#endif //TEST_GRPC_JOB_MANAGER_H_
