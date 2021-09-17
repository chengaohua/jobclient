//
// Created by gh on 2021/6/3.
//

#include "DatasetMgr.h"
#include "json.hpp"

#if 0
#define __MYDEBUG__ do { std::cout << __FILE__ << '[' << __LINE__ << ']' << std::endl; } while(0)
#else
#define __MYDEBUG__
#endif


std::vector<BASEPIC> DatasetMgr::parseJsonBase(std::string name) {
  using nlohmann::json;
  std::ifstream i(name);
  nlohmann::json j;
  i >> j;

  std::vector<BASEPIC> ret;
  for(json::iterator it = j.begin(); it != j.end(); ++it) {
     auto pic = (*it)["file"].get<std::string>();
    BASEPIC basepic;
    basepic.path = pic;
    ret.push_back(basepic);
  }

  return ret;
}

std::vector<PICWITHRECT> DatasetMgr::parseJsonWithRect(std::string name) {
  using nlohmann::json;
  std::ifstream i(name);
  nlohmann::json j;
  i >> j;

__MYDEBUG__;
  std::vector<PICWITHRECT> ret;
  for(json::iterator it = j.begin(); it != j.end(); ++it) {
__MYDEBUG__;
    auto pic = (*it)["file"].get<std::string>();
__MYDEBUG__;
    auto rect = (*it)["rect"].get<std::vector<int>>();
    PICWITHRECT picwithrect;
__MYDEBUG__;
    picwithrect.rect = {rect[0], rect[1], rect[2], rect[3]};
    picwithrect.path=pic;
    ret.push_back(picwithrect);
  }

  return ret;
}

std::vector<PICWITHLANDMARK> DatasetMgr::parseJsonWithLandmark(std::string name) {
  using nlohmann::json;
  std::ifstream i(name);
  nlohmann::json j;
  i >> j;

  std::vector<PICWITHLANDMARK> ret;
  for(json::iterator it = j.begin(); it != j.end(); ++it) {
    auto pic = (*it)["file"].get<std::string>();
    auto rect = (*it)["rect"].get<std::vector<int>>();
    auto landmarks = (*it)["landmark"].get<std::vector<int>>();
    PICWITHLANDMARK picwithlandmark;
    picwithlandmark.rect = {rect[0], rect[1], rect[2], rect[3]};
    picwithlandmark.path=pic;


    ret.push_back(picwithlandmark);
  }

  return ret;
}