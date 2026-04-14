#pragma once

#include <memory>
#include <string>

#include <photon/backend/backend.hpp>

namespace photon::backend {

class BackendFactory {
public:
  virtual ~BackendFactory() = default;

  [[nodiscard]]
  virtual ExecutionEngine backendName() const = 0;

  [[nodiscard]]
  virtual BackendPtr create() const = 0;
};

} // namespace photon::backend