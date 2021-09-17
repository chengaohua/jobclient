//
// Created by gh on 2021/8/30.
//

#ifndef TEST__MNNENGINE_H_
#define TEST__MNNENGINE_H_

#include "MNN/Interpreter.hpp"
#include "MNN/ImageProcess.hpp"
#include "MNN/MNNDefine.h"
#include "MNN/Tensor.hpp"
#include "tensor.h"

class MnnEngin {
 public:
  MnnEngin() = default;
  ~MnnEngin() ;
  MnnEngin(MnnEngin &) = delete;

  int loadModel(const char * model_path, std::vector<std::string> output_layer);
  std::vector<arctern::Tensor<float>> forward(arctern::Tensor<float> input_tensor);
 private:
  std::unique_ptr<MNN::Interpreter> interpreter_;
  MNN::Session *session_ = nullptr;
  MNN::Tensor *input_tensor_ = nullptr;
  std::vector<std::string > output_layer_;
};

#endif //TEST__MNNENGINE_H_
