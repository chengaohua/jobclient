//
// Created by gh on 2021/9/17.
//

#ifndef TEST_DATASET_DATASET_MGR_H_
#define TEST_DATASET_DATASET_MGR_H_

#include "opencv2/opencv.hpp"
#include "prePost.h"

class DatasetMgr final {
 public:
  DatasetMgr();

  DatasetMgr(const DatasetMgr &) = delete;
  DatasetMgr & operator=(const DatasetMgr &) = delete;
  ~DatasetMgr();

  void parse(std::string dataset_path);

  int next( InputParam & input_param);

  int length();
 private:
  std::vector<InputParam> sets_;
  int pos_ = 0;
};

#endif //TEST_DATASET_DATASET_MGR_H_
