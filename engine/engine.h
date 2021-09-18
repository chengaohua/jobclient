//
// Created by gh on 2021/9/17.
//

#ifndef TEST_ENGINE_ENGINE_H_
#define TEST_ENGINE_ENGINE_H_

#include "tensor.h"
class Engine {
 public:
  virtual std::vector<arctern::Tensor<float>> inference(arctern::Tensor<float> &input) = 0;
};

namespace EngineFactory {

typedef struct {
  std::string json;
  std::string params;
  int outputNum;
  std::vector<uint> inputDims;
} ModelInfo;

std::unique_ptr<Engine> buildEngine(ModelInfo & model_info);
};

#endif //TEST_ENGINE_ENGINE_H_
