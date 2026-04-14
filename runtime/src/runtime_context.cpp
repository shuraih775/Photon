#include <photon/runtime/runtime_context.hpp>

#include <utility>

namespace photon::runtime {

RuntimeContext::RuntimeContext(RuntimeConfig config)
    : m_config(std::move(config)) {}

// Configuration

RuntimeConfig &RuntimeContext::configuration() noexcept { return m_config; }

const RuntimeConfig &RuntimeContext::configuration() const noexcept {
  return m_config;
}

// Backend Registry

photon::backend::BackendRegistry &RuntimeContext::backendRegistry() noexcept {
  return m_backendRegistry;
}

const photon::backend::BackendRegistry &
RuntimeContext::backendRegistry() const noexcept {
  return m_backendRegistry;
}

// Active Backends

void RuntimeContext::registerBackend(
    photon::backend::ExecutionEngine exec_engine,
    photon::backend::BackendPtr backend) {
  m_backends.emplace(exec_engine, std::move(backend));
}

photon::backend::IBackend *
RuntimeContext::backend(const photon::backend::ExecutionEngine &exec_engine) {
  auto it = m_backends.find(exec_engine);
  if (it == m_backends.end()) {
    return nullptr;
  }
  return it->second.get();
}

// Loaded Models

void RuntimeContext::registerModel(photon::model::ModelHandle handle,
                                   ModelPtr model) {
  m_models.emplace(handle.id(), std::move(model));
}

photon::backend::ModelInstance *
RuntimeContext::model(photon::model::ModelHandle handle) {
  auto it = m_models.find(handle.id());
  if (it == m_models.end()) {
    return nullptr;
  }
  return it->second.get();
}

} // namespace photon::runtime
