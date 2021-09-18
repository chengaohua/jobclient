//
// Created by gh on 2021/6/2.
//

#ifndef TEST_MXNETENGIN_H
#define TEST_MXNETENGIN_H

#include "mxnet/c_predict_api.h"
#include "../engine.h"
#include <string>


typedef struct {
  std::string json;
  std::string params;
  int outputNum;
  std::vector<uint> inputDims;
} MxnetModel;

class MxnetEngin final : public Engine{
 public:
  MxnetEngin();

  MxnetEngin(const MxnetEngin &) = delete;
  MxnetEngin &operator=(const MxnetEngin &) = delete;

  ~MxnetEngin();
  bool loadModel(MxnetModel &info);
  std::vector<arctern::Tensor<float>> inference(arctern::Tensor<float> &input) override;
 private:
  PredictorHandle netHandle_;  /// < mxnet predictor handle
  int outputNum_;
};

#endif //TEST_MXNETENGIN_H
