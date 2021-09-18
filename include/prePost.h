//
// Created by gh on 2021/6/2.
//

#ifndef TEST_PREPOST_H
#define TEST_PREPOST_H

#ifdef __cplusplus
extern "C" {
#endif
  void * createAlgHandle();

#ifdef __cplusplus
};
#endif

#include "tensor.h"
typedef struct {
    std::string path;
    cv::Rect rect;
    std::vector<cv::Point> landmarks;
}InputParam;

class PrePost {
public:
    virtual arctern::Tensor<float> PreProcess(InputParam & param) = 0;
    virtual std::string PostProcess(std::vector<arctern::Tensor<float>> & tensors) = 0;

    virtual void initModel(std::vector<uint> & input_dims, int & output_num, std::vector<std::string> & output_layers) = 0;
};




#endif //TEST_PREPOST_H
