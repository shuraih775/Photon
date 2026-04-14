#include <photon/backend/onnx/onnx_backend.hpp>
#include <photon/backend/onnx/onnx_factory.hpp>

namespace photon::backend::onnx {
ExecutionEngine ONNXBackendFactory::backendName() const {
  return ExecutionEngine::ONNXRuntime;
}

BackendPtr ONNXBackendFactory::create() const {
  return std::make_shared<ONNXBackend>();
}
} // namespace photon::backend::onnx