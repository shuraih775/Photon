#include <photon/backend/backend_registry.hpp>
#include <photon/backend/onnx/onnx_factory.hpp>

namespace photon::runtime {
void registerBuiltInBackends(photon::backend::BackendRegistry &registry) {
  registry.registerFactory(
      std::make_unique<photon::backend::onnx::ONNXBackendFactory>());
}

} // namespace photon::runtime