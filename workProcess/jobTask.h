//
// Created by gh on 2021/9/20.
//

#ifndef TEST_WORKPROCESS_JOBMANAGER_H_
#define TEST_WORKPROCESS_JOBMANAGER_H_

#include <vector>
#include <string>
#include <memory>
#include "../grpc/job_manager.h"
#include "dataset_mgr.h"
#include "pluginManager.h"
#include "../engine/engine.h"

class JobTask {
 public:
  explicit JobTask(int job_id);
  JobTask(const JobTask &) = delete;
  JobTask & operator=(const JobTask &) = delete;
  ~JobTask();

  int next();

 private:
  int init();

  int report();

  int reportEnd();
 private:
  std::string so_path_ ;
  std::string aimodel_json_ ;
  std::string aimodel_params_ ;
  std::string dataset_path_;
  int job_id_;

  std::unique_ptr<JobManagerClient> grpc_client_;
  std::unique_ptr<PrePost> pre_post_;
  DatasetMgr dataset_mgr_;
  std::unique_ptr<Engine> engine_;
  bool init_ok;

  std::vector<std::string> alg_result_;

  long last_access_time_ = 0;
  double alg_exe_sum_time_ = 0;
  int frame_id_ = 0;







};

#endif //TEST_WORKPROCESS_JOBMANAGER_H_
