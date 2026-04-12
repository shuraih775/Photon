#pragma once

#include <vector>

#include <photon/model/model_descriptor.hpp>

#include <photon/runtime/runtime.hpp>
#include <photon/runtime/runtime_config.hpp>

namespace photon::runtime {

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

} // namespace photon::runtime