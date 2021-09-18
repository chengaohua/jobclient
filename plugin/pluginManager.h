//
// Created by gh on 2021/9/18.
//

#ifndef TEST_PLUGIN_PLUGINMANAGER_H_
#define TEST_PLUGIN_PLUGINMANAGER_H_

#include <memory>
#include <link.h>
#include "prePost.h"

namespace Plugin {
std::unique_ptr<PrePost> loadPlugin(std::string plugin_path) ;
}
#endif //TEST_PLUGIN_PLUGINMANAGER_H_
