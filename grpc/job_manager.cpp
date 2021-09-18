//
// Created by gh on 2021/9/17.
//

#include "job_manager.h"
#include "../taskProcess/taskManager.h"

JobManagerClient::JobManagerClient(std::shared_ptr<grpc::Channel> channel) : stub_(JobManager::JobManager::NewStub(channel)) {
}

int  JobManagerClient::GetJobAimodel(int job_id, std::tuple<std::string, std::string, std::string> &model){
  // Data we are sending to the server.
  GetJobAimodelRequest request;
  request.set_job_id(job_id);

  // Container for the data we expect from the server.
  GetJobAimodelReply reply;

  // Context for the client. It could be used to convey extra information to
  // the server and/or tweak certain RPC behaviors.
  grpc::ClientContext context;

  // The actual RPC.
  Status status = stub_->GetJobAimodel(&context, request, &reply);

  // Act upon its status.
  if (status.ok()) {
    std::get<0>(model) = reply.so_path();
    std::get<1>(model) = reply.aimodel_path1();
    std::get<2>(model) = reply.aimodel_path2();
    return 0;
  } else {
    std::cout << status.error_code() << ": " << status.error_message()
              << std::endl;
    return -1;
  }
}

int JobManagerClient::GetJobDataset(int job_id, std::string & dataset_path)  {
  // Data we are sending to the server.
  GetJobDatasetRequest request;
  request.set_job_id(job_id);

  // Container for the data we expect from the server.
  GetJobDatasetReply reply;

  // Context for the client. It could be used to convey extra information to
  // the server and/or tweak certain RPC behaviors.
  grpc::ClientContext context;

  // The actual RPC.
  Status status = stub_->GetJobDataset(&context, request, &reply);

  // Act upon its status.
  if (status.ok()) {
    dataset_path = reply.dataset_path();
    return 0;
  } else {
    std::cout << status.error_code() << ": " << status.error_message()
              << std::endl;
    return -1;
  }
}

void JobManagerClient::ReportJob(int job_id, EnumJobStatus job_status, int job_progress, std::string json_result ) {
  // Data we are sending to the server.
  ReportJobRequest request;
  request.set_job_id(job_id);
  request.set_job_status(job_status);
  request.set_job_progress(job_progress);
  request.set_json_result(json_result);

  // Container for the data we expect from the server.
  ReportJobReply reply;

  // Context for the client. It could be used to convey extra information to
  // the server and/or tweak certain RPC behaviors.
  grpc::ClientContext context;

  // The actual RPC.
  Status status = stub_->ReportJob(&context, request, &reply);

  // Act upon its status.
  if (status.ok()) {
    return ;
  } else {
    std::cout << status.error_code() << ": " << status.error_message()
              << std::endl;
    return ;
  }
}





