//
// Created by gh on 2021/9/17.
//

#ifndef TEST_GRPC_JOB_MANAGER_H_
#define TEST_GRPC_JOB_MANAGER_H_

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
using JobManager::GetJobDatasetReply;

class JobManagerClient {
 public:
  JobManagerClient(std::shared_ptr<grpc::Channel> channel) ;
  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  int  GetJobAimodel(int job_id, std::tuple<std::string, std::string, std::string> &model) ;
  int GetJobDataset(int job_id, std::string & dataset_path) ;

  void ReportJob(int job_id, EnumJobStatus status, int job_progress, std::string json_result );


 private:
  std::unique_ptr<JobManager::JobManager::Stub> stub_;
};

#endif //TEST_GRPC_JOB_MANAGER_H_
