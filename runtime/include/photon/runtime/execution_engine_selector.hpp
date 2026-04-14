#pragma once

#include <photon/backend/backend.hpp>
#include <photon/core/model/model_descriptor.hpp>
#include <photon/core/runtime/runtime_config.hpp>

namespace photon::runtime {

class ExecutionEngineSelector {
public:
  photon::backend::ExecutionEngine
  select(const photon::model::ModelDescriptor &descriptor,
         const RuntimeConfig &config) const;
};

} // namespace photon::runtime