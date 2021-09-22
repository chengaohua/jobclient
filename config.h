//
// Created by gh on 2021/9/22.
//

#ifndef TEST__CONFIG_H_
#define TEST__CONFIG_H_

#include <string>

void initDeviceId(int deviceId);

void initGrpcServer(std::string server);

int getDeviceId();

std::string getGrpcServer();



#endif //TEST__CONFIG_H_
