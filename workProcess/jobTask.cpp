//
// Created by gh on 2021/9/20.
//

#include <memory>
#include "jobTask.h"

JobTask::JobTask(int job_id) :job_id_(job_id){
  std::string target_str = "0.0.0.0:50051";
  grpc_client_ = std::make_unique<JobManagerClient>(grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials());
  init_ok = init() == 0;
}

JobTask::~JobTask() {

}

int JobTask::init() {
  std::tuple<std::string, std::string, std::string> model;
  auto ret =  grpc_client_->GetJobAimodel(job_id_, model);
  if(ret < 0) {
    return -1;
  }

  so_path_ = std::get<0>(model);
  aimodel_json_ = std::get<1>(model);
  aimodel_params_ = std::get<2>(model);

  pre_post_ = Plugin::loadPlugin(so_path_);
  if(!pre_post_) {
    return -1;
  }

  std::vector<uint>  input_dims;
  int  output_num;
  std::vector<std::string>  output_layers;
  pre_post_->initModel(input_dims, output_num, output_layers);

  std::string dataset_path;
  grpc_client_->GetJobDataset(job_id_, dataset_path);

  dataset_mgr_.parse(dataset_path);

  EngineFactory::ModelInfo model_info;
  model_info.json = aimodel_json_;
  model_info.params = aimodel_params_;
  model_info.outputNum = output_num;
  model_info.inputDims = input_dims;

  engine_ = EngineFactory::buildEngine(model_info);

  if(!engine_) {
    return -1;
  }



  return 0;
}

int JobTask::next(std::string & json_str) {
  InputParam input_param;
  auto ret = dataset_mgr_.next(input_param);
  auto input_tensor =pre_post_->PreProcess(input_param);
  auto output_tensors = engine_->inference(input_tensor);
  json_str = pre_post_->PostProcess(output_tensors);
  return 0;
}