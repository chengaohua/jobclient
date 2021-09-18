//
// Created by gh on 2021/9/17.
//

#include "dataset_mgr.h"
#include "json.hpp"


DatasetMgr::DatasetMgr() {

}

DatasetMgr::~DatasetMgr() {

}

void DatasetMgr::parse(std::string dataset_path) {
  using nlohmann::json;
  std::ifstream i(dataset_path);
  json j;
  i >> j;
  for(auto it = j.begin(); it != j.end(); ++it) {
    InputParam data;
    data.path = (*it)["path"].get<std::string>();
    if(it->contains("rect")) {
      auto rect = (*it)["rect"].get<std::vector<float>>();
      data.rect.x = rect[0];
      data.rect.y = rect[1];
      data.rect.width = rect[2];
      data.rect.height = rect[3];
    }
    if(it->contains("landmarks")) {
      auto landmark = (*it)["landmarks"].get<std::vector<float>>();
      int len = landmark.size() / 2;
      for(int i = 0 ; i < len; i++) {
        cv::Point2f point(landmark[2 * i], landmark[2 * i + 1]);
        data.landmarks.push_back(point);
      }
    }

    sets_.push_back(data);
  }
}

int DatasetMgr::next(InputParam &input_param) {
  if(pos_ < length()) {
    input_param = sets_[pos_];
    return 0;
  }
  return -1;
}

int DatasetMgr::length() {
  return sets_.size();
}
