/*
Copyright 2020 The OneFlow Authors. All rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#ifndef ONEFLOW_USER_KERNELS_RANDOM_MASK_GENERATOR_H_
#define ONEFLOW_USER_KERNELS_RANDOM_MASK_GENERATOR_H_

#include "oneflow/core/common/data_type.h"
#include "oneflow/core/ep/include/stream.h"
#include "oneflow/core/framework/random_generator.h"
#ifdef WITH_ROCM
#include <hiprand.h>
#include <hiprand_kernel.h>
#else
#include <curand.h>
#include <curand_kernel.h>
#endif


namespace oneflow {

template<DeviceType device_type>
class RandomMaskGenerator;

template<>
class RandomMaskGenerator<DeviceType::kCPU> final {
 public:
  OF_DISALLOW_COPY_AND_MOVE(RandomMaskGenerator);
  RandomMaskGenerator(const std::shared_ptr<one::Generator>& generator,
                      const int device_index = -1) {
    generator_ = CHECK_JUST(generator->Get<ep::CPUGenerator>(device_index));
  }
  ~RandomMaskGenerator() = default;

  void Generate(ep::Stream* stream, int64_t n, float rate, bool* mask);

 private:
  std::shared_ptr<ep::CPUGenerator> generator_;
};

#if defined(WITH_CUDA) || defined(WITH_ROCM)
template<>
class RandomMaskGenerator<DeviceType::kCUDA> final {
 public:
  OF_DISALLOW_COPY_AND_MOVE(RandomMaskGenerator);
  RandomMaskGenerator(const std::shared_ptr<one::Generator>& generator,
                      const int device_index = -1) {
    generator_ = CHECK_JUST(generator->Get<ep::CUDAGenerator>(device_index));
  }
  ~RandomMaskGenerator() = default;

  void Generate(ep::Stream* stream, int64_t n, float rate, bool* mask);

 private:
  std::shared_ptr<ep::CUDAGenerator> generator_;
};
#endif

}  // namespace oneflow

#endif  // ONEFLOW_USER_KERNELS_RANDOM_MASK_GENERATOR_H_
