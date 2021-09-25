//
// Created by gh on 2021/9/13.
//

#include "device_manager.h"
#include "../config.h"

DeviceManagerClient::DeviceManagerClient(std::shared_ptr<grpc::Channel> channel) : stub_(taskmanager::TaskManagerBack::NewStub(channel)) {
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



void DeviceManagerClient::dispatch(DispatchCallback & callback) {
  DispatchTaskIDRequest request;
  request.set_device_id(getDeviceId());
  grpc::ClientContext context;
  std::unique_ptr<grpc::ClientReader < DispatchTaskIDStream> > reader(
      stub_->DispatchTaskID(&context, request));
  DispatchTaskIDStream task;
  while (reader->Read(&task)) {
    TASK_INFO  task_info;
    task_info.task_id = task.task_id();
    std::cout<<"work--------------------->"<<__FILE__<<"  "<<__LINE__<<std::endl;
    switch (task.task_type()) {
      case taskmanager::TaskTypeCreateTask:
        task_info.cmd = TASK_CREATE;
        callback.callback(task_info);
        break;
      case taskmanager::TaskTypeCancelTask:
        task_info.cmd = TASK_CANCEL;
        callback.callback(task_info);
      default:
        break;
    }
  }
  Status status = reader->Finish();
  if (status.ok()) {
    std::cout << "ListFeatures rpc succeeded." << std::endl;
  } else {
    std::cout << "ListFeatures rpc failed." << std::endl;
  }
}





