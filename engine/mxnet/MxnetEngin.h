//
// Created by gh on 2021/6/2.
//

#ifndef TEST_MXNETENGIN_H
#define TEST_MXNETENGIN_H

#include "mxnet/c_predict_api.h"
#include <string>
#include "tensor.h"

typedef struct {
    uint n;
    uint c;
    uint h;
    uint w;
}NCHW;

typedef struct {
    std::string json;
    std::string params;
    int outputNum;
    NCHW inputShape;
}MxnetModel;

class MxnetEngin final {
public:
    MxnetEngin();

    ~MxnetEngin();
    bool loadModel(MxnetModel & info);
    std::vector<arctern::Tensor<float>> inference(arctern::Tensor<float> & input);
private:
    PredictorHandle netHandle_ ;  /// < mxnet predictor handle
    std::unique_ptr<MxnetModel> info_;
};


#endif //TEST_MXNETENGIN_H
