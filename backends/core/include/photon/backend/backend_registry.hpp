#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <photon/backend/backend.hpp>
#include <photon/backend/backend_factory.hpp>

namespace photon::backend {
class BackendRegistry {
public:
  void registerFactory(std::unique_ptr<BackendFactory> factory) {
    m_factories.emplace(factory->backendName(), std::move(factory));
  }

  [[nodiscard]]
  BackendPtr create(const ExecutionEngine &exec_engine) const {
    auto it = m_factories.find(exec_engine);

    if (it == m_factories.end()) {
      return nullptr;
    }

    return it->second->create();
  }

private:
  std::unordered_map<ExecutionEngine, std::unique_ptr<BackendFactory>>
      m_factories;
};

} // namespace photon::backend