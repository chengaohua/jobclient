//
// Created by gh on 2021/9/17.
//

#include "job_manager.h"

JobManagerClient::JobManagerClient(std::shared_ptr<grpc::Channel> channel) : stub_(taskmanager::TaskManagerBack::NewStub(channel)) {
}

int  JobManagerClient::GetTaskAimodel(int job_id, std::tuple<std::string, std::string, std::string> &model){
  // Data we are sending to the server.
  GetTaskAimodelRequest request;
  request.set_task_id(job_id);

  // Container for the data we expect from the server.
  GetTaskAimodelReply reply;

  // Context for the client. It could be used to convey extra information to
  // the server and/or tweak certain RPC behaviors.
  grpc::ClientContext context;

  std::cout<<"work--------------------->"<<__FILE__<<"  "<<__LINE__<<std::endl;
  // The actual RPC.
  Status status = stub_->GetTaskAimodel(&context, request, &reply);

  std::cout<<"work--------------------->"<<__FILE__<<"  "<<__LINE__<<std::endl;

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

int JobManagerClient::GetTaskDataset(int job_id, std::string & dataset_path)  {
  // Data we are sending to the server.
  GetTaskDatasetRequest request;
  request.set_task_id(job_id);

  // Container for the data we expect from the server.
  GetTaskDatasetReply reply;

  // Context for the client. It could be used to convey extra information to
  // the server and/or tweak certain RPC behaviors.
  grpc::ClientContext context;

  // The actual RPC.
  Status status = stub_->GetTaskDataset(&context, request, &reply);

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

void JobManagerClient::ReportTask(int job_id, EnumTaskStatus job_status, int job_progress, std::string json_result ) {
  // Data we are sending to the server.
  ReportTaskRequest request;
  request.set_task_id(job_id);
  request.set_task_status(job_status);
  request.set_task_progress(job_progress);
  request.set_json_result(json_result);

  // Container for the data we expect from the server.
  ReportTaskReply reply;

  // Context for the client. It could be used to convey extra information to
  // the server and/or tweak certain RPC behaviors.
  grpc::ClientContext context;

  // The actual RPC.
  Status status = stub_->ReportTask(&context, request, &reply);

  // Act upon its status.
  if (status.ok()) {
    return ;
  } else {
    std::cout << status.error_code() << ": " << status.error_message()
              << std::endl;
    return ;
  }
}





