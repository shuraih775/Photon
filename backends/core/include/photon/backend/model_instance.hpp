#pragma once

#include <memory>

#include <photon/backend/backend.hpp>
#include <photon/core/model/model_descriptor.hpp>

namespace photon::backend {

enum class ExecutionEngine {
  ONNXRuntime,
  TensorRT,
  LlamaCpp,
  OpenVINO,
  Custom
};
inline std::string ExecutionEngineToString(ExecutionEngine engine) {
  switch (engine) {
  case ExecutionEngine::ONNXRuntime:
    return "ONNXRuntime";
  case ExecutionEngine::TensorRT:
    return "TensorRT";
  case ExecutionEngine::LlamaCpp:
    return "LlamaCpp";
  case ExecutionEngine::OpenVINO:
    return "OpenVINO";
  case ExecutionEngine::Custom:
    return "Custom";
  default:
    return "Unknown";
  }
}

class ModelInstance {
public:
  virtual ~ModelInstance() = default;

  [[nodiscard]]
  virtual std::shared_ptr<const photon::model::ModelDescriptor>
  descriptor() const = 0;

  [[nodiscard]]
  virtual photon::backend::ExecutionEngine backendName() const = 0;
};

using ModelInstancePtr = std::shared_ptr<ModelInstance>;

} // namespace photon::backend