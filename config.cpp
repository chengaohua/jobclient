//
// Created by gh on 2021/9/22.
//

#include "config.h"

static int g_deviceId;
static std::string g_server;

void initDeviceId(int deviceId) {
  g_deviceId = deviceId;
}

void initGrpcServer(std::string server) {
  g_server = server;
}

int getDeviceId() {
  return g_deviceId;
}

std::string getGrpcServer() {
  return g_server;
}
