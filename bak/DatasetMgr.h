//
// Created by gh on 2021/6/3.
//

#ifndef TEST__DATASETMGR_H_
#define TEST__DATASETMGR_H_
#include <string>
#include "opencv2/opencv.hpp"

typedef struct {
  std::string path;
} BASEPIC;

typedef struct {
  std::string path;
  cv::Rect rect;
}PICWITHRECT;

typedef struct {
  std::string path;
  std::vector<cv::Point2f> landmarks;
  cv::Rect rect;
}PICWITHLANDMARK;

class DatasetMgr final {
 public:
  DatasetMgr() = default;
  ~DatasetMgr() = default;

 public:
  std::vector<BASEPIC> parseJsonBase(std::string json);
  std::vector<PICWITHRECT> parseJsonWithRect(std::string json);
  std::vector<PICWITHLANDMARK> parseJsonWithLandmark(std::string json);

 private:

};

#endif //TEST__DATASETMGR_H_
