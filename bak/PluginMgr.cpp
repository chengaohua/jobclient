//
// Created by gh on 2021/6/3.
//

#include "PluginMgr.h"
#include "link.h"

PluginMgr::PluginMgr() {

}

PluginMgr::~PluginMgr() {

}

std::unique_ptr<MxnetPrePost> PluginMgr::loadPlugin(std::string pluginPath) {
  std::unique_ptr<MxnetPrePost> pre_post;
  void *handle;
  void *(*createAlgHandle)();
  handle = dlopen(pluginPath.c_str(), RTLD_LAZY);
  if (!handle) {
    fprintf(stderr, "Error:%s\n", dlerror());
    return pre_post;
  }

  *(void **) (&createAlgHandle) = dlsym(handle, "createAlgHandle");
  if (!createAlgHandle) {
    fprintf(stderr, "Error:%s\n", dlerror());
    dlclose(handle);
    return pre_post;
  }

  MxnetPrePost *prePost = (MxnetPrePost *) createAlgHandle();
  pre_post.reset(prePost);
  return pre_post;
}