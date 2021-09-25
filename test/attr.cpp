#include "prePost.h"
#include "json.hpp"
#include "tensor.h"

class AttrPrePost : public PrePost {
 public:
  AttrPrePost() {};
  ~AttrPrePost() {};
  arctern::Tensor<float> PreProcess(InputParam & param) override {

    std::cout<<"work--------------------->"<<__FILE__<<"  "<<__LINE__<<" "<<param.path<<std::endl;
    using arctern::Tensor;
    cv::Mat mat = cv::imread(param.path);
    cv::Mat face = mat(param.rect).clone();
    cv::cvtColor(face, face, cv::COLOR_BGR2RGB);
    cv::Mat fimage,  resizeMat;
    cv::resize(face, resizeMat, cv::Size(w, h));
    float std = 128.0f;
    float mean = 127.5;
    float alpha = 1.0f / std;
    float beta = -1 * mean / std;
    std::cout<<resizeMat.cols<<resizeMat.rows<<std::endl;
    resizeMat.convertTo(fimage, CV_32F, alpha, beta);
    Tensor<float> input_tensor;
    input_tensor.from_cvmat(fimage, true);
    return input_tensor;
  }

  std::string PostProcess(std::vector<arctern::Tensor<float>> & tensors) override {
    int stride = tensors[1].size() / tensors[1].shape()[0];
    const float * data1 = tensors[1].data();
    assert(stride == tensors[1].size());
    float age = 0;
    float score = 0;
    for(auto i = 0 ; i < stride; i++) {
      age += data1[i] * i;
      score = std::max(score, data1[i]);
    }

    nlohmann::json j;
    j["age"] = age;
    j["ageScore"] = score;
    return j.dump();
  }

  void initModel(std::vector<uint> & input_dims, int & output_num, std::vector<std::string> & output_layers) override{
    input_dims.clear();
    std::vector<uint> dims = {1,3,h,w};
    input_dims = dims;
    output_num = 2;
  }

 private:
  int w = 112;
  int h = 128;
};

void *createAlgHandle() {
  AttrPrePost *handle = new AttrPrePost;
  return handle;
}