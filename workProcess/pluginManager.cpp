//
// Created by gh on 2021/9/18.
//

#include "pluginManager.h"

namespace Plugin {

std::unique_ptr<PrePost> loadPlugin(std::string plugin_path) {
  std::unique_ptr<PrePost> pre_post;

  void * handle;
  void *(*createAlgHandle)();
  handle = dlopen(plugin_path.c_str(),RTLD_LAZY);
  if(!handle) {
    fprintf(stderr, "Erros:%s\n", dlerror());
    return pre_post;
  }

  *(void **)(&createAlgHandle) = dlsym(handle, "createAlgHandle");
  if(!createAlgHandle) {
    fprintf(stderr, "Error:%s\n", dlerror());
    dlclose(handle);
    return pre_post;
  }

  PrePost * impl = (PrePost *)createAlgHandle;
  pre_post.reset(impl);
  return pre_post;
}

}