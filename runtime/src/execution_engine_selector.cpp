#include <photon/backend/backend.hpp>
#include <photon/runtime/execution_engine_selector.hpp>

namespace photon::runtime {

photon::backend::ExecutionEngine ExecutionEngineSelector::select(
    const photon::model::ModelDescriptor &descriptor,
    const RuntimeConfig &config) const {

  switch (descriptor.format()) {
  case photon::model::ModelFormat::ONNX:
    return photon::backend::ExecutionEngine::ONNXRuntime;

  case photon::model::ModelFormat::TensorRTEngine:
    return photon::backend::ExecutionEngine::TensorRT;

  case photon::model::ModelFormat::GGUF:
    return photon::backend::ExecutionEngine::LlamaCpp;

  case photon::model::ModelFormat::TorchScript:
    return photon::backend::ExecutionEngine::Custom;

  case photon::model::ModelFormat::OpenVINO:
    return photon::backend::ExecutionEngine::OpenVINO;

  default:
    return photon::backend::ExecutionEngine::Custom;
  }
}
}