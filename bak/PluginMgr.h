//
// Created by gh on 2021/6/3.
//

#ifndef TEST_PLUGINMGR_H
#define TEST_PLUGINMGR_H

#include "prePost.h"
#include <string>

class PluginMgr {
 public:
  PluginMgr();
  ~PluginMgr();

  std::unique_ptr<MxnetPrePost> loadPlugin(std::string pluginPath);
};


#endif //TEST_PLUGINMGR_H
