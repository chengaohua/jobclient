//
// Created by gh on 2021/9/16.
//

#include "workProcess.h"
#include <stdio.h>
#include <iostream>
#include "../hander/handerManager.h"
#include "dataset_mgr.h"
#include "../grpc/job_manager.h"
#include "pluginManager.h"
#include "../engine/engine.h"

typedef enum {
  JOB_NONE,
  JOB_RUNNING,
}JOB_STATUS;

typedef enum {
  CMD_NONE,
  CMD_CREATE,
  CMD_CANCEL,
} CMD_TYPE;

typedef struct {
  CMD_TYPE cmd = CMD_NONE;
  int task_id = 0;
} CMD;

CMD g_cmd;

static int g_job_id = 0;
static JOB_STATUS g_job_status = JOB_NONE;
void sig_siguser1_from_parent(int signum, siginfo_t *info, void *myact) {
// new job
  g_cmd.cmd =CMD_CREATE;
  g_cmd.task_id = info->si_int;
}
void sig_siguser2_from_parent(int signum, siginfo_t *info, void *myact) {
  g_cmd.cmd =CMD_CANCEL;
  g_cmd.task_id = info->si_int;
}

typedef enum {
  OPERATOR_CONTINUE,
  OPERATOR_RESET,
  OPERATRO_CANCEL,
} OPERATOR;

OPERATOR status_check() {
  if(CMD_NONE == g_cmd.cmd ) {
    if(JOB_NONE == g_job_status) {
      sigsuspend(nullptr);
      return OPERATOR_CONTINUE;
    } else if(JOB_RUNNING == g_job_status) {
      return OPERATOR_CONTINUE;
    }
  }

  if(CMD_CREATE == g_cmd.cmd) {
    if(JOB_NONE == g_job_status) {
      // begin run job
      g_job_id = g_cmd.task_id;

      return OPERATOR_RESET;
    } else if(JOB_RUNNING == g_job_status) {
      //continue run
      g_job_id = g_cmd.task_id;
      return OPERATOR_RESET;
    }

  }

  if(CMD_CANCEL == g_cmd.cmd) {
    if(JOB_NONE == g_job_status) {
      // begin run job
      return OPERATRO_CANCEL;
    } else if(JOB_RUNNING == g_job_status) {
      //continue run
      return OPERATRO_CANCEL;
    }
  }

  return OPERATOR_CONTINUE;

}

void run_work() {
  std::cout<<"run_work"<<std::endl;
  auto registerSignal = [](int signum, decltype(sig_siguser1_from_parent) func) {
    struct sigaction act;
    int sig = signum;

    sigemptyset(&act.sa_mask);
    act.sa_sigaction = func;
    act.sa_flags = SA_SIGINFO;

    if (sigaction(sig, &act, NULL) < 0) {
      printf("install sigal error＼n");
    }
  };

  registerSignal(SIGUSR1, sig_siguser1_from_parent);
  registerSignal(SIGUSR2, sig_siguser2_from_parent);

  std::string target_str = "0.0.0.0:50051";
  JobManagerClient greeter(
      grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));


  while (true) {
    // std::cout<<"run child ! \n";
    auto op = status_check();
    if(OPERATOR_RESET == op) {
      std::tuple<std::string, std::string, std::string> model;
      greeter.GetJobAimodel(g_job_id, model);

      std::string dataset_path;
      greeter.GetJobDataset(g_job_id, dataset_path);

      DatasetMgr dataset_mgr;
      dataset_mgr.parse(dataset_path);

      auto plugin_path = std::get<0>(model);
      auto prePost = Plugin::loadPlugin(plugin_path);

      std::vector<uint>  input_dims;
      int  output_num;
      std::vector<std::string>  output_layers;
      prePost->initModel(input_dims, output_num, output_layers);

      std::string model_json = std::get<1>(model);
      std::string model_param = std::get<2>(model);

      EngineFactory::ModelInfo model_info;
      model_info.json = model_json;
      model_info.params = model_param;
      model_info.outputNum = output_num;
      model_info.inputDims = input_dims;

      auto engine = EngineFactory::buildEngine(model_info);
      InputParam input_param;
      int idx = 0;
      std::string file_name = std::to_string(g_job_id) + ".txt";
      std::ofstream of(file_name, std::ios::trunc| std::ios::out);

      while (dataset_mgr.next(input_param) == 0) {
        if(idx == 0) {
          of<<"[";
        }else {
          of <<",";
        }
        auto input_tensor =prePost->PreProcess(input_param);
        auto output_tensors = engine->inference(input_tensor);
        auto result = prePost->PostProcess(output_tensors);
        of<<result;
        idx ++;
      }
      of <<"]";

    }

  }
  // std::cout<<" run child end!"<<std::endl;
  return;
}