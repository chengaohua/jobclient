//
// Created by gh on 2021/6/2.
//

#include "prePost.h"

#if 0
#define __MYDEBUG__ do { std::cout << __FILE__ << '[' << __LINE__ << ']' << std::endl; } while(0)
#else
#define __MYDEBUG__
#endif

class DetPrePose : public MxnetPrePost {
 public:
  DetPrePose() {
    n = 1;
    c = 3;
    h = 128;
    w = 112;
    outputNum = 2;
  }

  arctern::Tensor<float> PreProcess(InputParam &param) override {
    using arctern::Tensor;
    // std::cout<<"path = "<<param.path<<param.rect<<std::endl;
__MYDEBUG__;
    cv::Mat mat = cv::imread(param.path);
__MYDEBUG__;
    cv::Mat face = mat(param.rect).clone();
__MYDEBUG__;
    cv::cvtColor(face, face, cv::COLOR_BGR2RGB);
    cv::Mat fimage,  resizeMat;
    cv::resize(face, resizeMat, cv::Size(w, h));
    float std = 128.0f;
    float mean = 127.5;
    float alpha = 1.0f / std;
    float beta = -1 * mean / std;
    // std::cout<<resizeMat.cols<<resizeMat.rows<<std::endl;
__MYDEBUG__;
    resizeMat.convertTo(fimage, CV_32F, alpha, beta);
__MYDEBUG__;
    Tensor<float> input_tensor;
    input_tensor.from_cvmat(fimage, true);
__MYDEBUG__;
    return input_tensor;
  }

  nlohmann::json PostProcess(std::vector<arctern::Tensor<float>> &tensors) override {
    int stride = tensors[1].size() / tensors[1].shape()[0];
    const float * data1 = tensors[1].data();
    assert(stride == tensors[1].size());
    float age = 0;
    float score = 0;
    for(auto i = 0 ; i < stride; i++) {
      age += data1[i] * i;
      score = std::max(score, data1[i]);
    }

    using nlohmann::json;

    json j;
    j["age"] = age;
    j["score"] = score;

    return j;
  }
};



void *createAlgHandle() {
  DetPrePose *handle = new DetPrePose;
  return handle;
}
