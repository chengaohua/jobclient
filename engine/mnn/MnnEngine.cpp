//
// Created by gh on 2021/8/30.
//

#include "MnnEngine.h"
#include <numeric>
#include <functional>
#include "tensor.h"


MnnEngin::~MnnEngin() {
  if(interpreter_) {
    interpreter_->releaseModel();
    interpreter_->releaseSession(session_);
  }
}

int MnnEngin::loadModel(const char *model_path, std::vector<std::string> output_layer) {
  interpreter_ = std::unique_ptr<MNN::Interpreter>(MNN::Interpreter::createFromFile(model_path));

  MNN::ScheduleConfig schedule_config;
  schedule_config.type = MNN_FORWARD_CPU;
  schedule_config.numThread = 4;
  MNN::BackendConfig backend_config;
  backend_config.precision = MNN::BackendConfig::Precision_Normal;
  schedule_config.backendConfig = &backend_config;

  session_ = interpreter_->createSession(schedule_config);
  input_tensor_ = interpreter_->getSessionInput(session_, nullptr);

  output_layer_.swap(output_layer);

  return 0;
}

static int calcShapeSize(std::vector<int> shape) {
  return  std::accumulate(shape.begin(), shape.end(), 1, std::multiplies<int>());
}



std::vector<arctern::Tensor<float>> MnnEngin::forward(arctern::Tensor<float> input_tensor) {
  std::vector<arctern::Tensor<float>> output_tensors;
  std::vector<int> dims;
  for(int i = 0; i< input_tensor.shape().num_dims(); i++) {
    dims.push_back(input_tensor.shape()[i]);
  }

  auto nhwc_tensor = MNN::Tensor::create<float>(dims, (void *)input_tensor.data(), MNN::Tensor::TENSORFLOW);

  input_tensor_->copyFromHostTensor(nhwc_tensor);
  // forward
  interpreter_->runSession(session_);

  for(auto name : output_layer_) {
    MNN::Tensor * tensor = interpreter_->getSessionOutput(session_, name.c_str());
    auto size = calcShapeSize(tensor->shape());
    arctern::TensorShape shape(1, size);
    arctern::Tensor<float>  outputTensor(shape);

    MNN::Tensor hostTensor(tensor,tensor->getDimensionType());
    tensor->copyToHostTensor(&hostTensor);

    memcpy(outputTensor.data(), hostTensor.host<float>(), size * sizeof(float));
    output_tensors.push_back(outputTensor);
  }

  return output_tensors;
}

