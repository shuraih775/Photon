#pragma once

#include <photon/backend/backend.hpp>
#include <photon/backend/backend_factory.hpp>
#include <photon/backend/backend_registry.hpp>

namespace photon::backend::onnx {
class ONNXBackendFactory final : public photon::backend::BackendFactory {
public:
  photon::backend::ExecutionEngine backendName() const override;

  photon::backend::BackendPtr create() const override;
};
} // namespace photon::backend::onnx