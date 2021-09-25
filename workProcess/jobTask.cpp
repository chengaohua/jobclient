//
// Created by gh on 2021/9/20.
//

#include <memory>
#include "opencv2/opencv.hpp"
#include "jobTask.h"
#include "../config.h"

JobTask::JobTask(int job_id) :job_id_(job_id){

  init_ok = init() == 0;
}

JobTask::~JobTask() {

}

int JobTask::init() {
  std::string target_str = getGrpcServer();
  auto p =new JobManagerClient(grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
  grpc_client_.reset(p);
  std::cout<<"work--------------------->"<<__FILE__<<"  "<<__LINE__<<std::endl;
  std::tuple<std::string, std::string, std::string> model;
  auto ret =  grpc_client_->GetTaskAimodel(job_id_, model);

  std::cout<<"work--------------------->"<<__FILE__<<"  "<<__LINE__<<" "<<ret<<std::endl;
  if(ret < 0) {
    return -1;
  }

  std::cout<<"work--------------------->"<<__FILE__<<"  "<<__LINE__<<std::endl;
  so_path_ = std::get<0>(model);
  aimodel_json_ = std::get<1>(model);
  aimodel_params_ = std::get<2>(model);
  std::cout<<"work--------------------->"<<__FILE__<<"  "<<__LINE__<<so_path_<<std::endl;
  std::cout<<"work--------------------->"<<__FILE__<<"  "<<__LINE__<<aimodel_params_<<std::endl;
  std::cout<<"work--------------------->"<<__FILE__<<"  "<<__LINE__<<aimodel_json_<<std::endl;
  std::cout<<"work--------------------->"<<__FILE__<<"  "<<__LINE__<<std::endl;
  pre_post_ = Plugin::loadPlugin(so_path_);
  if(!pre_post_) {
    return -1;
  }

  std::cout<<"work--------------------->"<<__FILE__<<"  "<<__LINE__<<std::endl;
  std::vector<uint>  input_dims;
  int  output_num;
  std::vector<std::string>  output_layers;
  pre_post_->initModel(input_dims, output_num, output_layers);
  std::cout<<"work--------------------->"<<__FILE__<<"  "<<__LINE__<<std::endl;

  std::string dataset_path;
  grpc_client_->GetTaskDataset(job_id_, dataset_path);

  std::cout<<"dataset_path "<<dataset_path<<std::endl;
  dataset_mgr_.parse(dataset_path);
  std::cout<<"work--------------------->"<<__FILE__<<"  "<<__LINE__<<std::endl;
  EngineFactory::ModelInfo model_info;
  model_info.json = aimodel_json_;
  model_info.params = aimodel_params_;
  model_info.outputNum = output_num;
  model_info.inputDims = input_dims;

  engine_ = EngineFactory::buildEngine(model_info);
  std::cout<<"work--------------------->"<<__FILE__<<"  "<<__LINE__<<std::endl;

  if(!engine_) {
    return -1;
  }

  std::cout<<"work--------------------->"<<__FILE__<<"  "<<__LINE__<<std::endl;
  alg_result_.clear();

  return 0;
}

int JobTask::next() {
  std::cout<<"work--------------------->"<<__FILE__<<"  "<<__LINE__<<std::endl;
  long current_time = time(nullptr);
  std::cout<<"work--------------------->"<<__FILE__<<"  "<<__LINE__<<std::endl;
  if(current_time - last_access_time_ >  30) {
    // 30s 上报一次
    report();
    last_access_time_ = current_time;
  }

  std::cout<<"work--------------------->"<<__FILE__<<"  "<<__LINE__<<std::endl;

  InputParam input_param;
  auto ret = dataset_mgr_.next(input_param);
  if(ret < 0) {
    reportEnd();
    return -1;
  }

  std::cout<<"work--------------------->"<<__FILE__<<"  "<<__LINE__<<std::endl;
  double t = (double)cv::getTickCount();
  auto input_tensor =pre_post_->PreProcess(input_param);
  auto output_tensors = engine_->inference(input_tensor);
  auto json_str = pre_post_->PostProcess(output_tensors);
  std::cout<<"work--------------------->"<<__FILE__<<"  "<<__LINE__<<std::endl;
  t = cv::getTickCount() - t ;
  t = t / cv::getTickFrequency() * 1000;  /// ms
  alg_exe_sum_time_ += t;
  frame_id_ ++;
  alg_result_.push_back(json_str);
  return 0;
}

int JobTask::report() {
  int job_process = frame_id_ * 100 /dataset_mgr_.length();

  int len = std::min(20, (int)alg_result_.size());
  if(len == 0 ){
    return 0;
  }
  int size = alg_result_.size();

  std::string json_result = "[";
  while (len  > 1) {
    json_result += alg_result_[size - len ];
    json_result += ",";
    len--;
  }

  json_result += alg_result_[size -1];
  json_result += "]";

  grpc_client_->ReportTask(job_id_, taskmanager::TaskStatusSTART, job_process,  json_result);
  return 0;
}

int JobTask::reportEnd() {
  std::string json_result = "[";
  for(auto & item : alg_result_) {
    json_result += item;
    json_result += ",";
  }
  *json_result.rbegin() = ']';
  grpc_client_->ReportTask(job_id_, taskmanager::TaskStatusSTART, 100, json_result );
  return 0;
}

