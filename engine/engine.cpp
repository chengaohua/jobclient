//
// Created by gh on 2021/9/18.
//

#include <memory>
#include "engine.h"
#include "mxnet/MxnetEngin.h"

namespace EngineFactory {
std::unique_ptr<Engine> buildEngine(ModelInfo & model_info) {
  MxnetEngin * engine = new MxnetEngin;
  MxnetModel mxnet_model;
  mxnet_model.json = model_info.json;
  mxnet_model.params = model_info.params;
  mxnet_model.outputNum = model_info.outputNum;
  mxnet_model.inputDims = model_info.inputDims;
  engine->loadModel(mxnet_model);
  return std::unique_ptr<Engine>(engine);
}
}