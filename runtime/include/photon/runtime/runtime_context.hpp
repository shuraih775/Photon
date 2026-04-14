#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <photon/backend/backend.hpp>
#include <photon/backend/backend_registry.hpp>
#include <photon/backend/model_instance.hpp>
#include <photon/core/model/model_descriptor.hpp>
#include <photon/core/model/model_handle.hpp>
#include <photon/core/runtime/runtime_config.hpp>

namespace photon::runtime {

class RuntimeContext {
public:
  using ModelPtr = photon::backend::ModelInstancePtr;

  RuntimeContext() = default;
  explicit RuntimeContext(RuntimeConfig config);

  // Configuration
  [[nodiscard]] RuntimeConfig &configuration() noexcept;
  [[nodiscard]] const RuntimeConfig &configuration() const noexcept;

  // Backend Registry
  [[nodiscard]] photon::backend::BackendRegistry &backendRegistry() noexcept;
  [[nodiscard]] const photon::backend::BackendRegistry &
  backendRegistry() const noexcept;

  // Active Backends
  void registerBackend(photon::backend::ExecutionEngine exec_engine,
                       photon::backend::BackendPtr backend);
  [[nodiscard]] photon::backend::IBackend *
  backend(const photon::backend::ExecutionEngine &exec_engine);

  // Loaded Models
  void registerModel(photon::model::ModelHandle handle, ModelPtr model);
  [[nodiscard]] photon::backend::ModelInstance *
  model(photon::model::ModelHandle handle);

private:
  RuntimeConfig m_config;
  photon::backend::BackendRegistry m_backendRegistry;
  std::unordered_map<photon::backend::ExecutionEngine,
                     photon::backend::BackendPtr>
      m_backends;
  std::unordered_map<photon::model::ModelHandle::IdType, ModelPtr> m_models;
};

} // namespace photon::runtime
