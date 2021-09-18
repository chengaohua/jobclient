//
// Created by gh on 2021/9/13.
//

#include "device_manager.h"
#include "../taskProcess/taskManager.h"

DeviceManagerClient::DeviceManagerClient(std::shared_ptr<grpc::Channel> channel) : stub_(JobManager::JobManager::NewStub(channel)) {
}

std::string DeviceManagerClient::report(int32_t device_id, const std::string &device_name) {
  // Data we are sending to the server.
  ReportDeviceRequest request;
  request.set_device_id(device_id);
  request.set_device_name(device_name);

  // Container for the data we expect from the server.
  ReportDeviceReply reply;

  // Context for the client. It could be used to convey extra information to
  // the server and/or tweak certain RPC behaviors.
  grpc::ClientContext context;

  // The actual RPC.
  Status status = stub_->ReportDevice(&context, request, &reply);

  // Act upon its status.
  if (status.ok()) {
    return "success";
  } else {
    std::cout << status.error_code() << ": " << status.error_message()
              << std::endl;
    return "RPC failed";
  }
}



void DeviceManagerClient::dispatch() {
  DispatchJobIDRequest request;
  request.set_device_id(1024);
  grpc::ClientContext context;
  std::unique_ptr<grpc::ClientReader < DispatchJobIDStream> > reader(
      stub_->DispatchJobID(&context, request));
  DispatchJobIDStream task;
  while (reader->Read(&task)) {

    dispatchTask(task);
    std::cout<<task.job_id()<<std::endl;
  }
  Status status = reader->Finish();
  if (status.ok()) {
    std::cout << "ListFeatures rpc succeeded." << std::endl;
  } else {
    std::cout << "ListFeatures rpc failed." << std::endl;
  }
}



void DeviceManagerClient::dispatchTask(DispatchJobIDStream &task) {
  if(task.job_type() == JobManager::JobTypeCreateJob) {
    /// new task job
    taskManager::createTask(task.job_id());

  } else if (JobManager::JobTypeCancelJob == task.job_type()) {
    /// cancel task job
    taskManager::cancelTask(task.job_id());
  }
}

