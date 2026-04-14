#pragma once

#include <vector>

#include <photon/core/model/model_descriptor.hpp>
#include <photon/core/runtime/runtime_config.hpp>
#include <photon/sdk/runtime/runtime.hpp>

namespace photon::sdk::runtime {
using RuntimeConfig = photon::runtime::RuntimeConfig;
using ModelDescriptor = photon::model::ModelDescriptor;
using ModelFormat = photon::model::ModelFormat;
using ModelHandle = photon::model::ModelHandle;
class RuntimeBuilder {
public:
  RuntimeBuilder() = default;

  RuntimeBuilder &configuration(const RuntimeConfig &config) {
    m_config = config;
    return *this;
  }

  RuntimeBuilder &
  registerModel(const photon::model::ModelDescriptor &descriptor) {
    m_models.push_back(descriptor);
    return *this;
  }

  [[nodiscard]]
  Runtime build();

private:
  RuntimeConfig m_config;

  std::vector<photon::model::ModelDescriptor> m_models;
};

} // namespace photon::sdk::runtime