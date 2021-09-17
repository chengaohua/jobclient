//
// Created by gh on 2021/6/2.
//

#include <iostream>
#include "MxnetEngin.h"
#include <memory>
#include <vector>
#include <fstream>
#include <iostream>

#if 0
#define __MYDEBUG__ do { std::cout << __FILE__ << '[' << __LINE__ << ']' << std::endl; } while(0)
#else
#define __MYDEBUG__
#endif



static int LoadFile(const std::string &fname, std::vector<char> &buf) {
    std::ifstream fs(fname, std::ios::binary | std::ios::in);
    if (!fs.good()) {
        std::cerr << fname << "does not exist" << std::endl;
        return -1;
    }
    fs.seekg(0, std::ios::end);
    int fsize = fs.tellg();
    fs.seekg(0, std::ios::beg);
    buf.resize(fsize);
    fs.read(buf.data(), fsize);
    fs.close();
    return 0;
}

MxnetEngin::MxnetEngin() : netHandle_(nullptr) {
}

MxnetEngin::~MxnetEngin() {
    if (netHandle_ != nullptr) {
        MXPredFree(netHandle_);
        netHandle_ = nullptr;
    }
}

bool MxnetEngin::loadModel(MxnetModel &info) {
    info_ = std::make_unique<MxnetModel>(info);
    std::vector<char> param_buffer;
    std::vector<char> json_buffer;
    if (LoadFile(info.json, json_buffer) < 0) {
        return false;
    }

__MYDEBUG__;
    if (LoadFile(info.params, param_buffer) < 0) {
        return false;
    }

__MYDEBUG__;
    uint num_input_nodes = 1;
    int device_type = 1;
    int device_id = 0;

    const uint default_input_shape_idxes[2] = {0, 4};
    const uint input_shape_data[] = {
            info.inputShape.n, info.inputShape.c, info.inputShape.h, info.inputShape.w
    };
    const char *input_keys[1];
__MYDEBUG__;
    input_keys[0] = "data";


__MYDEBUG__;
    MXPredCreate(json_buffer.data(), param_buffer.data(), param_buffer.size(),
                 device_type,  // cpu
                 device_id, num_input_nodes, (const char **) input_keys,
                 default_input_shape_idxes,
                 input_shape_data, &netHandle_);

__MYDEBUG__;
    if (netHandle_ == nullptr) {
        const char *err = MXGetLastError();
        std::cout << "error!!!! netHandle_ is nullptr when init {}!!!!" << err;
        return false;
    }

__MYDEBUG__;
    return true;
}

std::vector<arctern::Tensor<float>> MxnetEngin::inference(arctern::Tensor<float> &input) {
    using arctern::TensorShape;
    using arctern::Tensor;
    MXPredSetInput(netHandle_, "data", input.data(), input.size());
    MXPredForward(netHandle_);

    auto outputNum = info_->outputNum;
    std::vector<arctern::Tensor<float>> outputTensors;
    outputTensors.reserve(outputNum);

    for (int i = 0; i < outputNum; ++i) {
        uint *oshape = NULL;
        uint oshape_len;
        MXPredGetOutputShape(netHandle_, i, &oshape, &oshape_len);

        if (oshape_len == 1) {
            outputTensors.push_back(Tensor<float>(TensorShape(oshape[0])));
        } else if (oshape_len == 2) {
            outputTensors.push_back(Tensor<float>(TensorShape(oshape[0], oshape[1])));
        } else if (oshape_len == 3) {
            outputTensors.push_back(
                    Tensor<float>(TensorShape(oshape[0], oshape[1], oshape[2])));

        } else {
            outputTensors.push_back(Tensor<float>(
                    TensorShape(oshape[0], oshape[1], oshape[2], oshape[3])));
        }
        size_t size = outputTensors[i].shape().size();
        MXPredGetOutput(netHandle_, i, outputTensors[i].data(), size);
    }
    return outputTensors;
}


