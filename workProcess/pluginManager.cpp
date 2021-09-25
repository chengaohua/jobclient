//
// Created by gh on 2021/9/18.
//

#include "pluginManager.h"

namespace Plugin {

std::unique_ptr<PrePost> loadPlugin(std::string plugin_path) {

  plugin_path = "./libplugin.so";
  std::unique_ptr<PrePost> pre_post;

  std::cout<<"work--------------------->"<<__FILE__<<"  "<<__LINE__<<std::endl;

  void * handle;
  void *(*createAlgHandle)();
  handle = dlopen(plugin_path.c_str(),RTLD_LAZY);

  std::cout<<"work--------------------->"<<__FILE__<<"  "<<__LINE__<<std::endl;
  if(!handle) {
    fprintf(stderr, "Erros:%s\n", dlerror());
    return pre_post;
  }

  std::cout<<"work--------------------->"<<__FILE__<<"  "<<__LINE__<<std::endl;

  *(void **)(&createAlgHandle) = dlsym(handle, "createAlgHandle");
  if(!createAlgHandle) {
    fprintf(stderr, "Error:%s\n", dlerror());
    dlclose(handle);
    return pre_post;
  }

  std::cout<<"work--------------------->"<<__FILE__<<"  "<<__LINE__<<std::endl;
  PrePost * impl = (PrePost *)createAlgHandle();

  std::cout<<"work--------------------->"<<__FILE__<<"  "<<__LINE__<<std::endl;
  pre_post.reset(impl);
  return pre_post;
}

}